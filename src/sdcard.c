#include <ch.h>
#include <hal.h>
#include <stdbool.h>
#include <ff.h>
#include "sdcard.h"
#include "leds.h"
#include "usbcfg.h"
#include "chprintf.h"

bool sdcard_mounted = false;
static FATFS SDC_FS;

void sdcard_start(void)
{
    static const SDCConfig sdc_config = {
        NULL, /* not needed for SD cards */
        SDC_MODE_4BIT
    };
    sdcStart(&SDCD1, &sdc_config);
}

void sdcard_mount(void)
{
    FRESULT err;
    sdcard_mounted = false;

//	chprintf((BaseSequentialStream *)&SDU1, "mounting card...\r\n");
	
    if (sdcConnect(&SDCD1) == HAL_FAILED) {
//		chprintf((BaseSequentialStream *)&SDU1, "connection failed...\r\n");
        return;
    }
	
//	chprintf((BaseSequentialStream *)&SDU1, "connected to card...\r\n");

    err = f_mount(&SDC_FS, "", 0);
//	chprintf((BaseSequentialStream *)&SDU1, "mount err=%d\r\n", err);
    if (err != FR_OK) {
        sdcDisconnect(&SDCD1);
        return;
    } else {
        sdcard_mounted = true;
    }
}

void sdcard_unmount(void)
{
    f_mount(NULL, "", 0); /* fatfs unmount */
    sdcDisconnect(&SDCD1);
    sdcard_mounted = false;
}

void sdcard_automount(void)
{
	//chprintf((BaseSequentialStream *)&SDU1, "automount\r\n");
    if (sdcIsCardInserted(&SDCD1)) {
//		chprintf((BaseSequentialStream *)&SDU1, "card inserted\r\n");
        if (!sdcard_mounted) {
            sdcard_mount();
        }
    } else {
//		chprintf((BaseSequentialStream *)&SDU1, "card not inserted\r\n");
        if (sdcard_mounted) {
            sdcard_unmount();
        }
    }
}

void sdcard_activity(void)
{
	e_set_body_led(2);
}
