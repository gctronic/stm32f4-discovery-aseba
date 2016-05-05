#include <ch.h>
#include <hal.h>
#include "button.h"
#include "aseba_vm/skel_user.h"
#include "aseba_vm/aseba_node.h"

static void wait_for_state(bool state)
{
    while (palReadPad(GPIOA, GPIOA_BUTTON) != state) {
        chThdSleepMilliseconds(10);
    }
}

static THD_FUNCTION(button_thd, p)
{
    (void) p;

    chRegSetThreadName("aseba-button");

    while (true) {
        /* Wait for a button press. */
        wait_for_state(true);

        /* Debounce button. */
        chThdSleepMilliseconds(10);

        /* Wait for button release. */
        wait_for_state(false);

        /* Debounce button. */
        chThdSleepMilliseconds(10);

        /* Signal the event to the Aseba virtual machine. */
        SET_EVENT(EVENT_BUTTON);
    }
}

void demo_button_start(void)
{
    static THD_WORKING_AREA(wa, 1024);
    chThdCreateStatic(wa, sizeof(wa), NORMALPRIO, button_thd, NULL);
}
