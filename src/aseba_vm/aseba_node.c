#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "usbcfg.h"
#include "unique_id.h"
#include <strings.h>
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

    AsebaVMSetupEvent(&vmState, ASEBA_EVENT_INIT);

    while (TRUE) {
        // Don't spin too fast to avoid consuming all CPU time
        chThdYield();

        // Sync Aseba with the state of the Microcontroller
        update_aseba_variables_read();

        // Run VM for some time
        AsebaVMRun(&vmState, 1000);
        AsebaProcessIncomingEvents(&vmState);

        // Sync the Microcontroller with the state of Aseba
        update_aseba_variables_write();

		// Either we are in step by step, so we go to sleep until further commands, or we are not executing an event,
		// and so we have nothing to do.
		if (AsebaMaskIsSet(vmState.flags, ASEBA_VM_STEP_BY_STEP_MASK) || AsebaMaskIsClear(vmState.flags, ASEBA_VM_EVENT_ACTIVE_MASK))
		{
			unsigned i;
			// Find first bit from right (LSB), check if some CAN packets are pending
			// Atomically, do an Idle() if nothing is found
            i = ffs(events_flags);

			// If a local eventCLEAR_EVENT is pending, then execute it.
			// Else, we waked up from idle because of an interrupt, so re-execute the whole thing
			// FIXME: do we want to kill execution upon local events? that would be consistant so Steph votes yes, but
			// we may discuss further
			if(i && !(AsebaMaskIsSet(vmState.flags, ASEBA_VM_STEP_BY_STEP_MASK) &&  AsebaMaskIsSet(vmState.flags, ASEBA_VM_EVENT_ACTIVE_MASK))) {
				i--;
				CLEAR_EVENT(i);
				vmVariables.source = vmState.nodeId;
				AsebaVMSetupEvent(&vmState, ASEBA_EVENT_LOCAL_EVENTS_START - i);
            }
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
    char name[32];
    vmState.nodeId = get_unique_id();

    sprintf(name, "Discovery %d", vmState.nodeId);
    set_board_name(name);

    AsebaVMInit(&vmState);
    vmVariables.id = vmState.nodeId;
    vmVariables.productId = ASEBA_PID_UNDEFINED;
    vmVariables.fwversion[0] = 0;
    vmVariables.fwversion[1] = 1;

    vmVariables.leds[0] = 0;
    vmVariables.leds[1] = 0;
    vmVariables.leds[2] = 0;
    vmVariables.leds[3] = 0;
    vmVariables.leds[4] = 0;
    vmVariables.leds[5] = 0;

    vmVariables.acc[0] = 0.0f;
    vmVariables.acc[1] = 0.0f;
    vmVariables.acc[2] = 0.0f;

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

// This function must update the variable to match the microcontroller state
void update_aseba_variables_read(void)
{
    // accelerometer_cb();
}

// This function must update the microcontrolleur state to match the variables
void update_aseba_variables_write(void)
{

}
