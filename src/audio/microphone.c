#include <string.h>
#include <ch.h>
#include <hal.h>
#include "microphone.h"
#include "mp45dt02_processing.h"

static void handlePCMdata(float *data, uint16_t samples) {
	(void)data;
	(void)samples;
	return;
}

void mic_start(void) {
	// Vedi:
	// * ChibiOS\os\hal\ports\STM32\LLD\TIMv1\icu_lld.c
	//   - timer input channel example
	//
	// * https://github.com/alanbarr/STM32F4_Streaming_Mic
	//   - S:\_projects\STM32F4_Streaming_Mic-master
	//   - esempio ChibiOS per microfono singolo => esempio di configurazione I2S (file stm32_streaming\audio\mp45dt02_processing.c)
	//   - utile per conversione PDM => PCM e per trasferimento al PC (lwip)
	//
	// * ChibiOS\testhal\STM32\STM32F4xx\I2S
	//   - esempio di configurazione dell'I2S
	//
	// * K:\stm32F4\dev\STM32CubeExpansion_MEMSMIC1_V1.3.1\Projects\Multi\Applications\Microphones_Streaming
	//   - esempio con librerie STM che gestisce fino a 4 microfoni
	//
	// * K:\stm32F4\dev\STM32Cube_FW_F4_V1.14.0\Projects\STM32F4-Discovery\Examples\AUDIO:
	// * K:\AUDIO_4mic_ok
	// * K:\AUDIO_2mic_ok
	// * K:\AUDIO_no_timer
	// * K:\AUDIO-discovery
	//   - esempio con librerie STM che sono riuscito a fare andare con i 4 microfoni e vedere i dati tramite external debugger
	//
	// * K:\stm32F4\dev\STM32Cube_FW_F4_V1.14.0\Projects\STM32F4-Discovery\Applications\Audio\Audio_playback_and_record:
	// * K:\Audio_playback_and_record
	//   - esempio con librerie STM che campiona 2 microfoni e salva su penna USB


	// *******************
	// TIMER CONFIGURATION
	// *******************
	// TIM9CH1 => input (spi clock) ==> da configurare manualmente (tramite registri)
	// TIM9CH2	=> output (clock for microphones, 1/2 of input clock) ==> usare PWM driver

    rccEnableTIM9(FALSE);
    rccResetTIM9();
    //nvicEnableVector(STM32_TIM9_NUMBER, STM32_ICU_TIM9_IRQ_PRIORITY);

    STM32_TIM9->SR   = 0; // Clear eventual pending IRQs.
    STM32_TIM9->DIER = 0; // DMA-related DIER settings => DMA disabled.

    // Input channel configuration.
    STM32_TIM9->CCER &= ~STM32_TIM_CCER_CC1E; // Channel 1 capture disabled.
    STM32_TIM9->CCMR1 &= ~STM32_TIM_CCMR1_CC1S_MASK; // Reset channel selection bits.
    STM32_TIM9->CCMR1 |= STM32_TIM_CCMR1_CC1S(1); // CH1 Input on TI1.
    STM32_TIM9->CCMR1 &= ~STM32_TIM_CCMR1_IC1F_MASK; // No filter.
    STM32_TIM9->CCER &= ~(STM32_TIM_CCER_CC1P | STM32_TIM_CCER_CC1NP); // Rising edge, non-inverted.
    STM32_TIM9->CCMR1 &= ~STM32_TIM_CCMR1_IC1PSC_MASK; // No prescaler

    // Trigger configuration.
    STM32_TIM9->SMCR &= ~STM32_TIM_SMCR_TS_MASK; // Reset trigger selection bits.
    STM32_TIM9->SMCR |= STM32_TIM_SMCR_TS(5); // Input is TI1FP1.
    STM32_TIM9->SMCR &= ~STM32_TIM_SMCR_SMS_MASK; // Reset the slave mode bits.
    STM32_TIM9->SMCR |= STM32_TIM_SMCR_SMS(7); // External clock mode 1 => clock is TI1FP1.

    // Output channel configuration.
    STM32_TIM9->CR1 &= ~STM32_TIM_CR1_CKD_MASK; // No clock division.
    STM32_TIM9->ARR = 1; // Output clock halved.
    STM32_TIM9->PSC = 0; // No prescaler, counter clock frequency = fCK_PSC / (PSC[15:0] + 1).
    STM32_TIM9->EGR = STM32_TIM_EGR_UG; // Enable update event to reload preload register value immediately.
    STM32_TIM9->CCER &= ~STM32_TIM_CCER_CC2E; // Channel 2 output disabled.
    STM32_TIM9->CCMR1 &= ~STM32_TIM_CCMR1_CC2S_MASK; // Reset channel selection bits => channel configured as output.
    STM32_TIM9->CCMR1 &= ~STM32_TIM_CCMR1_OC2M_MASK; // Reset channel mode bits.
    STM32_TIM9->CCMR1 |= STM32_TIM_CCMR1_OC2M(6); // PWM1 mode.
    STM32_TIM9->CCER &= ~(STM32_TIM_CCER_CC2P | STM32_TIM_CCER_CC2NP); // Active high.
    STM32_TIM9->CCR[1] = 1; // Output clock halved.
    STM32_TIM9->CCMR1 |= STM32_TIM_CCMR1_OC2PE; // Enable preload at each update event for channel 2.
    STM32_TIM9->CCER &= ~STM32_TIM_CCMR1_OC2FE; // Disable fast mode.

    // Enable channels.
    STM32_TIM9->CCER |= STM32_TIM_CCER_CC1E | STM32_TIM_CCER_CC2E;
    STM32_TIM9->CR1 |= STM32_TIM_CR1_CEN;
    //STM32_TIM9->CR1 |= STM32_TIM_CR1_ARPE | STM32_TIM_CR1_URS | STM32_TIM_CR1_CEN;

    // ******************
	// I2S2 CONFIGURATION
    // ******************
    mp45dt02Config micConfig;
    memset(&micConfig, 0, sizeof(micConfig));
    micConfig.fullbufferCb = handlePCMdata; // Callback chiamata quando buffer riempito con PCM data (1 ms of data).
    mp45dt02Init(&micConfig);


    // ******************
	// SPI3 CONFIGURATION
    // ******************
	// SPI3



}



