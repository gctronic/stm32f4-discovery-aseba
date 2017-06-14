/**
 * @file    led_rgb.c
 * @brief   Functions to use the LED RGB I2C driver.
 *
 * @author  Eliot Ferragni
 */


#include "hal.h"
#include "led_rgb.h"

#define LED_RGB_CURRENT_REG		(1<<5)

//////////////////// PUBLIC FUNCTIONS /////////////////////////


msg_t led_rgb_set_current(uint8_t address, uint8_t current) {

  systime_t timeout = MS2ST(50); // 50 ms
  msg_t rdymsg = MSG_OK;

  uint8_t txbuff[1];
  uint8_t rxbuff[0];

  txbuff[0] = ((current & 0x1F) | LED_RGB_CURRENT_REG);

  i2cAcquireBus(&I2CD1);
  rdymsg = i2cMasterTransmitTimeout(&I2CD1, address>>1, txbuff, 1, rxbuff, 0, timeout);
  i2cReleaseBus(&I2CD1);

  return rdymsg;
}

msg_t led_rgb_set_intensity(uint8_t address, LED_RGB_COLOR_t color, uint8_t intensity) {

  systime_t timeout = MS2ST(50); // 50 ms
  msg_t rdymsg = MSG_OK;

  uint8_t txbuff[1];
  uint8_t rxbuff[0];

  txbuff[0] = ((intensity & LED_RGB_MAX_VALUE) | color);

  i2cAcquireBus(&I2CD1);
  rdymsg = i2cMasterTransmitTimeout(&I2CD1, address>>1, txbuff, 1, rxbuff, 0, timeout);
  i2cReleaseBus(&I2CD1);

  return rdymsg;
}

msg_t led_rgb_set_intensity_all(uint8_t address, uint8_t intensity_red, uint8_t intensity_green, uint8_t intensity_blue) {

  msg_t rdymsg = MSG_OK;

  rdymsg = led_rgb_set_intensity(address, LED_RGB_RED, intensity_red);

  if(rdymsg == MSG_OK){
    rdymsg = led_rgb_set_intensity(address, LED_RGB_GREEN, intensity_green);
  }

  if(rdymsg == MSG_OK){
    rdymsg = led_rgb_set_intensity(address, LED_RGB_BLUE, intensity_blue);
  }

  return rdymsg;
}

static THD_WORKING_AREA(waLEDRGBThd, 256);
static THD_FUNCTION(LEDRGBThd, arg) {

    (void)arg;

    uint8_t i = 0;
    uint8_t step = 0;

    led_rgb_set_current(LED1_DEV_CARD_ADDR, LED_RGB_MAX_VALUE);
    led_rgb_set_current(LED2_DEV_CARD_ADDR, LED_RGB_MAX_VALUE);
    led_rgb_set_current(LED3_DEV_CARD_ADDR, LED_RGB_MAX_VALUE);

    /* Reader thread loop.*/
    while (TRUE) {

      if((step == 0) && (i <= LED_RGB_MAX_VALUE)){
        led_rgb_set_intensity_all(LED1_DEV_CARD_ADDR, i, 0, LED_RGB_MAX_VALUE-i);
        led_rgb_set_intensity_all(LED2_DEV_CARD_ADDR, LED_RGB_MAX_VALUE-i, i, 0);
        led_rgb_set_intensity_all(LED3_DEV_CARD_ADDR, 0, LED_RGB_MAX_VALUE-i, i);
        i++;
          if(i > LED_RGB_MAX_VALUE){
            i = 0;
            step = 1;
            chThdSleepMilliseconds(50);
            continue;
          }
      }
      if((step == 1) && (i <= LED_RGB_MAX_VALUE)){
        led_rgb_set_intensity_all(LED1_DEV_CARD_ADDR, LED_RGB_MAX_VALUE-i, i, 0);
        led_rgb_set_intensity_all(LED2_DEV_CARD_ADDR, 0, LED_RGB_MAX_VALUE-i, i);
        led_rgb_set_intensity_all(LED3_DEV_CARD_ADDR, i, 0, LED_RGB_MAX_VALUE-i);
        i++;
          if(i > LED_RGB_MAX_VALUE){
            i = 0;
            step = 2;
            chThdSleepMilliseconds(50);
            continue;
          }
      }

      if((step == 2) && (i <= LED_RGB_MAX_VALUE)){
        led_rgb_set_intensity_all(LED1_DEV_CARD_ADDR, 0, LED_RGB_MAX_VALUE-i, i);
        led_rgb_set_intensity_all(LED2_DEV_CARD_ADDR, i, 0, LED_RGB_MAX_VALUE-i);
        led_rgb_set_intensity_all(LED3_DEV_CARD_ADDR, LED_RGB_MAX_VALUE-i, i, 0);
        i++;
          if(i > LED_RGB_MAX_VALUE){
            i = 0;
            step = 0;
            chThdSleepMilliseconds(50);
            continue;
          }
      }

      chThdSleepMilliseconds(50);
    }
}

void led_rgb_init_demo(void){

  chThdCreateStatic(waLEDRGBThd,
                     sizeof(waLEDRGBThd),
                     NORMALPRIO + 10,
                     LEDRGBThd,
                     NULL);

}





