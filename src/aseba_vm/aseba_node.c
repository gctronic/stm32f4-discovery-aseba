#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "usbcfg.h"

#include "common/consts.h"
#include "common/productids.h"
#include "vm/vm.h"
#include "aseba_vm/skel.h"
#include "aseba_vm/skel_user.h"
#include "aseba_vm/aseba_node.h"

#include "discovery_demo/accelerometer.h"


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
        chThdSleepMilliseconds(10);

        // Sync Aseba with the state of the Microcontroller
        update_aseba_variables_read();

        // Run VM for some time
        AsebaVMRun(&vmState, 1000);
        AsebaProcessIncomingEvents(&vmState);

        // Sync the Microcontroller with the state of Aseba
        update_aseba_variables_write();
    }
    return 0;
}

void aseba_vm_init(void)
{
    vmState.nodeId = 3;

    AsebaVMInit(&vmState);
    vmVariables.id = vmState.nodeId;
    vmVariables.productId = ASEBA_PID_UNDEFINED;
    vmVariables.fwversion[0] = 0;
    vmVariables.fwversion[1] = 1;

    vmVariables.led = 0;
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

// This function must update the variable to match the microcontroller state
void update_aseba_variables_read(void)
{
    static float accf[3];
    demo_acc_get_acc(accf);
    vmVariables.acc[0] = (sint16) accf[0];
    vmVariables.acc[1] = (sint16) accf[1];
    vmVariables.acc[2] = (sint16) accf[2];
}

// This function must update the microcontrolleur state to match the variables
void update_aseba_variables_write(void)
{
    if (vmVariables.led == 0) {
        palClearPad(GPIOD, GPIOD_LED4);
    } else {
        palSetPad(GPIOD, GPIOD_LED4);
    }
}
