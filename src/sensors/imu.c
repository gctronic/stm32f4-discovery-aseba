#include <ch.h>
#include <hal.h>
#include "main.h"
#include "usbcfg.h"
#include "chprintf.h"

//#include "exti.h"
#include "sensors/mpu9250.h"
#include "imu.h"
#include "../leds.h"

#define IMU_INTERRUPT_EVENT 1

imu_msg_t imuMsgTopic;
uint8_t accAxisFilteringInProgress = 0;
uint8_t accAxisFilteringState = 0;
uint8_t accAxisSelected = 0;
uint8_t filterSize = 0;
uint8_t accCalibrationInProgress = 0;

static THD_FUNCTION(imu_reader_thd, arg) {
     (void) arg;
     chRegSetThreadName(__FUNCTION__);

	 systime_t time;

    // event_listener_t imu_int;
    // mpu60X0_t dev;

    // /* Iniits the hardware. */
    // imu_low_level_init(&dev);

    // /* Starts waiting for the external interrupts. */
    // chEvtRegisterMaskWithFlags(&exti_events, &imu_int,
                               // (eventmask_t)IMU_INTERRUPT_EVENT,
                               // (eventflags_t)EXTI_EVENT_MPU6000_INT);

     // Declares the topic on the bus.
     messagebus_topic_t imu_topic;
     MUTEX_DECL(imu_topic_lock);
     CONDVAR_DECL(imu_topic_condvar);
     messagebus_topic_init(&imu_topic, &imu_topic_lock, &imu_topic_condvar, &imuMsgTopic, sizeof(imuMsgTopic));
     messagebus_advertise_topic(&bus, &imu_topic, "/imu");

     uint8_t calibrationNumSamples = 0;
     int32_t calibrationSum = 0;

     while (true) {

    	 time = chVTGetSystemTime();

//         /* Wait for a measurement to come. */
//         chEvtWaitAny(IMU_INTERRUPT_EVENT);

//         /* Read the incoming measurement. */
    	 mpu9250_read(imuMsgTopic.gyro, imuMsgTopic.acceleration, &imuMsgTopic.temperature, imuMsgTopic.gyro_raw, imuMsgTopic.acc_raw);

         /* Publish it on the bus. */
         messagebus_topic_publish(&imu_topic, &imuMsgTopic, sizeof(imuMsgTopic));

         if(accAxisFilteringInProgress) {
         	switch(accAxisFilteringState) {
 				case 0:
 					imuMsgTopic.acc_raw_offset[accAxisSelected] = 0;
 					calibrationSum = 0;
 					calibrationNumSamples = 0;
 					accAxisFilteringState = 1;
 					break;

 				case 1:
 					calibrationSum += imuMsgTopic.acc_raw[accAxisSelected];
 					calibrationNumSamples++;
 					if(calibrationNumSamples == filterSize) {
 						imuMsgTopic.acc_raw_filtered[accAxisSelected] = calibrationSum/filterSize;
 						accAxisFilteringInProgress = 0;
 						if(accCalibrationInProgress == 1) {
 							imuMsgTopic.acc_raw_offset[accAxisSelected] = imuMsgTopic.acc_raw_filtered[accAxisSelected];
 							accCalibrationInProgress = 0;
 						}
 					}
 					break;
         	}
         }

         chThdSleepUntilWindowed(time, time + MS2ST(20)); // Refresh @ 50 Hz.

     }
}

void imu_start(void)
{
    mpu9250_setup(MPU9250_ACC_FULL_RANGE_2G
                  | MPU9250_GYRO_FULL_RANGE_250DPS
                  | MPU9250_SAMPLE_RATE_DIV(100));
                  //| MPU60X0_LOW_PASS_FILTER_6);

    static THD_WORKING_AREA(imu_reader_thd_wa, 1024);
    chThdCreateStatic(imu_reader_thd_wa, sizeof(imu_reader_thd_wa), NORMALPRIO, imu_reader_thd, NULL);

	//uint8_t imu_id = 0;
	//mpu9250_read_id(&imu_id);
	//chprintf((BaseSequentialStream *)&SDU1, "imu id=%X\r\n", imu_id);
}

int16_t get_acc(uint8_t axis) {
	if(axis < 3) {
		return imuMsgTopic.acc_raw[axis];
	}
	return 0;
}

int16_t get_acc_filtered(uint8_t axis, uint8_t filter_size) {
	if(axis < 3) {
		accAxisFilteringState = 0;
		accAxisFilteringInProgress = 1;
		accAxisSelected = axis;
		filterSize = filter_size;
		while(accAxisFilteringInProgress) {
			chThdSleepMilliseconds(20);
		}
		return imuMsgTopic.acc_raw_filtered[axis];
	}
	return 0;
}

void calibrate_acc(void) {
	accCalibrationInProgress = 1;
	get_acc_filtered(0, 50);
	get_acc_filtered(1, 50);
	get_acc_filtered(2, 50);
	accCalibrationInProgress = 0;
}

int16_t get_acc_offset(uint8_t axis) {
	return imuMsgTopic.acc_raw_offset[axis];
}


