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

    AsebaVMSetupEvent(&vmState, ASEBA_EVENT_INIT);

    while (TRUE) {
        palTogglePad(GPIOD, GPIOD_LED6);
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

    palSetPad(GPIOD, GPIOD_LED5);
    chThdSleepMilliseconds(200);
    palClearPad(GPIOD, GPIOD_LED5);
    chThdSleepMilliseconds(200);
    palSetPad(GPIOD, GPIOD_LED5);
    chThdSleepMilliseconds(200);
    palClearPad(GPIOD, GPIOD_LED5);

    AsebaVMSetupEvent(&vmState, ASEBA_EVENT_INIT);
}

void aseba_vm_start(void)
{
    chThdCreateStatic(aseba_vm_thd_wa, sizeof(aseba_vm_thd_wa), LOWPRIO, aseba_vm_thd, NULL);
}

// This function must update the variable to match the microcontroller state
// It is called _BEFORE_ running the VM, so it's a {Microcontroller state} -> {Aseba Variable}
// synchronisation
void update_aseba_variables_read(void)
{

}

// This function must update the microcontrolleur state to match the variables
// It is called _AFTER_ running the VM, so it's a {Aseba Variables} -> {Microcontroller state}
// synchronisation
void update_aseba_variables_write(void)
{

}
