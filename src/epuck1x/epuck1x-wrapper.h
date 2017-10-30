#ifndef EPUCK1X_WRAPPER_H
#define EPUCK1X_WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

// LEDs handling.
void e_set_led(unsigned int led_number, unsigned int value); // set led_number (0-7) to value (0=off 1=on higher=inverse) 
void e_led_clear(void);
void e_set_body_led(unsigned int value); // value (0=off 1=on higher=inverse) 
void e_set_front_led(unsigned int value); //value (0=off 1=on higher=inverse) 

// Various.
int getselector(void);

// UARTs handling.
// UART1 was used for BT communication.
// UART2 was used for serial communication with the Gumstix extension.
/*! \brief Init uart 1 at 115200bps, 8 data bits, 1 stop bit, Enable ISR for RX and TX */
void e_init_uart1(void);

/*! \brief Check if something is comming on uart 1
 * \return the number of characters available, 0 if none are available */
int  e_ischar_uart1(void);

/*! \brief If available, read 1 char and put it in pointer
 * \param car The pointer where the caracter will be stored if available
 * \return 1 if a char has been readed, 0 if no char is available
 */
int  e_getchar_uart1(char *car);

/*! \brief Send a buffer of char of size length
 * \param buff The top of the array where the data are stored
 * \param length The length of the array to send
 */
void e_send_uart1_char(const char * buff, int length);

/*! \brief  To check if the sending operation is done
 * \return 1 if buffer sending is in progress, return 0 if not
 */
int  e_uart1_sending(void);


/*! \brief Init uart 2 at 115200bps, 8 data bits, 1 stop bit, Enable ISR for RX and TX */
void e_init_uart2(int baud);

/*! \brief Check if something is comming on uart 2
 * \return the number of characters available, 0 if none are available */
int  e_ischar_uart2(void);

/*! \brief If available, read 1 char and put it in pointer
 * \param car The pointer where the caracter will be stored if available
 * \return 1 if a char has been readed, 0 if no char is available
 */
int  e_getchar_uart2(char *car);

/*! \brief Send a buffer of char of size length
 * \param buff The top of the array where the datas are stored
 * \param length The length of the array
 */
void e_send_uart2_char(const char * buff, int length);

/*! \brief  To check if the sending operation is done
 * \return 1 if buffer sending is in progress, return 0 if not
 */
int  e_uart2_sending(void);

// Motors handling.
void e_init_motors(void); 				// init to be done before using the other calls
void e_set_speed_left(int motor_speed);  // motor speed: from -1000 to 1000
void e_set_speed_right(int motor_speed); // motor speed: from -1000 to 1000
void e_set_speed(int linear_speed, int angular_speed);
void e_set_steps_left(int steps_left);
void e_set_steps_right(int steps_right);
int e_get_steps_left(void);
int e_get_steps_right(void);

// Proximity handling.
void e_calibrate_ir(void);
int e_get_prox(unsigned int sensor_number); // to get a prox value
int e_get_calibrated_prox(unsigned int sensor_number);
int e_get_ambient_light(unsigned int sensor_number); // to get ambient light value

// Camera handling.
#define ARRAY_WIDTH 640
#define ARRAY_HEIGHT 480
#define GREY_SCALE_MODE		0
#define RGB_565_MODE		1
#define YUV_MODE			2
int e_poxxxx_init_cam(void);
int e_poxxxx_config_cam(unsigned int sensor_x1,unsigned int sensor_y1,
			 unsigned int sensor_width,unsigned int sensor_height,
			 unsigned int zoom_fact_width,unsigned int zoom_fact_height,
			 int color_mode);
void e_poxxxx_write_cam_registers(void);
void e_poxxxx_launch_capture(char * buf);
int e_poxxxx_is_img_ready(void);


#ifdef __cplusplus
}
#endif

#endif
