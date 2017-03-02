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
//#include "camera/stm32f4xx_rcc.h"
//#include "camera/stm32f4xx_dma.h"

#define SHELL_WA_SIZE   THD_WORKING_AREA_SIZE(2048)

parameter_namespace_t parameter_root, aseba_ns;

#define IMAGE_SIZE 160*120*2

uint32_t dmamode;
const stm32_dma_stream_t *dmastp;
unsigned char sample_buffer[IMAGE_SIZE] = {0}; // bytes = 160*120*2 = 38400
uint32_t dcmiFlags;
uint32_t dmaFlags;
unsigned char txComplete = 0;
unsigned char tempBuff[3] = {0xAA, 0xBB, 0xCC};
unsigned char ledToggle = 0;

static bool load_config(void)
{
    extern uint32_t _config_start;

    return config_load(&parameter_root, &_config_start);
}

void my_button_cb(void) {
    txComplete = 0;
    dcmiFlags = 0;
    dmaFlags = 0;
//    dmaStreamDisable(dmastp);
//    dmaStreamSetTransactionSize(dmastp, IMAGE_SIZE/4);
//    dmaStreamEnable(dmastp);
    palSetPad(GPIOD, 10);
    DCMI->CR |= DCMI_CR_CAPTURE;
}

int32_t dma_start(void) {
	(dmastp)->stream->CR |= (STM32_DMA_CR_EN);
	return 0;
}

int32_t dma_stop(void) {
	(dmastp)->stream->CR &= ~(STM32_DMA_CR_EN);
	return 0;
}

static void dma_interrupt(void *p, uint32_t flags) {
	(void)p;

    dmaFlags |= flags;

	if ((flags & STM32_DMA_ISR_DMEIF) != 0) {

	}
	if ((flags & STM32_DMA_ISR_TEIF) != 0) {

	}
	if ((flags & STM32_DMA_ISR_HTIF) != 0) {
		//palSetPad(GPIOD, 10);
	}
	if ((flags & STM32_DMA_ISR_TCIF) != 0) {
		/* stop dma */
		//dma_stop();
		palClearPad(GPIOD, 10);
		DCMI->CR &= ~(DCMI_CR_CAPTURE);
		txComplete = 1;
	}
}

CH_IRQ_HANDLER(Vector178) {
    CH_IRQ_PROLOGUE();

    uint32_t flags;

//  #if STM32_DCMI_USE_DCMI1
//    DCMI->ICR |= (1<<4)|(1<<3)|(1<<2)|(1<<1)|(1<<0)|STM32_DCMI_ICR_FRAME_ISC;
//    _dcmi_isr_code(&DCMID1);
//  #endif

    flags = DCMI->MISR;
    dcmiFlags |= flags;

	if ((flags & DCMI_MISR_ERR_MIS) != 0) { // Sync error.
		DCMI->ICR |= DCMI_ICR_ERR_ISC;
	}

	if ((flags & DCMI_MISR_FRAME_MIS) != 0) { // Capture complete.
		DCMI->ICR |= DCMI_ICR_FRAME_ISC;
	}

	if ((flags & DCMI_MISR_OVF_MIS) != 0) { // DMA overflow.
		DCMI->ICR |= DCMI_ICR_OVF_ISC;
	}

    if ((flags & DCMI_MISR_VSYNC_MIS) != 0) { // Frame start.
		DCMI->ICR |= DCMI_ICR_VSYNC_ISC;
	}

    CH_IRQ_EPILOGUE();
}

int main(void)
{
    int initError = 0;

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
        demo_led_init();
        //aseba_vm_start();
    }

    //demo_acc_start(accelerometer_cb);
    demo_button_start(my_button_cb);

    /* Start shell on the USB port. */
    //shell_start();


    /* Configure PO8030 camera */
    // Generate the master clock for the camera.
    palSetPadMode(GPIOA, 8, PAL_MODE_ALTERNATE(0)); // MCO1 output from HSI (16 MHz) => see "mcuconf.h".

    // Configure I2C communication.
    palSetPadMode(GPIOB, 8, PAL_MODE_ALTERNATE(4)); // I2C1 SCL.
    //palSetPadMode(GPIOB, 9, PAL_MODE_ALTERNATE(4)); // I2C1 SDA. Already setup in the board file (ChibiOS/os/hal/boards/ST_STM32F4_DISCOVERY/board.h).
    static const I2CConfig i2cfg1 = {
        OPMODE_I2C,
        400000,
        FAST_DUTY_CYCLE_2,
    };
    i2cStart(&I2CD1, &i2cfg1);

    // Keep reset pin low for at least 8 x MCLK cycles...100 ms is more than enough.
    palSetPadMode(GPIOC, 10, PAL_MODE_OUTPUT_PUSHPULL); // PO8030 RST pin.
    palWritePad(GPIOC, 10, PAL_HIGH);
    chThdSleepMilliseconds(10);
    palWritePad(GPIOC, 10, PAL_LOW);
    chThdSleepMilliseconds(100);
    palWritePad(GPIOC, 10, PAL_HIGH);

    po8030_init();

    // Configure the digital camera interface (DCMI).
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
    rccEnableAHB2(0x00000001, TRUE); // Enable DCMI clock.
    // Control Regsiter.
    //DCMI->CR &= ~(DCMI_CR_CM); // Continuous mode.
    DCMI->CR |= (DCMI_CR_CM); // Snapshot mode.
    DCMI->CR &= ~(DCMI_CR_ESS); // Hardware synchronization.
    DCMI->CR |= DCMI_CR_PCKPOL; // Rising edge active.
    DCMI->CR &= ~(DCMI_CR_VSPOL); // VSYNC active low (it means 1 when valid data are coming).
    DCMI->CR &= ~(DCMI_CR_HSPOL); // HSYNC active low (it means 1 when valid data are coming).
    DCMI->CR &= ~(DCMI_CR_FCRC_0); // All frames are captured.
    DCMI->CR &= ~(DCMI_CR_FCRC_1);
    DCMI->CR &= ~(DCMI_CR_EDM_0); // 8-bit mode.
    DCMI->CR &= ~(DCMI_CR_EDM_1);
    DCMI->CR |= DCMI_CR_ENABLE;
    // Interrupt enable register.
    DCMI->IER |= DCMI_IER_ERR_IE; // Synchronization error.
    DCMI->IER |= DCMI_IER_FRAME_IE; // Capture complete.
    DCMI->IER |= DCMI_IER_OVF_IE; // Overrun (by DMA).
    DCMI->IER |= DCMI_IER_VSYNC_IE; // Interrupt generated when vsync become active.
    nvicEnableVector(DCMI_IRQn, 6);

    // DMA Configuration.
    rccEnableAHB1(0x00400000, TRUE); // Enable DMA2 clock.
    dmastp = STM32_DMA2_STREAM1; //STM32_DMA_STREAM(STM32_DMA_STREAM_ID(2, 1)); // DMA2, stream 1.
    if(dmaStreamAllocate(dmastp, 12, (stm32_dmaisr_t)dma_interrupt, NULL) == TRUE) {
        initError = 1;
    } else {
        initError = 0;
    }
    dmaStreamDisable(dmastp);
    dmamode = STM32_DMA_CR_CHSEL(1) | // Channel 1.
        STM32_DMA_CR_PL(2) | // High priority level.
        STM32_DMA_CR_DIR_P2M | // Peripheral to memory.
        STM32_DMA_CR_MSIZE_WORD | // Memory data size = 4 bytes.
        STM32_DMA_CR_PSIZE_WORD | // Peripheral data size = 4 bytes.
        STM32_DMA_CR_MINC | // Increment memory address after each data transfer.
        STM32_DMA_CR_DMEIE | // Direct mode error interrupt enabled.
        STM32_DMA_CR_TEIE | // Transfer error interrupt enabled.
        STM32_DMA_CR_CIRC | // Circular mode.
        STM32_DMA_CR_TCIE | // Transfer complete interrupt enabled.
        STM32_DMA_CR_HTIE | // Half transfer interrupt enabled.
        STM32_DMA_CR_MBURST_SINGLE |
        STM32_DMA_CR_PBURST_SINGLE;
    // By default:
    // - single transfer (no burst)
    // - direct mode enabled (FIFO disabled)
    dmaStreamSetPeripheral(dmastp, &DCMI->DR);
    dmaStreamSetMemory0(dmastp, (uint32_t)sample_buffer);
    dmaStreamSetTransactionSize(dmastp, IMAGE_SIZE/4); //160*120*2/4 => num bytes/4; 4 is the data size.
    dmaStreamSetMode(dmastp, dmamode);
    dmaStreamSetFIFO(dmastp, STM32_DMA_FCR_DMDIS | STM32_DMA_FCR_FTH_HALF);
    dmaStreamEnable(dmastp);

    // Configure pin D10 for debugging (timing measurements).
    palSetPadMode(GPIOD, 10, PAL_MODE_OUTPUT_PUSHPULL); // Debug pin.
    //palWritePad(GPIOD, 10, PAL_LOW);
    palClearPad(GPIOD, 10);

    /* Infinite loop, do nothing. */
    while (1) {
        chThdSleepMilliseconds(500);

        ledToggle = 1 - ledToggle;
        demo_led_set(3, 128*ledToggle); // Orange led toggled to verify main is running.

        //chprintf((BaseSequentialStream *)&SDU1, "%d, %d, %x %x %x %x, %d, %x, %x\r\n", initError, sample_buffer[0], ((DCMI->DR)>>24)&0xFF, ((DCMI->DR)>>16)&0xFF, ((DCMI->DR)>>8)&0xFF, (DCMI->DR)&0xFF, dmaStreamGetTransactionSize(dmastp), DCMI->MISR, dcmiFlags);
        //chprintf((BaseSequentialStream *)&SDU1, "%x %x\r\n", dmaFlags>>8, dmaFlags);
        if(txComplete == 1) {
            txComplete = 0;
            chnWrite((BaseSequentialStream *)&SDU1, sample_buffer, IMAGE_SIZE);
        }
    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}
