#include <hal.h>
#include <stdlib.h>
#include "epuck1x-wrapper.h"
#include "..\leds.h"
#include "..\utility.h"
#include "..\usbcfg.h"
#include "..\motor.h"
#include "..\sensors\proximity.h"
#include "..\main.h"
#include "..\camera\po8030.h"

// LEDs handling.
void e_set_led(unsigned int led_number, unsigned int value) {
	set_led(led_number, value);
}

void e_led_clear(void) {
	clear_leds();
}

void e_set_body_led(unsigned int value) {
	set_body_led(value);
}

void e_set_front_led(unsigned int value) {
	set_front_led(value);
}

// Various.
int getselector(void) {
	return get_selector();
}

// UARTs handling.
void e_init_uart1(void) {
	return;
}

int  e_ischar_uart1(void) {
	return 0;
}

int  e_getchar_uart1(char *car) {
	return 0;
}

void e_send_uart1_char(const char * buff, int length) {
	return;
}

int  e_uart1_sending(void) {
	return 0;
}

void e_init_uart2(int baud) {
	return;
}

int  e_ischar_uart2(void) {
	return 0;
}

int  e_getchar_uart2(char *car) {
	if (SDU1.config->usbp->state == USB_ACTIVE) {
		//return chnReadTimeout(&SDU1, car, 1, TIME_IMMEDIATE);
		//return chnReadTimeout(&SDU1, car, 1, MS2ST(10));
		return chSequentialStreamRead(&SDU1, car, 1);
	}
}

void e_send_uart2_char(const char * buff, int length) {
	if (SDU1.config->usbp->state == USB_ACTIVE) {
		//chnWriteTimeout(&SDU1, (uint8_t*)buff, length, TIME_INFINITE);
		chSequentialStreamWrite(&SDU1, buff, length);
	}
}

int  e_uart2_sending(void) {
	return 0;
}

// Motors handling.
void e_init_motors(void) {
	return;
}

void e_set_speed_left(int motor_speed) {
	left_motor_set_speed(motor_speed);
}

void e_set_speed_right(int motor_speed) {
	right_motor_set_speed(motor_speed);
}

void e_set_speed(int linear_speed, int angular_speed) {
	if(abs(linear_speed) + abs(angular_speed) > MOTOR_SPEED_LIMIT) {
		return;
	} else {
		left_motor_set_speed (linear_speed - angular_speed);
		right_motor_set_speed(linear_speed + angular_speed);
	}
}

void e_set_steps_left(int steps_left) {
	return;
}

void e_set_steps_right(int steps_right) {
	return;
}

int e_get_steps_left(void) {
	return left_motor_get_pos();
}

int e_get_steps_right(void) {
	return right_motor_get_pos();
}

// Proximity handling.
void e_calibrate_ir(void) {
	calibrate_ir();
}

int e_get_prox(unsigned int sensor_number) {
	return get_prox(sensor_number);
}

int e_get_calibrated_prox(unsigned int sensor_number) {
	return get_calibrated_prox(sensor_number);
}

int e_get_ambient_light(unsigned int sensor_number) {
	return get_ambient_light(sensor_number);
}

// Camera handling.
/**
 * Initalize the camera, return the version in hex, 0x3030, 0x6030 or 0x8030.
 */
int e_poxxxx_init_cam(void) {

	capture_mode = CAPTURE_ONE_SHOT;
	double_buffering = 0;
	po8030_save_current_format(FORMAT_RGB565);
	po8030_save_current_subsampling(SUBSAMPLING_X4, SUBSAMPLING_X4);
	po8030_advanced_config(FORMAT_RGB565, 240, 160, 160, 160, SUBSAMPLING_X4, SUBSAMPLING_X4);
	sample_buffer = (uint8_t*)malloc(po8030_get_image_size());
	dcmiPrepare(&DCMID, &dcmicfg, po8030_get_image_size(), (uint32_t*)sample_buffer, NULL);

	return 0x8030;
}

int e_poxxxx_config_cam(unsigned int sensor_x1,unsigned int sensor_y1,
			 unsigned int sensor_width,unsigned int sensor_height,
			 unsigned int zoom_fact_width,unsigned int zoom_fact_height,
			 int color_mode) {
	return 0;
}

void e_poxxxx_write_cam_registers(void) {
	return;
}

void e_poxxxx_launch_capture(char * buf) {
	camReady = 0;
	set_body_led(1);
	dcmiStartOneShot(&DCMID);
}

/*! Check if the current capture is finished
 * \return Zero if the current capture is in progress, non-zero if the capture is done.
 * \sa e_poxxxx_launch_capture
 */
int e_poxxxx_is_img_ready(void) {
	return camReady;
}
