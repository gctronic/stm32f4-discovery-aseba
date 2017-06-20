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

//#include "aseba_vm/aseba_node.h"
//#include "aseba_vm/skel_user.h"
//#include "aseba_vm/aseba_can_interface.h"
//#include "aseba_vm/aseba_bridge.h"

#include "camera/po8030.h"

#define SHELL_WA_SIZE   THD_WORKING_AREA_SIZE(2048)

parameter_namespace_t parameter_root, aseba_ns;

uint8_t capture_mode = CAPTURE_ONE_SHOT;
//uint8_t capture_mode =  CAPTURE_CONTINUOUS;
uint8_t *sample_buffer = NULL;
uint8_t *sample_buffer2 = NULL;
uint8_t double_buffering = 0;

unsigned char txComplete = 0;
unsigned char btnState = 0;
unsigned char dcmiErrorFlag = 0;

void frameEndCb(DCMIDriver* dcmip);
void dmaTransferEndCb(DCMIDriver* dcmip);
void dcmiErrorCb(DCMIDriver* dcmip, dcmierror_t err);
const DCMIConfig dcmicfg = {
    frameEndCb,
    dmaTransferEndCb,
	dcmiErrorCb,
    DCMI_CR_PCKPOL
};

static bool load_config(void)
{
    extern uint32_t _config_start;

    return config_load(&parameter_root, &_config_start);
}

void my_button_cb(void) {
    if(btnState == 0) {
        btnState = 1;
        palSetPad(GPIOD, 15); // Blue.
        palSetPad(GPIOD, 13) ; // Orange.
        if(capture_mode == CAPTURE_ONE_SHOT) {
            dcmiStartOneShot(&DCMID);
        } else {
            dcmiStartStream(&DCMID);
        }
    } else if(btnState == 1) {
        btnState = 0;
        if(capture_mode == CAPTURE_ONE_SHOT) {
            txComplete = 1;
        } else {
            if(dcmiStopStream(&DCMID) == MSG_OK) {
                txComplete = 1;
            } else {
                dcmiErrorFlag = 1;
            }
        }
    }
}

void frameEndCb(DCMIDriver* dcmip) {
    (void) dcmip;
    palTogglePad(GPIOD, 13) ; // Orange.
    txComplete = 1;
}

void dmaTransferEndCb(DCMIDriver* dcmip) {
   (void) dcmip;
    palTogglePad(GPIOD, 15); // Blue.
}

void dcmiErrorCb(DCMIDriver* dcmip, dcmierror_t err) {
   (void) dcmip;
   (void) err;
    dcmiErrorFlag = 1;
	//chSysHalt("DCMI error");
}

int main(void)
{

    halInit();
    chSysInit();
    mpu_init();

    parameter_namespace_declare(&parameter_root, NULL, NULL);


    // UART2 on PA2(TX) and PA3(RX)
    //sdStart(&SD2, NULL);
    //palSetPadMode(GPIOA, 2, PAL_MODE_ALTERNATE(7));
    //palSetPadMode(GPIOA, 3, PAL_MODE_ALTERNATE(7));

    // serial-over-USB CDC driver.
    sduObjectInit(&SDU1);
    sduStart(&SDU1, &serusbcfg);
    usbDisconnectBus(serusbcfg.usbp);
    chThdSleepMilliseconds(1000);
    usbStart(serusbcfg.usbp, &usbcfg);
    usbConnectBus(serusbcfg.usbp);


    // Initialise Aseba system, declaring parameters
    //parameter_namespace_declare(&aseba_ns, &parameter_root, "aseba");
    //aseba_declare_parameters(&aseba_ns);

    /* Load parameter tree from flash. */
    load_config();

    /* Start AsebaCAN. Must be after config was loaded because the CAN id
     * cannot be changed at runtime. */
    //aseba_vm_init();
    //aseba_can_start(&vmState);

    /* If button is pressed, start in translator mode. */
    if (palReadPad(GPIOA, GPIOA_BUTTON)) {
        //aseba_bridge((BaseSequentialStream *)&SDU1);
        while (true) {
            chThdSleepMilliseconds(100);
        }
    } else {
        // Initialise Discovery board demo setup
        //demo_led_init();
        //aseba_vm_start();
    }

    //demo_acc_start(accelerometer_cb);
    demo_button_start(my_button_cb);

    /* Start shell on the USB port. */
    shell_start();

    /* Configure PO8030 camera. */
    po8030_init();
    if(po8030_config(FORMAT_YCBYCR, SIZE_QQVGA) != MSG_OK) { // Default configuration.
        dcmiErrorFlag = 1;
    }

    po8030_save_current_format(FORMAT_YYYY);
    po8030_save_current_subsampling(SUBSAMPLING_X1, SUBSAMPLING_X1);
    po8030_advanced_config(FORMAT_YYYY, 1, 1, 320, 240, SUBSAMPLING_X1, SUBSAMPLING_X1);
    uint32_t image_size = po8030_get_image_size();
    sample_buffer = (uint8_t*)malloc(image_size);
    dcmiPrepare(&DCMID, &dcmicfg, image_size, (uint32_t*)sample_buffer, NULL);

    /* Infinite loop. */
    while (1) {
        chThdSleepMilliseconds(500);

        // Led toggled to verify main is running and to show DCMI state.
        if(dcmiErrorFlag == 1) {
            palClearPad(GPIOD, 12); // Green.
            palTogglePad(GPIOD, 14); // Red.
        } else {
            palClearPad(GPIOD, 14); // Red.
            palTogglePad(GPIOD, 12); // Green.
        }

        //chprintf((BaseSequentialStream *)&SDU1, "%d\r\n", dmaStreamGetTransactionSize(DCMID.dmastp));

        if(txComplete == 1) {
            txComplete = 0;

            palClearPad(GPIOD, 15); // Blue.
            palClearPad(GPIOD, 13) ; // Orange.

            if(capture_mode == CAPTURE_ONE_SHOT) {
                chnWrite((BaseSequentialStream *)&SDU1, sample_buffer, po8030_get_image_size());
            } else {
                if(double_buffering == 1) { // Send both images.
                    chnWrite((BaseSequentialStream *)&SDU1, sample_buffer, po8030_get_image_size());
                    chThdSleepMilliseconds(3000);
                    chnWrite((BaseSequentialStream *)&SDU1, sample_buffer2, po8030_get_image_size());
                } else {
                    chnWrite((BaseSequentialStream *)&SDU1, sample_buffer, po8030_get_image_size());
                }
            }
        }
    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}