#include <ch.h>
#include <hal.h>
#include "main.h"
#include "usbcfg.h"
#include "chprintf.h"

//#include "exti.h"
#include "sensors/mpu9250.h"
#include "imu.h"

#define IMU_INTERRUPT_EVENT 1

/** Inits all the IMU peripherals. */
/*
static void imu_low_level_init(mpu60X0_t *mpu)
{
    mpu60X0_init_using_spi(mpu, &SPID1);

    while (!mpu60X0_ping(mpu)) {
        chThdSleepMilliseconds(10);
    }

    mpu60X0_setup(mpu, MPU60X0_ACC_FULL_RANGE_2G
                  | MPU60X0_GYRO_FULL_RANGE_250DPS
                  | MPU60X0_SAMPLE_RATE_DIV(100)
                  | MPU60X0_LOW_PASS_FILTER_6);
				  
}
*/

static THD_FUNCTION(imu_reader_thd, arg) {
     (void) arg;
     chRegSetThreadName(__FUNCTION__);

    // event_listener_t imu_int;
    // mpu60X0_t dev;

    // /* Iniits the hardware. */
    // imu_low_level_init(&dev);

    // /* Starts waiting for the external interrupts. */
    // chEvtRegisterMaskWithFlags(&exti_events, &imu_int,
                               // (eventmask_t)IMU_INTERRUPT_EVENT,
                               // (eventflags_t)EXTI_EVENT_MPU6000_INT);

     imu_msg_t imuMsgTopic;

     // Declares the topic on the bus.
     messagebus_topic_t imu_topic;
     MUTEX_DECL(imu_topic_lock);
     CONDVAR_DECL(imu_topic_condvar);
     messagebus_topic_init(&imu_topic, &imu_topic_lock, &imu_topic_condvar, &imuMsgTopic, sizeof(imuMsgTopic));
     messagebus_advertise_topic(&bus, &imu_topic, "/imu");

     while (true) {

//         /* Wait for a measurement to come. */
//         chEvtWaitAny(IMU_INTERRUPT_EVENT);

//         /* Read the incoming measurement. */
//         mpu60X0_read(&dev, msg.roll_rate, msg.acceleration, NULL);

    	 mpu9250_read_gyro_raw((int16_t *)&imuMsgTopic.gyro_raw);
    	 mpu9250_read_acc_raw((int16_t *)&imuMsgTopic.acc_raw);

         /* Publish it on the bus. */
         messagebus_topic_publish(&imu_topic, &imuMsgTopic, sizeof(imuMsgTopic));

         chThdSleepMilliseconds(50);

     }
}

void imu_start(void)
{
    static THD_WORKING_AREA(imu_reader_thd_wa, 1024);
    chThdCreateStatic(imu_reader_thd_wa, sizeof(imu_reader_thd_wa), NORMALPRIO, imu_reader_thd, NULL);

	//uint8_t imu_id = 0;
	//mpu9250_read_id(&imu_id);
	//chprintf((BaseSequentialStream *)&SDU1, "imu id=%X\r\n", imu_id);
	
    //mpu9250_setup(MPU60X0_ACC_FULL_RANGE_2G
    //              | MPU60X0_GYRO_FULL_RANGE_250DPS
    //              | MPU60X0_SAMPLE_RATE_DIV(100)
    //              | MPU60X0_LOW_PASS_FILTER_6);	
	


}
