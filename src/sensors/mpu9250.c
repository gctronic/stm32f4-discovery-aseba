#include "mpu9250.h"
#include "../i2c_bus.h"

int8_t mpu9250_setup(int config) {
	int8_t err = 0;
	int8_t regValue = 0;
	
    // Reset device.
    if((err = write_reg(MPU9250_ADDRESS, PWR_MGMT_1, 0x80)) != MSG_OK) {
        return err;
    }	
    chThdSleepMilliseconds(1);
    while(1) {
		read_reg(MPU9250_ADDRESS, PWR_MGMT_1, &regValue);
		if(!(regValue & 0x80)) {
			break;
		}
        chThdSleepMilliseconds(1);
    }
	
    if((err = write_reg(MPU9250_ADDRESS, SMPLRT_DIV, 0x64)) != MSG_OK) {
        return err;
    }
	
}

int8_t mpu9250_read_id(uint8_t *id) {
	int8_t err = 0;
    if((err = read_reg(MPU9250_ADDRESS, WHO_AM_I_MPU9250, id)) != MSG_OK) {
        return err;
    }
    return MSG_OK;
}

bool mpu9250_self_test(void) {

}

void mpu9250_read(float *gyro, float *acc, float *temp) {

}

void mpu9250_read_gyro_raw(int16_t *data) {
	uint8_t buf[6];
    read_reg_multi(MPU9250_ADDRESS, GYRO_XOUT_H, buf, sizeof(buf));
	data[0] = (buf[0]<<8) + buf[1];
	data[1] = (buf[2]<<8) + buf[3];
	data[2] = (buf[4]<<8) + buf[5];
}

void mpu9250_read_acc_raw(int16_t *data) {
	uint8_t buf[6];
    read_reg_multi(MPU9250_ADDRESS, ACCEL_XOUT_H, buf, sizeof(buf));
	data[0] = (buf[0]<<8) + buf[1];
	data[1] = (buf[2]<<8) + buf[3];
	data[2] = (buf[4]<<8) + buf[5];
}



