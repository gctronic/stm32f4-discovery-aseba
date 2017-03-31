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

#define MODE_ONE_SHOT
//#define IMAGE_SIZE_QQVGA
//#define FORMAT_COLOR

#if !defined IMAGE_SIZE_QQVGA && defined FORMAT_COLOR
    #error Cannot grab QVGA in color format
#endif

#if !defined IMAGE_SIZE_QQVGA && !defined MODE_ONE_SHOT
    #warning QVGA streaming allowed only in single-buffer mode
#endif

#ifdef IMAGE_SIZE_QQVGA
    #define IMAGE_WIDTH 160
    #define IMAGE_HEIGHT 120
#else
    #define IMAGE_WIDTH 320
    #define IMAGE_HEIGHT 240
#endif
#ifdef FORMAT_COLOR
    #define IMAGE_SIZE IMAGE_WIDTH*IMAGE_HEIGHT*2 // Size in bytes.
#else
    #define IMAGE_SIZE IMAGE_WIDTH*IMAGE_HEIGHT // Size in bytes.
#endif

unsigned char sample_buffer[IMAGE_SIZE] = {0};
#ifdef IMAGE_SIZE_QQVGA
unsigned char sample_buffer2[IMAGE_SIZE] = {0};
#endif
unsigned char txComplete = 0;
unsigned char btnState = 0;
unsigned char dcmiErrorFlag = 0;

void frameEndCb(DCMIDriver* dcmip);
void dmaTransferEndCb(DCMIDriver* dcmip);
void dcmiErrorCb(DCMIDriver* dcmip, dcmierror_t err);
static const DCMIConfig dcmicfg = {
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
#ifdef MODE_ONE_SHOT
        dcmiStartOneShot(&DCMID);
#else
        dcmiStartStream(&DCMID);
#endif
    } else if(btnState == 1) {
        btnState = 0;
#ifdef MODE_ONE_SHOT
        txComplete = 1;
#else
        if(dcmiStopStream(&DCMID) == MSG_OK) {
            txComplete = 1;
        } else {
            dcmiErrorFlag = 1;
        }
#endif
    }
}

void frameEndCb(DCMIDriver* dcmip) {
    (void) dcmip;
    palTogglePad(GPIOD, 13) ; // Orange.
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
    //shell_start();

    /* Configure PO8030 camera. */
    po8030_init();
#ifdef IMAGE_SIZE_QQVGA
#ifdef FORMAT_COLOR
	po8030_config(FORMAT_YCBYCR, SIZE_QQVGA);
#else
	po8030_config(FORMAT_YYYY, SIZE_QQVGA);
#endif // FORMAT_COLOR
#else
#ifdef FORMAT_COLOR
	// Not valid, do not fit in RAM.
#else
	po8030_config(FORMAT_YYYY, SIZE_QVGA);
#endif // FORMAT_COLOR
#endif // IMAGE_SIZE_QQVGA

    /* Configure the digital camera interface (DCMI). */
    iomode_t mode = PAL_STM32_MODE_ALTERNATE | PAL_STM32_OTYPE_PUSHPULL
                    | PAL_STM32_OSPEED_HIGHEST | PAL_STM32_PUDR_PULLUP
                    | PAL_STM32_ALTERNATE(13);
    palSetPadMode(GPIOA, 4, mode); // Hsync.
    palSetPadMode(GPIOA, 6, mode); // Pclk.
    palSetPadMode(GPIOB, 6, mode); // D5.
    palSetPadMode(GPIOB, 7, mode); // Vsync.
    palSetPadMode(GPIOC, 6, mode); // D0.
    palSetPadMode(GPIOC, 7, mode); // D1.
    palSetPadMode(GPIOC, 8, mode); // D2.
    palSetPadMode(GPIOC, 9, mode); // D3.
    palSetPadMode(GPIOC, 11, mode); // D4.
    palSetPadMode(GPIOE, 5, mode); // D6.
    palSetPadMode(GPIOE, 6, mode); // D7.

#ifdef MODE_ONE_SHOT
    dcmiPrepare(&DCMID, &dcmicfg, IMAGE_SIZE, (uint32_t*)sample_buffer, NULL);
#else
#ifdef IMAGE_SIZE_QQVGA
    dcmiPrepare(&DCMID, &dcmicfg, IMAGE_SIZE, (uint32_t*)sample_buffer, (uint32_t*)sample_buffer2);
#else
    dcmiPrepare(&DCMID, &dcmicfg, IMAGE_SIZE, (uint32_t*)sample_buffer, NULL);
#endif // IMAGE_SIZE_QQVGA
#endif

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

#ifdef MODE_ONE_SHOT
            //int bytesTransferred = 0;
            //while(bytesTransferred < IMAGE_SIZE) {
            //    bytesTransferred += chnWriteTimeout((BaseChannel *)&SDU1, &sample_buffer[bytesTransferred], IMAGE_SIZE-bytesTransferred, 100);
            //}
            chnWrite((BaseSequentialStream *)&SDU1, sample_buffer, IMAGE_SIZE);
#else
            chnWrite((BaseSequentialStream *)&SDU1, sample_buffer, IMAGE_SIZE);
#ifdef IMAGE_SIZE_QQVGA
            chThdSleepMilliseconds(3000);
            chnWrite((BaseSequentialStream *)&SDU1, sample_buffer2, IMAGE_SIZE);
#endif
#endif
        }
    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}
