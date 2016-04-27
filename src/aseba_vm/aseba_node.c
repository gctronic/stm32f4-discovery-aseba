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
#include "vm/vm.h"
#include "transport/buffer/vm-buffer.h"
#include "aseba_vm/skel.h"
#include "aseba_vm/skel_user.h"
#include "aseba_vm/aseba_node.h"

#include "discovery_demo/accelerometer.h"
#include "discovery_demo/leds.h"


void update_aseba_variables_read(void);
void update_aseba_variables_write(void);
sint16 aseba_float_to_int(float var, float max);

static THD_WORKING_AREA(aseba_vm_thd_wa, 1024);
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
    chThdCreateStatic(aseba_vm_thd_wa, sizeof(aseba_vm_thd_wa), LOWPRIO, aseba_vm_thd, NULL);
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
