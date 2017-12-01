#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "shell.h"
#include "usbcfg.h"
#include "cmd.h"
#include "memory_protection.h"
#include "main.h"
#include "config_flash_storage.h"

//#include "discovery_demo/accelerometer.h"
//#include "discovery_demo/leds.h"
#include "discovery_demo/button.h"

//#include "aseba_vm/aseba_node.h"
//#include "aseba_vm/skel_user.h"
//#include "aseba_vm/aseba_can_interface.h"
//#include "aseba_vm/aseba_bridge.h"

#include "camera/po8030.h"
#include "leds.h"
#include "utility.h"
#include "sensors/imu.h"
#include "sensors/mpu9250.h"
#include "i2c_bus.h"
#include "sensors/proximity.h"
#include "audio/audio_thread.h"
#include "audio/microphone.h"
#include "sensors/VL53L0X/VL53L0X.h"
#include "motors.h"
#include "sdcard.h"
#include <ff.h>
#include "diskio.h"
#include "epuck1x/Asercom.h"
#include "exti.h"
#include "spi_comm.h"
#include "sensors/battery_level.h"
#include "ir_remote.h"

#define SHELL_WA_SIZE   THD_WORKING_AREA_SIZE(2048)

messagebus_t bus;
MUTEX_DECL(bus_lock);
CONDVAR_DECL(bus_condvar);

parameter_namespace_t parameter_root, aseba_ns;

static THD_WORKING_AREA(selector_thd_wa, 2048);

uint8_t txComplete = 0;
uint8_t btnState = 0;

static bool load_config(void)
{
    extern uint32_t _config_start;

    return config_load(&parameter_root, &_config_start);
}

void my_button_cb(void) {

	txComplete = 1;

/*
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
*/	
}

FRESULT open_append(
	FIL* fp,            // [OUT] File object to create
	const char* path    // [IN]  File name to be opened
    )
{
	FRESULT fr;
	
	chprintf((BaseSequentialStream *)&SDU1, "opening file...\r\n");
	
	// Opens an existing file. If not exist, creates a new file.
	fr = f_open(fp, path, FA_WRITE | FA_OPEN_ALWAYS);
	if (fr == FR_OK) {
		// Seek to end of the file to append data
		fr = f_lseek(fp, f_size(fp));
		if (fr != FR_OK) {
			f_close(fp);
		}
	}
	return fr;
}

static THD_FUNCTION(selector_thd, arg)
{
    (void) arg;
    chRegSetThreadName(__FUNCTION__);

    systime_t time;

    messagebus_topic_t *imuTopic = messagebus_find_topic_blocking(&bus, "/imu");
    imu_msg_t imu;

    messagebus_topic_t *proxTopic = messagebus_find_topic_blocking(&bus, "/proximity");
    proximity_msg_t proximity;
	signed int leftSpeed=0, rightSpeed=0;

	messagebus_topic_t *batteryTopic = messagebus_find_topic_blocking(&bus, "/battery_level");
	battery_msg_t battery_value;

	uint8_t playing = 0;

	uint8_t tof_measuring = 0;

	uint8_t rab_addr = 0x20;
	uint8_t rab_state = 0;
	int8_t i2c_err = 0;
	uint8_t regValue[2] = {0};
	uint16_t rab_data = 0;
	double rab_bearing = 0.0;
	uint16_t rab_range = 0;
	uint16_t rab_sensor = 0;

	uint8_t sdState = 0;
	FRESULT fr;
	FIL fil;
//	FATFS fs;           // Filesystem object.
	//BYTE work[1024]; /* Work area (larger is better for processing time) */
//	int rc;
	//DWORD buff[512];  /* 2048 byte working buffer */

	uint8_t toEsp32 = 'c', fromEsp32 = 0;
	int16_t len = 0;

    while(1) {
    	time = chVTGetSystemTime();

		switch(get_selector()) {
			case 0: // Read IMU raw senosrs values.
		    	messagebus_topic_wait(imuTopic, &imu, sizeof(imu));
		    	if (SDU1.config->usbp->state != USB_ACTIVE) { // Skip printing if port not opened.
		    		continue;
		    	}
		    	chprintf((BaseSequentialStream *)&SDU1, "%Ax=%-7d Ay=%-7d Az=%-7d Gx=%-7d Gy=%-7d Gz=%-7d\r\n", imu.acc_raw[0], imu.acc_raw[1], imu.acc_raw[2], imu.gyro_raw[0], imu.gyro_raw[1], imu.gyro_raw[2]);
		    	chThdSleepUntilWindowed(time, time + MS2ST(100)); // Refresh @ 10 Hz.
				break;

			case 1: // Read acceleration and rotation speeed (rad/s).
		    	messagebus_topic_wait(imuTopic, &imu, sizeof(imu));
		    	if (SDU1.config->usbp->state != USB_ACTIVE) { // Skip printing if port not opened.
		    		continue;
		    	}
		    	chprintf((BaseSequentialStream *)&SDU1, "%Ax=%-7.3f Ay=%-7.3f Az=%-7.3f Gx=%-6.3f Gy=%-6.3f Gz=%-6.3f\r\n", imu.acceleration[0], imu.acceleration[1], imu.acceleration[2], imu.gyro[0], imu.gyro[1], imu.gyro[2]);
		    	chThdSleepUntilWindowed(time, time + MS2ST(100)); // Refresh @ 10 Hz.
				break;

			case 2: // Led toggling.
				set_led(0, 2);
				set_led(1, 2);
				set_led(2, 2);
				set_led(3, 2);
				set_body_led(2);
				set_front_led(2);
				chThdSleepUntilWindowed(time, time + MS2ST(500)); // Refresh @ 2 Hz.
				break;

			case 3: // Read proximity sensors.
				messagebus_topic_wait(proxTopic, &proximity, sizeof(proximity));

				if (SDU1.config->usbp->state != USB_ACTIVE) { // Skip printing if port not opened.
					continue;
				}

				// Sensors info print: each line contains data related to a single sensor.
		        for (int i = 0; i < PROXIMITY_NB_CHANNELS; i++) {
		        	chprintf((BaseSequentialStream *)&SDU1, "%.4d,", proximity.ambient[i]);
		        	chprintf((BaseSequentialStream *)&SDU1, "%.4d,", proximity.reflected[i]);
		        	chprintf((BaseSequentialStream *)&SDU1, "%.4d", proximity.delta[i]);
		        	chprintf((BaseSequentialStream *)&SDU1, "\r\n");
		        }
		        chprintf((BaseSequentialStream *)&SDU1, "\r\n");

				// CSV print: each line contains IR0amb, IR0ref, IR0delta, IR1amb, ..., IR7delta.
//		        for (int i = 0; i < PROXIMITY_NB_CHANNELS; i++) {
//		        	chprintf((BaseSequentialStream *)&SDU1, "%.4d;%.4d;%.4d;", proximity.ambient[i], proximity.reflected[i], proximity.delta[i]);
//		        }
//		        chprintf((BaseSequentialStream *)&SDU1, "\r\n");

//		        // CSV print: each line contains only delta values for each sensor.
//		        for (int i = 0; i < PROXIMITY_NB_CHANNELS; i++) {
//		        	chprintf((BaseSequentialStream *)&SDU1, "%d;", proximity.delta[i]);
//		        }
//		        chprintf((BaseSequentialStream *)&SDU1, "\r\n");
		        chThdSleepUntilWindowed(time, time + MS2ST(100)); // Refresh @ 10 Hz.
				break;

			case 4:
				if(playing == 0) {
					//audio_start(200);
					dac_play(200);
					playing = 1;
				}
				chThdSleepUntilWindowed(time, time + MS2ST(100)); // Refresh @ 10 Hz.
				break;

			case 5:
				if(playing == 0) {
					//audio_start(2000);
					dac_play(2000);
					playing = 1;
				}
				chThdSleepUntilWindowed(time, time + MS2ST(100)); // Refresh @ 10 Hz.
				break;

			case 6:
				if(playing == 0) {
					//audio_start(10000);
					dac_play(10000);
					playing = 1;
				}
				chThdSleepUntilWindowed(time, time + MS2ST(100)); // Refresh @ 10 Hz.
				break;

			case 7:
				if(tof_measuring == 0) {
					tof_measuring = 1;
					VL53L0X_init_demo();
				}
				chThdSleepUntilWindowed(time, time + MS2ST(100)); // Refresh @ 10 Hz.
				break;

			case 8:
				right_motor_set_speed(2200);
				left_motor_set_speed(-2200);
				chThdSleepUntilWindowed(time, time + MS2ST(100)); // Refresh @ 10 Hz.
				break;

			case 9:
				//chprintf((BaseSequentialStream *)&SDU1, "sdState=%d\r\n", sdState);
				switch(sdState) {
					case 0:
						palSetPadMode(GPIOC, GPIOC_MIC_SPI3_SCK, PAL_STM32_MODE_ALTERNATE | PAL_STM32_OTYPE_PUSHPULL | PAL_STM32_PUDR_PULLUP | PAL_STM32_OSPEED_HIGHEST | PAL_STM32_ALTERNATE(12));
						sdcard_start();
						sdState = 1;
						break;

					case 1:
						sdcard_automount();
						sdState = 2;
						break;

					case 2:
						/*
						chprintf((BaseSequentialStream *)&SDU1, "formatting the micro sd...\r\n");
						fr = f_mkfs("", 0, 0);
						if (fr != FR_OK) {
							chprintf((BaseSequentialStream *)&SDU1, "f_mkfs err=%d\r\n", fr);
							sdState = 3;
						}
						chprintf((BaseSequentialStream *)&SDU1, "operation completed\r\n");
						*/

						/*
						chprintf((BaseSequentialStream *)&SDU1, "opening file...\r\n");
						fr = f_open(&fil, "logfile.txt", FA_READ);
						if (fr != FR_OK) {
							chprintf((BaseSequentialStream *)&SDU1, "open_append err=%d\r\n", fr);
							sdState = 3;
						}
						chprintf((BaseSequentialStream *)&SDU1, "operation completed\r\n");
						*/

						chprintf((BaseSequentialStream *)&SDU1, "opening file for writing...\r\n");
						fr = open_append(&fil, "logfile.txt");
						if (fr != FR_OK) {
							chprintf((BaseSequentialStream *)&SDU1, "open_append err=%d\r\n", fr);
							sdState = 3;
						}
						chprintf((BaseSequentialStream *)&SDU1, "operation completed\r\n");

						/*
						// Append a line
						f_printf(&fil, "%02u/%02u/%u, %2u:%02u\n", 31, 8, 2017, 9, 32);

						// Close the file
						f_close(&fil);
						*/
						sdState = 3;
						break;

					case 3:
						/*
						sdcard_automount();
						*/
						break;
				}
				chThdSleepUntilWindowed(time, time + MS2ST(100)); // Refresh @ 10 Hz.
				break;

			case 10:
				/*
				// Check function/compatibility of the physical drive #0
				rc = test_diskio(0, 1, buff, sizeof buff);
				if (rc) {
					chprintf((BaseSequentialStream *)&SDU1, "Sorry the function/compatibility test failed.\nFatFs will not work on this disk driver.\n");
				} else {
					chprintf((BaseSequentialStream *)&SDU1, "Congratulations! The disk I/O layer works well.\n");
				}
				*/
				break;

			case 11:
				messagebus_topic_wait(proxTopic, &proximity, sizeof(proximity));
				leftSpeed = 2000 - proximity.delta[0]*4 - proximity.delta[1]*2;
				rightSpeed = 2000 - proximity.delta[7]*4 - proximity.delta[6]*2;
				right_motor_set_speed(rightSpeed);
				left_motor_set_speed(leftSpeed);
				chThdSleepUntilWindowed(time, time + MS2ST(10)); // Refresh @ 100 Hz.
				break;

			case 12:
				run_asercom();
				chThdSleepUntilWindowed(time, time + MS2ST(10)); // Refresh @ 100 Hz.
				break;

			case 13:
			    /* Configure PO8030 camera. */
			//    if(po8030_config(FORMAT_YCBYCR, SIZE_QQVGA) != MSG_OK) { // Default configuration.
			//        dcmiErrorFlag = 1;
			//    }
				/*
				capture_mode = CAPTURE_ONE_SHOT;
				double_buffering = 0;
				po8030_save_current_format(FORMAT_YYYY);
				po8030_save_current_subsampling(SUBSAMPLING_X1, SUBSAMPLING_X1);
				po8030_advanced_config(FORMAT_YYYY, 1, 1, 320, 240, SUBSAMPLING_X1, SUBSAMPLING_X1);
				sample_buffer = (uint8_t*)malloc(po8030_get_image_size());
				dcmiPrepare(&DCMID, &dcmicfg, po8030_get_image_size(), (uint32_t*)sample_buffer, NULL);
				//dcmiStartOneShot(&DCMID);
				*/

				messagebus_topic_wait(batteryTopic, &battery_value, sizeof(battery_value));
				//chprintf((BaseSequentialStream *)&SDU1, "battery raw = %d\r\n", get_battery_raw());
				chprintf((BaseSequentialStream *)&SDU1, "battery raw = %d\r\n", battery_value.raw_value);
				chThdSleepUntilWindowed(time, time + MS2ST(2000)); // Refresh @ 0.5 Hz.
				break;

			case 14: // ESP32 UART communication test.
				sdPut(&SD3, toEsp32);
				len = sdReadTimeout(&SD3, &fromEsp32, 1, MS2ST(50));
				if(len > 0) {
					sdPut(&SDU1, fromEsp32);
				}
				chThdSleepUntilWindowed(time, time + MS2ST(10)); // Refresh @ 100 Hz.
				break;

			case 15: // Range and bearing testing.
				switch(rab_state) {
					case 0:
						write_reg(rab_addr, 12, 150); // Set range.
						if((i2c_err = read_reg(rab_addr, 12, &regValue[0])) == MSG_OK) {
						   	chprintf((BaseSequentialStream *)&SDU1, "set range to %d\r\n", regValue[0]);
						}
						write_reg(rab_addr, 17, 1); // Onboard calculation.
						if((i2c_err = read_reg(rab_addr, 12, &regValue[0])) == MSG_OK) {
							chprintf((BaseSequentialStream *)&SDU1, "onboard calculation enabled = %d\r\n", regValue[0]);
						}
						write_reg(rab_addr, 16, 0); // Store light conditions.
						rab_state = 1;
						break;

					case 1:
					    if((i2c_err = read_reg(rab_addr, 0, &regValue[0])) != MSG_OK) {
					    	chprintf((BaseSequentialStream *)&SDU1, "err\r\n");
					        break;
					    }
					    chprintf((BaseSequentialStream *)&SDU1, "recv = %d\r\n", regValue[0]);
					    if(regValue[0] != 0) {
					    	read_reg(rab_addr, 1, &regValue[0]);
							read_reg(rab_addr, 2, &regValue[1]);
							rab_data = (((uint16_t)regValue[0])<<8) + regValue[1];

					    	read_reg(rab_addr, 3, &regValue[0]);
							read_reg(rab_addr, 4, &regValue[1]);
							rab_bearing = ((double)((((uint16_t)regValue[0])<<8) + regValue[1])) * 0.0001;

					    	read_reg(rab_addr, 5, &regValue[0]);
							read_reg(rab_addr, 6, &regValue[1]);
							rab_range = (((uint16_t)regValue[0])<<8) + regValue[1];

							read_reg(rab_addr, 9, &regValue[0]);
							rab_sensor = regValue[0];

							if (SDU1.config->usbp->state != USB_ACTIVE) { // Skip printing if port not opened.
								break;
							}

							chprintf((BaseSequentialStream *)&SDU1, "%d %f %d %d\r\n", rab_data, (rab_bearing*180/M_PI), rab_range, rab_sensor);
					    }

					    write_reg(rab_addr, 13, 0x00);
					    write_reg(rab_addr, 14, 0xFF);
						break;
				}
				chThdSleepUntilWindowed(time, time + MS2ST(100)); // Refresh @ 10 Hz.
				break;
		}
    }
}

int main(void)
{

    halInit();
    chSysInit();
    mpu_init();

    /** Inits the Inter Process Communication bus. */
    messagebus_init(&bus, &bus_lock, &bus_condvar);

    parameter_namespace_declare(&parameter_root, NULL, NULL);

	clear_leds();
	set_body_led(0);
	set_front_led(0);
	static SerialConfig ser_cfg = {
	    2500000,
	    0,
	    0,
	    0,
	};
	sdStart(&SD3, &ser_cfg); // UART3.
	usb_start();
	i2c_start();
	dcmi_start();
	po8030_start();
	imu_start();
	proximity_start();
	dac_start();
	motors_init();
	exti_start();
	ir_remote_start();
	spi_comm_start();
	mic_start();
	battery_level_start();
	
	//i2cStop(&I2CD1);

    // Initialise Aseba system, declaring parameters
    //parameter_namespace_declare(&aseba_ns, &parameter_root, "aseba");
    //aseba_declare_parameters(&aseba_ns);

    /* Load parameter tree from flash. */
    load_config();

    /* Start AsebaCAN. Must be after config was loaded because the CAN id
     * cannot be changed at runtime. */
    //aseba_vm_init();
    //aseba_can_start(&vmState);

	// Needed?? No button available directly on the STM32F407, "user button" connected to ESP32...
	// PA0 is free anyway...
    /* If button is pressed, start in translator mode. */
//    if (palReadPad(GPIOA, GPIOA_BUTTON)) {
//        //aseba_bridge((BaseSequentialStream *)&SDU1);
//        while (true) {
//            chThdSleepMilliseconds(100);
//        }
//    } else {
        // Initialise Discovery board demo setup
        //demo_led_init();
        //aseba_vm_start();
//    }

    //demo_acc_start(accelerometer_cb);
    //demo_button_start(my_button_cb);

    /* Start shell on the USB port. */
   // shell_start();

    chThdSleepMilliseconds(5000);

    chThdCreateStatic(selector_thd_wa, sizeof(selector_thd_wa), NORMALPRIO, selector_thd, NULL);

    /* Infinite loop. */
    while (1) {
        chThdSleepMilliseconds(100);
		
		//set_body_led(2);
		//set_front_led(2);
		
        if(txComplete == 1) {
            txComplete = 0;
			
			chThdSleepMilliseconds(5000);

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
