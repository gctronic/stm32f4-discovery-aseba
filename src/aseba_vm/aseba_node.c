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

    while (TRUE) {
        palTogglePad(GPIOD, GPIOD_LED6);
        chThdSleepMilliseconds(100);

        update_robot_variables();

        AsebaProcessIncomingEvents(&vmState);
        AsebaVMRun(&vmState, 1000);
    }
    return 0;
}

void aseba_vm_init(void)
{
    vmState.nodeId = 3;

    AsebaVMInit(&vmState);
    vmVariables.id = vmState.nodeId;
    vmVariables.productId = ASEBA_PID_EPUCK;
    vmVariables.fwversion[0] = 0;
    vmVariables.fwversion[1] = 1;

    AsebaVMSetupEvent(&vmState, ASEBA_EVENT_INIT);

    AsebaVMRun(&vmState, 10);

    palSetPad(GPIOD, GPIOD_LED5);
    chThdSleepMilliseconds(200);
    palClearPad(GPIOD, GPIOD_LED5);
    chThdSleepMilliseconds(200);
    palSetPad(GPIOD, GPIOD_LED5);
    chThdSleepMilliseconds(200);
    palClearPad(GPIOD, GPIOD_LED5);
}

void aseba_vm_start(void)
{
    chThdCreateStatic(aseba_vm_thd_wa, sizeof(aseba_vm_thd_wa), NORMALPRIO, aseba_vm_thd, NULL);
}

void update_robot_variables(void)
{

}
