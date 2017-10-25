#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
#include "sensors/VL53L0X/VL53L0X.h"
#include "motor.h"
#include "sdcard.h"
#include <ff.h>
#include "diskio.h"
#include "Asercom.h"

#define SHELL_WA_SIZE   THD_WORKING_AREA_SIZE(2048)

#define SPI_COMMAND_SIZE 64
#define SPI_DATA_HEADER_SIZE 4
#define SPI_DATA_PAYLOAD_SIZE 4092
#define SPI_DELAY 1200

messagebus_t bus;
MUTEX_DECL(bus_lock);
CONDVAR_DECL(bus_condvar);

parameter_namespace_t parameter_root, aseba_ns;

uint8_t capture_mode = CAPTURE_ONE_SHOT;
uint8_t *sample_buffer = NULL;
uint8_t *sample_buffer2 = NULL;
uint8_t double_buffering = 0;

uint8_t txComplete = 0;
uint8_t btnState = 0;
uint8_t dcmiErrorFlag = 0;

uint8_t spiRxBuff[SPI_COMMAND_SIZE];
uint8_t spiTxBuff[SPI_COMMAND_SIZE];
uint8_t spiHeader[SPI_DATA_HEADER_SIZE];

event_source_t ss_event;

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

void frameEndCb(DCMIDriver* dcmip) {
    (void) dcmip;
    //palTogglePad(GPIOD, 13) ; // Orange.
}

void dmaTransferEndCb(DCMIDriver* dcmip) {
   (void) dcmip;
    //palTogglePad(GPIOD, 15); // Blue.
	//osalEventBroadcastFlagsI(&ss_event, 0);
}

void dcmiErrorCb(DCMIDriver* dcmip, dcmierror_t err) {
   (void) dcmip;
   (void) err;
    dcmiErrorFlag = 1;
	//chSysHalt("DCMI error");
}

/*
 * SPI image exchanger thread.
 */
static THD_WORKING_AREA(spi_thread_wa, 1024);
static THD_FUNCTION(spi_thread, p) {
	(void)p;
	chRegSetThreadName("SPI thread");
	uint32_t i = 0;
	//uint16_t transCount = 0; // image size / SPI_BUFF_LEN
	uint8_t id = 0;
	uint16_t checksum = 0;
	volatile uint32_t delay = 0;	
	uint16_t packetId = 0;
	uint16_t numPackets = 0;
	uint32_t remainingBytes = 0;
	uint32_t spiDataIndex = 0;	
	event_listener_t ss_listener;
	//eventmask_t evt;	
	
	// Create a fixed command packet content for debugging.
	// The first two bytes are fixed to 0xAA, 0xBB, this is for synchronization purposes with the ESP32.
	// The last byte represents the checksum (block check character), this is also used for synchronization purposes.
	// The command packet content will be ([index]value): [0]0xAA, [1]0xBB, [2]3, [3]4, [4]5, ..., [61]62, [62]63, [63]checksum.
	spiTxBuff[0] = 0xAA;
	checksum += spiTxBuff[0];
	spiTxBuff[1] = 0xBB;
	checksum += spiTxBuff[1];
	for(i=2; i<SPI_COMMAND_SIZE-1; i++) {
		spiTxBuff[i] = i+1;
		checksum += spiTxBuff[i];
	}
	spiTxBuff[SPI_COMMAND_SIZE-1] = checksum&0xFF; // Block check character checksum.

	// Create a fixed data packet content for debugging.
	// The data packet content will be: 0, 1, ..., 254, 255, 0, 1, ..., 254, 255, 0, 1, ...
	sample_buffer = malloc(76800);
	id = 0;
	for(i=0; i<76800; i++) {
		sample_buffer[i] = id;
		if(id == 255) {
			id = 0;
		} else {
			id++;
		}
	}		
	
	chEvtRegister(&ss_event, &ss_listener, 0);
	
	//dcmiStartOneShot(&DCMID);
	//chThdSleepMilliseconds(500);

	while (true) {
	
		//evt = chEvtWaitAny(ALL_EVENTS);
		
		//if (evt & EVENT_MASK(0)) {

			palSetPad(GPIOD, 13) ; // Orange.
			palSetPad(GPIOD, 15); // Blue.
			
			memset(spiRxBuff, 0x00, SPI_COMMAND_SIZE);	
			spiSelect(&SPID1);
			//chThdSleepMilliseconds(1);
			spiExchange(&SPID1, SPI_COMMAND_SIZE, spiTxBuff, spiRxBuff);
			//chThdSleepMilliseconds(1);
			//spiReceive(&SPID1, SPI_COMMAND_SIZE, spiRxBuff);
			spiUnselect(&SPID1);
			
			// A little pause is needed for the communication to work, 400 NOP loops last about 26 us.
			// Probably this pause can be avoided/reduced since we loose some time computing the checksum...	
			for(delay=0; delay<SPI_DELAY; delay++) {
				__NOP();
			}
			
			// Compute the checksum (block check character) to verify the command is received correctly.
			// If the command is incorrect, wait for the next command, this is an easy way to synchronize the two chips.
			checksum = 0;
			for(i=0; i<SPI_COMMAND_SIZE-1; i++) {
				checksum += spiRxBuff[i];
			}
			checksum = checksum &0xFF;
			// The checksum coupled with the two fixed bytes increase the probability that the command packet is identified correctly.
			if(checksum != spiRxBuff[SPI_COMMAND_SIZE-1] || spiRxBuff[0]!=0xAA || spiRxBuff[1]!=0xBB) {	
				//chprintf((BaseSequentialStream *)&SDU1, "F:%.3d %.3d %.3d %.3d %.3d %.3d %.3d\r\n", spiRxBuff[0], spiRxBuff[1], spiRxBuff[2], spiRxBuff[3], spiRxBuff[SPI_COMMAND_SIZE-3], spiRxBuff[SPI_COMMAND_SIZE-2], spiRxBuff[SPI_COMMAND_SIZE-1]);			
				//chThdSleepMilliseconds(1); // Give time to the ESP32 to be listening.
				continue;
			}
					
			palClearPad(GPIOD, 15); // Blue.
			
			/*
			spiSelect(&SPID1);
			spiExchange(&SPID1, SPI_COMMAND_SIZE, spiTxBuff, spiRxBuff);
			//spiReceive(&SPID1, SPI_COMMAND_SIZE, spiRxBuff);
			spiUnselect(&SPID1);
			
			// A little pause is needed for the communication to work, 400 NOP loops last about 26 us.
			// Probably this pause can be avoided since we loose some time computing the checksum...	
			for(delay=0; delay<SPI_DELAY; delay++) {
				__NOP();
			}		
			*/
			
			// Modify the packet content in order to test the exchange of a dynamic payload instead of a fixed one.
			/*
			if(spiTxBuff[0] >= SPI_COMMAND_SIZE*0) {
				for(i=0; i<SPI_COMMAND_SIZE; i++) {
					spiTxBuff[i] = i;
				}
			} else {
				for(i=0; i<SPI_COMMAND_SIZE; i++) {
					spiTxBuff[i] += SPI_COMMAND_SIZE;
				}		
			}
			*/

			numPackets = 76800/SPI_DATA_PAYLOAD_SIZE;
			remainingBytes = 76800%SPI_DATA_PAYLOAD_SIZE;
			spiDataIndex = 0;	

			for(packetId=0; packetId<numPackets; packetId++) {
				/*
				spiHeader[0] = packetId&0xFF;
				spiHeader[1] = packetId>>8;
				spiHeader[2] = SPI_DATA_PAYLOAD_SIZE&0xFF;
				spiHeader[3] = SPI_DATA_PAYLOAD_SIZE>>8;
				
				spiSelect(&SPID1);
				spiSend(&SPID1, SPI_DATA_HEADER_SIZE, spiHeader);
				spiUnselect(&SPID1);
				// A little pause is needed for the communication to work, 400 NOP loops last about 26 us.
				for(delay=0; delay<SPI_DELAY; delay++) {
					__NOP();
				}			
				*/
							
				spiSelect(&SPID1);
				//chThdSleepMilliseconds(1);
				spiSend(&SPID1, SPI_DATA_PAYLOAD_SIZE, &sample_buffer[spiDataIndex]);
				//chThdSleepMilliseconds(1);
				spiUnselect(&SPID1);
				// A little pause is needed for the communication to work, 400 NOP loops last about 26 us.
				for(delay=0; delay<SPI_DELAY; delay++) {
					__NOP();
				}			

				spiDataIndex += SPI_DATA_PAYLOAD_SIZE;
			}
			if(remainingBytes > 0) {
				/*
				spiHeader[0] = packetId&0xFF;
				spiHeader[1] = packetId>>8;
				spiHeader[2] = remainingBytes&0xFF;
				spiHeader[3] = remainingBytes>>8;
				
				spiSelect(&SPID1);
				spiSend(&SPID1, SPI_DATA_HEADER_SIZE, spiHeader);
				spiUnselect(&SPID1);
				// A little pause is needed for the communication to work, 400 NOP loops last about 26 us.
				for(delay=0; delay<SPI_DELAY; delay++) {
					__NOP();
				}
				*/
							
				spiSelect(&SPID1);
				//palSetPad(GPIOD, 15); // Blue.
				//chThdSleepMilliseconds(1);
				spiSend(&SPID1, remainingBytes, &sample_buffer[spiDataIndex]);
				//chThdSleepMilliseconds(1);
				//palClearPad(GPIOD, 15); // Blue.
				spiUnselect(&SPID1);
				
				// A little pause is needed for the communication to work, 400 NOP loops last about 26 us.
				for(delay=0; delay<SPI_DELAY; delay++) {
					__NOP();
				}
			}		
			
			/*
			for(transCount=0; transCount<76800/SPI_PACKET_SIZE; transCount++) {
				spiSelect(&SPID1);
				spiSend(&SPID1, SPI_PACKET_SIZE, &sample_buffer[transCount*SPI_PACKET_SIZE]);
				spiUnselect(&SPID1);
				// A little pause is needed for the communication to work, 400 NOP loops last about 26 us.
				for(delay=0; delay<SPI_DELAY; delay++) {
					__NOP();
				}
			}
			*/
			
			
			palClearPad(GPIOD, 13) ; // Orange.
			
			//chprintf((BaseSequentialStream *)&SDU1, "recv: %d, %d, %d, %d, %d, %d, %d\r\n", spiRxBuff[0], spiRxBuff[1], spiRxBuff[2], spiRxBuff[3], spiRxBuff[SPI_COMMAND_SIZE-3], spiRxBuff[SPI_COMMAND_SIZE-2], spiRxBuff[SPI_COMMAND_SIZE-1]);		
			//chprintf((BaseSequentialStream *)&SDU1, "n=%d, r=%d\r\n", numPackets, remainingBytes);			
			
			//chThdSleepMilliseconds(50);
			//dcmiStartOneShot(&DCMID);
			//chThdSleepMilliseconds(500);
			
		//} // Event handling.
		
		//break;
		
	} // Infinite loop.
	
	free(sample_buffer);
	
}

void adc_start(void) {
    adcStart(&ADCD2, NULL);
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

int main(void)
{

    halInit();
    chSysInit();
    mpu_init();

    /** Inits the Inter Process Communication bus. */
    messagebus_init(&bus, &bus_lock, &bus_condvar);

    parameter_namespace_declare(&parameter_root, NULL, NULL);

	sdStart(&SD3, NULL); // UART3.
	usb_start();
	i2c_start();
	e_led_clear();
	e_set_body_led(0);
	e_set_front_led(0);
//	imu_start();
	adc_start();
	proximity_start();
	dac_start();	
	motors_init();
	
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
    //shell_start();

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

//	osalEventObjectInit(&ss_event);
	
	/*
	* SPI1 maximum speed is 42 MHz, ESP32 supports at most 10MHz, so use a prescaler of 1/8 (84 MHz / 8 = 10.5 MHz).
	* SPI1 configuration (10.5 MHz, CPHA=0, CPOL=0, MSb first).
	*/	
//	static const SPIConfig hs_spicfg = {
//		NULL,
//		GPIOA,
//		15,
//		SPI_CR1_BR_1
//		//SPI_CR1_BR_1 | SPI_CR1_BR_0 // 5.25 MHz
//	};		
//	spiStart(&SPID1, &hs_spicfg);       /* Setup transfer parameters. */
	//chThdCreateStatic(spi_thread_wa, sizeof(spi_thread_wa), NORMALPRIO, spi_thread, NULL);
	//chThdCreateStatic(spi_thread_wa, sizeof(spi_thread_wa), NORMALPRIO + 1, spi_thread, NULL);
	
	int16_t gyro[3];
	int16_t acc[3];

	uint8_t playing = 0;
	uint8_t tof_measuring = 0;
	
	uint8_t sdState = 0;
	FRESULT fr;
	FIL fil;
	FATFS fs;           // Filesystem object.
	BYTE work[1024]; /* Work area (larger is better for processing time) */
	int rc;
	DWORD buff[512];  /* 2048 byte working buffer */	
	
	signed int leftSpeed=0, rightSpeed=0;

    messagebus_topic_t *topic = messagebus_find_topic_blocking(&bus, "/proximity");
    proximity_msg_t proximity;

	chThdSleepMilliseconds(5000);
	
    /* Infinite loop. */
    while (1) {
        chThdSleepMilliseconds(10);
	
		switch(getselector()) {
			case 0:
				mpu9250_read_gyro_raw((int16_t *)&gyro);
				if (SDU1.config->usbp->state != USB_ACTIVE) { // Skip printing if port not opened.
					continue;
				}
				chprintf((BaseSequentialStream *)&SDU1, "%gyro: x=%d, y=%d, z=%d\r\n", gyro[0], gyro[1], gyro[2]);
				break;
				
			case 1:
				mpu9250_read_acc_raw((int16_t *)&acc);
				if (SDU1.config->usbp->state != USB_ACTIVE) { // Skip printing if port not opened.
					continue;
				}
				chprintf((BaseSequentialStream *)&SDU1, "acc: x=%d, y=%d, z=%d\r\n", acc[0], acc[1], acc[2]);
				break;
				
			case 2:
				e_set_led(0, 2);
				e_set_led(1, 2);
				e_set_led(2, 2);
				e_set_led(3, 2);
				e_set_body_led(2);
				e_set_front_led(2);
				break;
				
			case 3:
				// Read proximity sensors.
				messagebus_topic_wait(topic, &proximity, sizeof(proximity));

				if (SDU1.config->usbp->state != USB_ACTIVE) { // Skip printing if port not opened.
					continue;
				}

				// Sensors info print: each line contains data related to a single sensor.
//		        for (int i = 0; i < PROXIMITY_NB_CHANNELS; i++) {
//		        	chprintf((BaseSequentialStream *)&SDU1, "%.4d,", proximity.ambient[i]);
//		        	chprintf((BaseSequentialStream *)&SDU1, "%.4d,", proximity.reflected[i]);
//		        	chprintf((BaseSequentialStream *)&SDU1, "%.4d", proximity.delta[i]);
//		        	chprintf((BaseSequentialStream *)&SDU1, "\r\n");
//		        }
//		        chprintf((BaseSequentialStream *)&SDU1, "\r\n");

				// CSV print: each line contains IR0amb, IR0ref, IR0delta, IR1amb, ..., IR7delta.
//		        for (int i = 0; i < PROXIMITY_NB_CHANNELS; i++) {
//		        	chprintf((BaseSequentialStream *)&SDU1, "%.4d;%.4d;%.4d;", proximity.ambient[i], proximity.reflected[i], proximity.delta[i]);
//		        }
//		        chprintf((BaseSequentialStream *)&SDU1, "\r\n");

		        // CSV print: each line contains only delta values for each sensor.
		        for (int i = 0; i < PROXIMITY_NB_CHANNELS; i++) {
		        	chprintf((BaseSequentialStream *)&SDU1, "%d;", proximity.delta[i]);
		        }
		        chprintf((BaseSequentialStream *)&SDU1, "\r\n");
				break;
				
			case 4:
				if(playing == 0) {
					//audio_start(200);
					dac_play(200);
					playing = 1;
				}
				break;
				
			case 5:
				if(playing == 0) {
					//audio_start(2000);
					dac_play(2000);
					playing = 1;
				}			
				break;
				
			case 6:
				if(playing == 0) {
					//audio_start(10000);
					dac_play(10000);
					playing = 1;
				}
				break;
				
			case 7:
				if(tof_measuring == 0) {
					tof_measuring = 1;
					VL53L0X_init_demo();
				}
				break;
				
			case 8:
				right_motor_set_speed(2200);
				left_motor_set_speed(-2200);
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
				messagebus_topic_wait(topic, &proximity, sizeof(proximity));
				leftSpeed = 2000 - proximity.delta[0]*4 - proximity.delta[1]*2;
				rightSpeed = 2000 - proximity.delta[7]*4 - proximity.delta[6]*2;
				right_motor_set_speed(rightSpeed);
				left_motor_set_speed(leftSpeed);
				break;
				
			case 12:
				run_asercom();
				break;
				
			case 13:
				break;
				
			case 14:
				break;
				
			case 15:
				break;				
		}
		
		//e_set_body_led(2);
		//e_set_front_led(2);
		
        // Led toggled to verify main is running and to show DCMI state.
        if(dcmiErrorFlag == 1) {
			//e_set_led(0, 0);
			//e_set_led(2, 2);
        } else {
			//e_set_led(0, 2);
			//e_set_led(2, 0);
        }

        //chprintf((BaseSequentialStream *)&SDU1, "%d\r\n", dmaStreamGetTransactionSize(DCMID.dmastp));

		
        if(txComplete == 1) {
            txComplete = 0;
			
			chThdSleepMilliseconds(5000);
			
			//chThdCreateStatic(spi_thread_wa, sizeof(spi_thread_wa), NORMALPRIO, spi_thread, NULL);

            //palClearPad(GPIOD, 15); // Blue.
            //palClearPad(GPIOD, 13) ; // Orange.

			
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
