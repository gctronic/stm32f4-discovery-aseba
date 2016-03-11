#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "usbcfg.h"

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
        chThdSleepMilliseconds(1);

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
			// Atomstidhcp-1-064ically, do an Idle() if nothing is found
			// Warning, I'm doing some nasty things with the IPL bits (forcing the value, without safeguard)
			// Second warning: It doesn't disable level 7 interrupt. So if you set an event inside a level 7 interrupt
			// It may get delayed until next interrupt
			/* Original dsPIC33 code ...

			asm __volatile__ ("mov #SR, w0 \r\n"
							"mov #0xC0, w1\r\n"
							"ior.b w1, [w0], [w0]\r\n" // Disable all interrupts (except level 7) by setting IPL to 6
							"ff1r [%[word]], %[b]\r\n" // Find the LSb. If there is one then the C flag is cleared
							"bra nc, 1f\r\n"		   // Then branch to label 1 if there was one LSb
							"rcall _AsebaCanRecvBufferEmpty\r\n"  // Else test if received something via Can bus
					  		"cp0 w0\r\n"      // If received something then 0
							"bra z, 1f \r\n"  // Then branch to label 1 if received something
							"rcall _clock_idle\r\n"  // Powersave WITH interrupt disabled. It works, read section 6.2.5 of dsPIC manual
							"1: \r\n"
							"mov #SR, w0 \r\n"		 //
							"mov #0x1F, w1\r\n"
							"and.b w1, [w0],[w0] \r\n" // enable interrupts
							 : [b] "=x" (i) : [word] "r" (&events_flags) : "cc", "w0", "w1", "w2", "w3", "w4", "w5", "w6", "w7");
							// Why putting "x" as constrain register. Because it's w8-w9, so it's preserved accross function call
							// we do a rcall, so we must clobber w0-w7
            */
            // ... simplified by just ...
//            i = __builtin_ffs(events_flags);    // ... Find first set bit in events_flags
            i = ffs(events_flags);    // ... Find first set bit in events_flags

			// If a local eventCLEAR_EVENT is pending, then execute it.
			// Else, we waked up from idle because of an interrupt, so re-execute the whole thing
			// FIXME: do we want to kill execution upon local events? that would be consistant so Steph votes yes, but
			// we may discuss further
			if(i && !(AsebaMaskIsSet(vmState.flags, ASEBA_VM_STEP_BY_STEP_MASK) &&  AsebaMaskIsSet(vmState.flags, ASEBA_VM_EVENT_ACTIVE_MASK))) {
			// that is the same as (thx de Morgan)
			//if(i && (AsebaMaskIsClear(vmState.flags, ASEBA_VM_STEP_BY_STEP_MASK) ||  AsebaMaskIsClear(vmState.flags, ASEBA_VM_EVENT_ACTIVE_MASK))) {
				i--;
				CLEAR_EVENT(i);
				vmVariables.source = vmState.nodeId;
				AsebaVMSetupEvent(&vmState, ASEBA_EVENT_LOCAL_EVENTS_START - i);
            }
        }
    }
    return 0;
}

void aseba_vm_init(void)
{
    vmState.nodeId = 4;

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
