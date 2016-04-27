#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "usbcfg.h"
#include "unique_id.h"
#include <string.h>
#include <stdio.h>

#include "common/types.h"
#include "common/consts.h"
#include "common/productids.h"
#include "transport/buffer/vm-buffer.h"
#include "aseba_vm/skel_user.h"
#include "aseba_vm/aseba_node.h"
#include "aseba_vm/aseba_bridge.h"
#include "flash/flash.h"

#include "discovery_demo/accelerometer.h"
#include "discovery_demo/leds.h"

void update_aseba_variables_read(void);
void update_aseba_variables_write(void);
sint16 aseba_float_to_int(float var, float max);

unsigned int events_flags = 0;
static uint16 vmBytecode[VM_BYTECODE_SIZE];
static sint16 vmStack[VM_STACK_SIZE];

AsebaVMState vmState = {
    .nodeId=0, /* changed by aseba_vm_init() */

    .bytecodeSize=VM_BYTECODE_SIZE,
    .bytecode=vmBytecode,

    .variablesSize=sizeof(vmVariables) / sizeof(sint16),
    .variables=(sint16*)&vmVariables,

    .stackSize=VM_STACK_SIZE,
    .stack=vmStack,
    .flags=0, .pc=0, .sp=0,
    .breakpoints={0}, .breakpointsCount=0,
};


static THD_FUNCTION(aseba_vm_thd, arg)
{
    (void)arg;

    chRegSetThreadName("aseba");

    AsebaVMSetupEvent(&vmState, ASEBA_EVENT_INIT);

    while (TRUE) {
        // Don't spin too fast to avoid consuming all CPU time
        chThdYield();

        // Run VM for some time
        AsebaVMRun(&vmState, 1000);
        AsebaProcessIncomingEvents(&vmState);

        // Do not process events in step by step mode
        if (AsebaMaskIsSet(vmState.flags, ASEBA_VM_STEP_BY_STEP_MASK)) {
            continue;
        }

        // If we are not executing an event, there is nothing to do
        if (AsebaMaskIsSet(vmState.flags, ASEBA_VM_EVENT_ACTIVE_MASK)) {
            continue;
        }

        int event = ffs(events_flags) - 1;

        // If a local event is pending, then execute it.
        if (event != - 1) {

            CLEAR_EVENT(event);

            vmVariables.source = vmState.nodeId;

            AsebaVMSetupEvent(&vmState, ASEBA_EVENT_LOCAL_EVENTS_START - event);
        }
    }
    return 0;
}


/** Returns a more or less unique node ID.*/
static uint16_t get_unique_id(void)
{
    uint8_t chip_id[UNIQUE_ID_SIZE];
    uint8_t res = 0;
    int i;

    unique_id_read(chip_id);

    for (i = 0; i < UNIQUE_ID_SIZE; i++) {
        res += chip_id[i];
    }

    return res;
}

void aseba_vm_init(void)
{
    uint16_t bytecode_size;
    vmState.nodeId = get_unique_id();

    AsebaVMInit(&vmState);

    /* Initializes constant variables. */
    memset(&vmVariables, 0, sizeof(vmVariables));
    vmVariables.id = vmState.nodeId;
    vmVariables.productId = ASEBA_PID_UNDEFINED;
    vmVariables.fwversion[0] = 0;
    vmVariables.fwversion[1] = 1;

    extern uint8_t _aseba_bytecode_start;
    uint8_t *pos = &_aseba_bytecode_start;

    memcpy(&bytecode_size, pos, sizeof(bytecode_size));

    pos += sizeof(uint16_t);

    // Check if the bytecode page was erased
    if (bytecode_size != 0xffff) {
        memcpy(vmState.bytecode, pos, bytecode_size);
    }

    chThdSleepMilliseconds(500);

    AsebaVMSetupEvent(&vmState, ASEBA_EVENT_INIT);
}

void aseba_vm_start(void)
{
    static THD_WORKING_AREA(aseba_vm_thd_wa, 1024);
    chThdCreateStatic(aseba_vm_thd_wa, sizeof(aseba_vm_thd_wa), LOWPRIO, aseba_vm_thd, NULL);
}

void AsebaIdle(void)
{
    chThdYield();
}

void AsebaPutVmToSleep(AsebaVMState *vm)
{
    (void) vm;
    chThdSleepMilliseconds(1000);
}

void AsebaResetIntoBootloader(AsebaVMState *vm)
{
    (void) vm;
    NVIC_SystemReset();
}

void AsebaNativeFunction(AsebaVMState *vm, uint16 id)
{
    if (id < nativeFunctions_length) {
        nativeFunctions[id](vm);
    } else {
        AsebaVMEmitNodeSpecificError(vm, "Invalid native function.");
    }
}

const AsebaNativeFunctionDescription * const * AsebaGetNativeFunctionsDescriptions(AsebaVMState *vm)
{
    (void) vm;
    return nativeFunctionsDescription;
}


const AsebaVMDescription* AsebaGetVMDescription(AsebaVMState *vm)
{
    (void) vm;
    return &vmDescription;
}

const AsebaLocalEventDescription * AsebaGetLocalEventsDescriptions(AsebaVMState *vm)
{
    (void) vm;
    return localEvents;
}


uint16 AsebaShouldDropPacket(uint16 source, const uint8* data)
{
    /* Accept all packets in bridge mode. */
    if (aseba_is_bridge()) {
        return 0;
    }

    return AsebaVMShouldDropPacket(&vmState, source, data);
}

void AsebaWriteBytecode(AsebaVMState *vm)
{
    extern uint8_t _aseba_bytecode_start;

    flash_unlock();
    flash_sector_erase(&_aseba_bytecode_start);
    flash_write(&_aseba_bytecode_start, &vm->bytecodeSize, sizeof(uint16));
    flash_write(&_aseba_bytecode_start + sizeof(uint16), vm->bytecode, vm->bytecodeSize);
    flash_lock();
}



// This function must update the accelerometer variables
void accelerometer_cb(void)
{
    static float accf[3];
    demo_acc_get_acc(accf);
    vmVariables.acc[0] = (sint16) accf[0];
    vmVariables.acc[1] = (sint16) accf[1];
    vmVariables.acc[2] = (sint16) accf[2];
    SET_EVENT(EVENT_ACC);
}
