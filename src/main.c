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

#include "discovery_demo/accelerometer.h"
#include "discovery_demo/leds.h"
#include "discovery_demo/button.h"

//#include "aseba_vm/aseba_node.h"
//#include "aseba_vm/skel_user.h"
//#include "aseba_vm/aseba_can_interface.h"
//#include "aseba_vm/aseba_bridge.h"

#include "camera/po8030.h"

#define SHELL_WA_SIZE   THD_WORKING_AREA_SIZE(2048)

#define SPI_COMMAND_SIZE 64
#define SPI_DATA_HEADER_SIZE 4
#define SPI_DATA_PAYLOAD_SIZE 4092
#define SPI_DELAY 400

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
	uint16_t transCount = 0; // image size / SPI_BUFF_LEN
	uint8_t id = 0;
	
	uint16_t checksum = 0;
	
	spiTxBuff[0] = 0xAA;
	checksum += spiTxBuff[0];
	spiTxBuff[1] = 0xBB;
	checksum += spiTxBuff[1];
	for(i=2; i<SPI_COMMAND_SIZE-1; i++) {
		spiTxBuff[i] = i+1;
		checksum += spiTxBuff[i];
	}
	spiTxBuff[SPI_COMMAND_SIZE-1] = checksum&0xFF; // Block check character checksum.
	
	
	volatile uint32_t delay = 0;
	
	uint16_t packetId = 0;
	uint16_t numPackets = 0;
	uint32_t remainingBytes = 0;
	uint32_t spiDataIndex = 0;
	
	// Create a fixed packet content for debugging.
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

/*
	while(1) {
		memset(spiRxBuff, 0xFF, SPI_COMMAND_SIZE);	
		spiSelect(&SPID1);
		spiExchange(&SPID1, SPI_COMMAND_SIZE, spiTxBuff, spiRxBuff);
		//spiReceive(&SPID1, SPI_COMMAND_SIZE, spiRxBuff);
		spiUnselect(&SPID1);

		for(i=0; i<SPI_COMMAND_SIZE; i++) {
			printf("%d, ", spiRxBuff[i]);
		}
		printf("\r\n\n");		
		
		chThdSleepMilliseconds(1000);
		
	}
*/
	
	while (true) {

		palSetPad(GPIOD, 13) ; // Orange.
		
		memset(spiRxBuff, 0x00, SPI_COMMAND_SIZE);	
		spiSelect(&SPID1);
		spiExchange(&SPID1, SPI_COMMAND_SIZE, spiTxBuff, spiRxBuff);
		//spiReceive(&SPID1, SPI_COMMAND_SIZE, spiRxBuff);
		spiUnselect(&SPID1);
		
		// A little pause is needed for the communication to work, 400 NOP loops last about 26 us.
		// Probably this pause can be avoided since we loose some time computing the checksum...	
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
		if(checksum != spiRxBuff[SPI_COMMAND_SIZE-1] || spiRxBuff[0]!=0xAA || spiRxBuff[1]!=0xBB) {	
			continue;
		}
		
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
			spiSend(&SPID1, SPI_DATA_PAYLOAD_SIZE, &sample_buffer[spiDataIndex]);
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
			spiSend(&SPID1, remainingBytes, &sample_buffer[spiDataIndex]);
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
		
//		dcmiStartOneShot(&DCMID);
		
		
		palClearPad(GPIOD, 13) ; // Orange.
		
		chThdSleepMilliseconds(50);
	}
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
    if(po8030_config(FORMAT_YCBYCR, SIZE_QQVGA) != MSG_OK) { // Default configuration.
        dcmiErrorFlag = 1;
    }

	/*
	capture_mode = CAPTURE_ONE_SHOT;
	double_buffering = 0;
	po8030_save_current_format(FORMAT_YYYY);
	po8030_save_current_subsampling(SUBSAMPLING_X1, SUBSAMPLING_X1);
	po8030_advanced_config(FORMAT_YYYY, 1, 1, 320, 240, SUBSAMPLING_X1, SUBSAMPLING_X1);
	sample_buffer = (uint8_t*)malloc(po8030_get_image_size());
	dcmiPrepare(&DCMID, &dcmicfg, po8030_get_image_size(), (uint32_t*)sample_buffer, NULL);
	palSetPad(GPIOD, 13) ; // Orange.
	dcmiStartOneShot(&DCMID);
	*/

	
	/*
	* Maximum speed SPI configuration (21MHz, CPHA=0, CPOL=0, MSb first).
	*/
	// static const SPIConfig hs_spicfg = {
		// NULL,
		// GPIOB,
		// 12,
		// 0
	// };
	//static const SPIConfig hs_spicfg = {
	//	NULL,
	//	GPIOB,
	//	12,
	//	SPI_CR1_BR_0 //SPI_CR1_BR_2 | SPI_CR1_BR_1
	//};
	//static const SPIConfig hs_spicfg = {
	//	NULL,
	//	GPIOB,
	//	12,
	//	SPI_CR1_BR_2 | SPI_CR1_BR_1
	//};
	
	/*
	* SPI1 maximum speed is 42 MHz, ESP32 supports at most 10MHz, so use a prescaler of 1/8 (84 MHz / 8 = 10.5 MHz).
	* SPI1 configuration (10.5 MHz, CPHA=0, CPOL=0, MSb first).
	*/	
	static const SPIConfig hs_spicfg = {
		NULL,
		GPIOA,
		15,
		SPI_CR1_BR_1
	};	
	
	//spiAcquireBus(&SPID1);              /* Acquire ownership of the bus.    */		
	spiStart(&SPID1, &hs_spicfg);       /* Setup transfer parameters. */
	chThdCreateStatic(spi_thread_wa, sizeof(spi_thread_wa), NORMALPRIO, spi_thread, NULL);
	//chThdCreateStatic(spi_thread_wa, sizeof(spi_thread_wa), NORMALPRIO + 1, spi_thread, NULL);

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
