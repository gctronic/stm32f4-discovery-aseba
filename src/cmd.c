#include <math.h>
#include <string.h>
#include <stdio.h>

#include "hal.h"
#include "test.h"
#include "chprintf.h"
#include "shell.h"
#include "usbcfg.h"
#include "chtm.h"
#include "common/types.h"
#include "vm/natives.h"
#include "main.h"
#include "config_flash_storage.h"

#define TEST_WA_SIZE        THD_WORKING_AREA_SIZE(256)

static void cmd_mem(BaseSequentialStream *chp, int argc, char *argv[])
{
    size_t n, size;

    (void)argv;
    if (argc > 0) {
        chprintf(chp, "Usage: mem\r\n");
        return;
    }
    n = chHeapStatus(NULL, &size);
    chprintf(chp, "core free memory : %u bytes\r\n", chCoreGetStatusX());
    chprintf(chp, "heap fragments     : %u\r\n", n);
    chprintf(chp, "heap free total    : %u bytes\r\n", size);
}

static void cmd_threads(BaseSequentialStream *chp, int argc, char *argv[])
{
    static const char *states[] = {CH_STATE_NAMES};
    thread_t *tp;

    (void)argv;
    if (argc > 0) {
        chprintf(chp, "Usage: threads\r\n");
        return;
    }
    chprintf(chp, "        addr        stack prio refs         state\r\n");
    tp = chRegFirstThread();
    do {
        chprintf(chp, "%08lx %08lx %4lu %4lu %9s\r\n",
                         (uint32_t)tp, (uint32_t)tp->p_ctx.r13,
                         (uint32_t)tp->p_prio, (uint32_t)(tp->p_refs - 1),
                         states[tp->p_state]);
        tp = chRegNextThread(tp);
    } while (tp != NULL);
}

static void cmd_test(BaseSequentialStream *chp, int argc, char *argv[])
{
    thread_t *tp;

    (void)argv;
    if (argc > 0) {
        chprintf(chp, "Usage: test\r\n");
        return;
    }
    tp = chThdCreateFromHeap(NULL, TEST_WA_SIZE, chThdGetPriorityX(),
                                                     TestThread, chp);
    if (tp == NULL) {
        chprintf(chp, "out of memory\r\n");
        return;
    }
    chThdWait(tp);
}

static void cmd_readclock(BaseSequentialStream *chp, int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    chprintf(chp, "SYSCLK: %i \n HCLK: %i \n PCLK1  %i \n PCLK2 %i \n",
        STM32_SYSCLK, STM32_HCLK, STM32_PCLK1, STM32_PCLK2);
}


extern sint16 aseba_sqrt(sint16 num);

static void cmd_sqrt(BaseSequentialStream *chp, int argc, char *argv[])
{
    uint16_t input, result;
    float x;
    time_measurement_t tmp;
    chTMObjectInit(&tmp);

    if(argc != 2) {
        chprintf(chp, "Usage: sqrt mode int\r\nModes: a (aseba), b (math), c (assembler) is default mode\r\n");
    } else {
        input =(uint16_t) atoi(argv[1]);

        if(!strcmp(argv[0], "a")) {
            chSysLock();
            chTMStartMeasurementX(&tmp);
            result = aseba_sqrt(input);
            chTMStopMeasurementX(&tmp);
            chSysUnlock();
        } else if(!strcmp(argv[0], "b")) {
            chSysLock();
            chTMStartMeasurementX(&tmp);
            result = sqrtf(input);
            chTMStopMeasurementX(&tmp);
            chSysUnlock();
        } else {
            chSysLock();
            chTMStartMeasurementX(&tmp);
            x = (float) input;
            __asm__ volatile(
                "vsqrt.f32 %[var], %[var]"
                : [var]"+t"(x)
            );
            result =(uint16_t) x;
            chTMStopMeasurementX(&tmp);
            chSysUnlock();
        }

        chprintf(chp, "sqrt(%u) = %u \r\n", input, result);
        chprintf(chp, "time: %u \r\n", tmp.last);
    }
}

extern sint16 aseba_atan2(sint16 y, sint16 x);

static void cmd_atan2(BaseSequentialStream *chp, int argc, char *argv[])
{
    int16_t a, b, result;
    time_measurement_t tmp;
    chTMObjectInit(&tmp);

    if(argc != 3) {
        chprintf(chp, "Usage: atan2 mode a b\r\nModes: a (aseba), b (math) is default mode\r\n");
    } else {
        a =(int16_t) atoi(argv[1]);
        b =(int16_t) atoi(argv[2]);

        if(!strcmp(argv[0], "a")) {
            chSysLock();
            chTMStartMeasurementX(&tmp);
            result = aseba_atan2(a, b);
            chTMStopMeasurementX(&tmp);
            chSysUnlock();
        } else {
            chSysLock();
            chTMStartMeasurementX(&tmp);
            result =(int16_t)(atan2f(a, b) * 32768 / M_PI);
            chTMStopMeasurementX(&tmp);
            chSysUnlock();
        }


        chprintf(chp, "atan2(%d, %d) = %d \r\n", a, b, result);
        chprintf(chp, "time: %u \r\n", tmp.last);
    }
}

static void tree_indent(BaseSequentialStream *out, int indent)
{
    int i;
    for (i = 0; i < indent; ++i) {
        chprintf(out, "  ");
    }
}

static void show_config_tree(BaseSequentialStream *out, parameter_namespace_t *ns, int indent)
{
    parameter_t *p;
    char string_buf[64];

    tree_indent(out, indent);
    chprintf(out, "%s:\r\n", ns->id);

    for (p=ns->parameter_list; p!=NULL; p=p->next) {
        tree_indent(out, indent + 1);
        if (parameter_defined(p)) {
            switch (p->type) {
                case _PARAM_TYPE_SCALAR:
                    chprintf(out, "%s: %f\r\n", p->id, parameter_scalar_get(p));
                    break;

                case _PARAM_TYPE_INTEGER:
                    chprintf(out, "%s: %d\r\n", p->id, parameter_integer_get(p));
                    break;

                case _PARAM_TYPE_BOOLEAN:
                    chprintf(out, "%s: %s\r\n", p->id, parameter_boolean_get(p)?"true":"false");
                    break;

                case _PARAM_TYPE_STRING:
                    parameter_string_get(p, string_buf, sizeof(string_buf));
                    chprintf(out, "%s: %s\r\n", p->id, string_buf);
                    break;

                default:
                    chprintf(out, "%s: unknown type %d\r\n", p->id, p->type);
                    break;
            }
        } else {
            chprintf(out, "%s: [not set]\r\n", p->id);
        }
    }

    if (ns->subspaces) {
        show_config_tree(out, ns->subspaces, indent + 1);
    }

    if (ns->next) {
        show_config_tree(out, ns->next, indent);
    }
}

static void cmd_config_tree(BaseSequentialStream *chp, int argc, char **argv)
{
    parameter_namespace_t *ns;
    if (argc != 1) {
        ns = &parameter_root;
    } else {
        ns = parameter_namespace_find(&parameter_root, argv[0]);
        if (ns == NULL) {
            chprintf(chp, "Cannot find subtree.\r\n");
            return;
        }
    }

    show_config_tree(chp, ns, 0);
}

static void cmd_config_set(BaseSequentialStream *chp, int argc, char **argv)
{
    parameter_t *param;
    int value_i;

    if (argc != 2) {
        chprintf(chp, "Usage: config_set /parameter/url value.\r\n");
        return;
    }

    param = parameter_find(&parameter_root, argv[0]);

    if (param == NULL) {
        chprintf(chp, "Could not find parameter \"%s\"\r\n", argv[0]);
        return;
    }

    switch (param->type) {
        case _PARAM_TYPE_INTEGER:
            if (sscanf(argv[1], "%d", &value_i) == 1) {
                parameter_integer_set(param, value_i);
            } else {
                chprintf(chp, "Invalid value for integer parameter.\r\n");
            }
            break;

        case _PARAM_TYPE_BOOLEAN:
            if (!strcmp(argv[1], "true")) {
                parameter_boolean_set(param, true);
            } else if (!strcmp(argv[1], "false")) {
                parameter_boolean_set(param, false);
            } else {
                chprintf(chp, "Invalid value for boolean parameter, must be true or false.\r\n");
            }
            break;

        case _PARAM_TYPE_STRING:
            if (argc == 2) {
                parameter_string_set(param, argv[1]);
            } else {
                chprintf(chp, "Invalid value for string parameter, must not use spaces.\r\n");
            }
            break;

        default:
            chprintf(chp, "%s: unknown type %d\r\n", param->id, param->type);
            break;
    }
}

static void cmd_config_erase(BaseSequentialStream *chp, int argc, char **argv)
{
    (void) argc;
    (void) argv;
    (void) chp;
    extern uint32_t _config_start;

    config_erase(&_config_start);
}

static void cmd_config_save(BaseSequentialStream *chp, int argc, char **argv)
{
    (void) argc;
    (void) argv;
    extern uint32_t _config_start, _config_end;
    size_t len = (size_t)(&_config_end - &_config_start);
    bool success;

    // First write the config to flash
    config_save(&_config_start, len, &parameter_root);

    // Second try to read it back, see if we failed
    success = config_load(&parameter_root, &_config_start, len);

    if (success) {
        chprintf(chp, "OK.\r\n");
    } else {
        chprintf(chp, "Save failed.\r\n");
    }
}

static void cmd_config_load(BaseSequentialStream *chp, int argc, char **argv)
{
    (void) argc;
    (void) argv;
    extern uint32_t _config_start, _config_end;
    size_t len = (size_t)(&_config_end - &_config_start);
    bool success;

    success = config_load(&parameter_root, &_config_start, len);

    if (success) {
        chprintf(chp, "OK.\r\n");
    } else {
        chprintf(chp, "Load failed.\r\n");
    }
}


const ShellCommand shell_commands[] = {
    {"mem", cmd_mem},
    {"threads", cmd_threads},
    {"test", cmd_test},
    {"clock", cmd_readclock},
    {"sqrt", cmd_sqrt},
    {"atan2", cmd_atan2},
    {"config_tree", cmd_config_tree},
    {"config_set", cmd_config_set},
    {"config_save", cmd_config_save},
    {"config_load", cmd_config_load},
    {"config_erase", cmd_config_erase},
    {NULL, NULL}
};


void shell_start(BaseSequentialStream *interface)
{
    static THD_WORKING_AREA(wa, 2048);
    static ShellConfig shell_cfg;

    shell_cfg.sc_channel = interface;
    shell_cfg.sc_commands = shell_commands;

    shellInit();
    shellCreateStatic(&shell_cfg, wa, sizeof(wa), NORMALPRIO);
}
