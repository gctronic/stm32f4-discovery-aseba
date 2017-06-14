#include <stdio.h>
#include <stdlib.h>
#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "shell.h"
#include "usbcfg.h"
#include "cmd.h"
#include "memory_protection.h"
#include "main.h"
#include "config_flash_storage.h"

#include "discovery_demo/accelerometer.h"
#include "discovery_demo/leds.h"
#include "discovery_demo/button.h"

#include "aseba_vm/aseba_node.h"
#include "aseba_vm/skel_user.h"
#include "aseba_vm/aseba_can_interface.h"
#include "aseba_vm/aseba_bridge.h"

#include "VL53L0X/VL53L0X.h"
#include "LED_RGB/led_rgb.h"

#define SHELL_WA_SIZE   THD_WORKING_AREA_SIZE(2048)

parameter_namespace_t parameter_root, aseba_ns;

static bool load_config(void)
{
    extern uint32_t _config_start;

    return config_load(&parameter_root, &_config_start);
}

/* I2C interface #1 */
static const I2CConfig i2cfg1 = {
    OPMODE_I2C,
    400000,
    FAST_DUTY_CYCLE_2,
};

int main(void)
{
    halInit();
    chSysInit();
    mpu_init();

    parameter_namespace_declare(&parameter_root, NULL, NULL);


    // UART2 on PA2(TX) and PA3(RX)
    sdStart(&SD2, NULL);
    palSetPadMode(GPIOA, 2, PAL_MODE_ALTERNATE(7));
    palSetPadMode(GPIOA, 3, PAL_MODE_ALTERNATE(7));

    // serial-over-USB CDC driver.
    sduObjectInit(&SDU1);
    sduStart(&SDU1, &serusbcfg);
    usbDisconnectBus(serusbcfg.usbp);
    chThdSleepMilliseconds(1000);
    usbStart(serusbcfg.usbp, &usbcfg);
    usbConnectBus(serusbcfg.usbp);

    //I2C1 Init
    palSetPadMode(GPIOB, 8, PAL_MODE_ALTERNATE(4) | PAL_STM32_OTYPE_OPENDRAIN);
    palSetPadMode(GPIOB, 9, PAL_MODE_ALTERNATE(4) | PAL_STM32_OTYPE_OPENDRAIN);
    i2cStart(&I2CD1, &i2cfg1);

    // Initialise Aseba system, declaring parameters
    //parameter_namespace_declare(&aseba_ns, &parameter_root, "aseba");
    //aseba_declare_parameters(&aseba_ns);

    /* Load parameter tree from flash. */
    //load_config();

    /* Start AsebaCAN. Must be after config was loaded because the CAN id
     * cannot be changed at runtime. */
    //aseba_vm_init();
    //aseba_can_start(&vmState);

    /* If button is pressed, start in translator mode. */
    if (palReadPad(GPIOA, GPIOA_BUTTON)) {
        //aseba_bridge((BaseSequentialStream *)&SDU1);
        led_rgb_init_demo();
        while (true) {
            chThdSleepMilliseconds(100);
        }
    } else {
        // Initialise Discovery board demo setup
        //demo_led_init();
        //aseba_vm_start();
        VL53L0X_init_demo();
    }

    //demo_acc_start(accelerometer_cb);e
    //demo_button_start(button_cb);

    /* Start shell on the USB port. */
    shell_start();

    /* Infinite loop, do nothing. */
    while (1) {
        chThdSleepMilliseconds(1000);
    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}
