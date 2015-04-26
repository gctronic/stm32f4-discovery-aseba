#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "usbcfg.h"

#include "common/consts.h"
#include "common/productids.h"
#include "vm/vm.h"
#include "skel.h"
#include "skel_user.h"
#include "aseba_node.h"

static THD_WORKING_AREA(aseba_vm_thd_wa, 1024);
static THD_FUNCTION(aseba_vm_thd, arg)
{
    (void)arg;

    aseba_vm_init();

    while (TRUE) {
        palTogglePad(GPIOD, GPIOD_LED6);
        chThdSleepMilliseconds(100);

        chprintf((BaseSequentialStream *)&SDU1, "vm pc0: %d\n", vmState.pc);

        update_robot_variables();
        AsebaProcessIncomingEvents(&vmState);
        AsebaVMRun(&vmState, 10);
    }
    return 0;
}

void aseba_vm_init(void)
{
    vmState.nodeId = 1;

    AsebaVMInit(&vmState);
    vmVariables.id = vmState.nodeId;
    vmVariables.productId = ASEBA_PID_THYMIO2;

    // vmState.bytecode[1] = (ASEBA_BYTECODE_SMALL_IMMEDIATE << 12) | 20; // push 20 on stack
    // vmState.bytecode[2] = (ASEBA_BYTECODE_SMALL_IMMEDIATE << 12) | 22; // push 22 on stack
    // vmState.bytecode[3] = (ASEBA_BYTECODE_NATIVE_CALL << 12) | 20; // call sqrt on 22 (last thing on stack)

    // vmState.bytecode[1] = (ASEBA_BYTECODE_SMALL_IMMEDIATE << 12) | 1024; // push 1024 on stack
    // vmState.bytecode[2] = (ASEBA_BYTECODE_UNARY_ARITHMETIC << 12) | 0; // sub 1 to value on stack
    // //vmState.bytecode[3] = (ASEBA_BYTECODE_JUMP << 12) | 0x0fff; // jump by -1
    // vmState.bytecode[3] = (ASEBA_BYTECODE_STOP << 12); // stop

    vmState.bytecode[0] =  0x4142;
    vmState.bytecode[1] =  0x4f00;
    vmState.bytecode[2] =  0x0000;
    vmState.bytecode[3] =  0x0400;
    vmState.bytecode[4] =  0x0800;
    vmState.bytecode[5] =  0x0600;
    vmState.bytecode[6] =  0x3415;
    vmState.bytecode[7] =  0x0442;
    vmState.bytecode[8] =  0x2ceb;
    vmState.bytecode[9] =  0x1900;
    vmState.bytecode[10] = 0x0300;
    vmState.bytecode[11] = 0xffff;
    vmState.bytecode[12] = 0x0300;
    vmState.bytecode[13] = 0x0020;
    vmState.bytecode[14] = 0x3075;
    vmState.bytecode[15] = 0x5c40;
    vmState.bytecode[16] = 0x5c30;
    vmState.bytecode[17] = 0x0110;
    vmState.bytecode[18] = 0x0da0;
    vmState.bytecode[19] = 0x1000;
    vmState.bytecode[20] = 0x5c30;
    vmState.bytecode[21] = 0x0110;
    vmState.bytecode[22] = 0x0380;
    vmState.bytecode[23] = 0x5b42;
    vmState.bytecode[24] = 0x5b32;
    vmState.bytecode[25] = 0x5c40;
    vmState.bytecode[26] = 0x5c30;
    vmState.bytecode[27] = 0x0010;
    vmState.bytecode[28] = 0x0aa0;
    vmState.bytecode[29] = 0x0500;
    vmState.bytecode[30] = 0x0020;
    vmState.bytecode[31] = 0x3075;
    vmState.bytecode[32] = 0x5c40;
    vmState.bytecode[33] = 0xef9f;
    vmState.bytecode[34] = 0x0000;
    vmState.bytecode[35] = 0x6cef;

    chprintf((BaseSequentialStream *)&SDU1, "vm pc1: %d\n", vmState.pc);

    AsebaVMSetupEvent(&vmState, ASEBA_EVENT_INIT);

    chprintf((BaseSequentialStream *)&SDU1, "vm pc2: %d\n", vmState.pc);

    AsebaVMRun(&vmState, 10);

    chprintf((BaseSequentialStream *)&SDU1, "vm pc3: %d\n", vmState.pc);

    palSetPad(GPIOD, GPIOD_LED6);
    chThdSleepMilliseconds(200);
    palClearPad(GPIOD, GPIOD_LED6);
    chThdSleepMilliseconds(200);
    palSetPad(GPIOD, GPIOD_LED6);
    chThdSleepMilliseconds(200);
    palClearPad(GPIOD, GPIOD_LED6);
}

void aseba_vm_start(void)
{
    chThdCreateStatic(aseba_vm_thd_wa, sizeof(aseba_vm_thd_wa), NORMALPRIO, aseba_vm_thd, NULL);
}

void update_robot_variables(void)
{

}
