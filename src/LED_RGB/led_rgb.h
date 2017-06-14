/**
 * @file    led_rgb.h
 * @brief   Functions to use the LED RGB I2C driver.
 * 
 * @author  Eliot Ferragni
 */


#ifndef LED_RGB_H
#define LED_RGB_H


#define LED1_DEV_CARD_ADDR		0x62
#define LED2_DEV_CARD_ADDR		0x60
#define LED3_DEV_CARD_ADDR		0x66

typedef uint8_t LED_RGB_COLOR_t;

#define LED_RGB_RED		((LED_RGB_COLOR_t)(2<<5))
#define LED_RGB_GREEN	((LED_RGB_COLOR_t)(3<<5))
#define LED_RGB_BLUE	((LED_RGB_COLOR_t)(4<<5))

#define LED_RGB_MAX_VALUE		0x1F

//////////////////// PROTOTYPES PUBLIC FUNCTIONS /////////////////////

/**
 * @brief Set the maximal current for the leds. To be configured at first
 * 
 * @param address I2C address of the LED GRB driver (8bit)
 * @param current Value of the maximal current to be allowed. From 0 to LED_RGB_MAX_VALUE
 * 
 * @return msg_t message
 */
msg_t led_rgb_set_current(uint8_t address, uint8_t current);

/**
 * @brief Set the intensity of the color chosen for the LED chosen
 * 
 * @param address I2C address of the LED GRB driver (8bit)
 * @param color Color we want to adjust the intensity
 * @param intensity Intensity. From 0 to LED_RGB_MAX_VALUE
 * @return msg_t message
 */
msg_t led_rgb_set_intensity(uint8_t address, LED_RGB_COLOR_t color, uint8_t intensity);

/**
 * @brief Set the intensity of the three colors of one LED
 * 
 * @param address I2C address of the LED GRB driver (8bit)
 * @param intensity_red Intensity of the red color
 * @param intensity_green Intensity of the green color
 * @param intensity_blue Intensity of the blue color
 * @return msg_t message
 */
msg_t led_rgb_set_intensity_all(uint8_t address, uint8_t intensity_red, uint8_t intensity_green, uint8_t intensity_blue);

/**
 * @brief Init a thread which runs a demo varying the color of the three LEDs.
 */
void led_rgb_init_demo(void);

#endif /* LED_RGB_H*/