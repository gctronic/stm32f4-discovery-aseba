#include "../inc/vl53l0x_i2c_platform.h"
#include "../../core/inc/vl53l0x_def.h"
#include "hal.h"

//#define I2C_DEBUG

int32_t VL53L0X_write_multi(uint8_t address, uint8_t index, uint8_t *pdata, uint32_t count) {

  VL53L0X_Error status = VL53L0X_ERROR_NONE;
  msg_t rdymsg = MSG_OK;

  uint8_t txbuff[count+1];
  uint8_t rxbuff[0];

  txbuff[0]=index;

  while(count>0){
    txbuff[count] = pdata[count-1];
    count--;
  }
  i2cAcquireBus(&I2CD1);
  rdymsg = i2cMasterTransmit(&I2CD1, address>>1, txbuff, count+1, rxbuff, 0);
  i2cReleaseBus(&I2CD1);

  switch(rdymsg){
    case MSG_OK :
      status = VL53L0X_ERROR_NONE;
      break;

    default:
      status = VL53L0X_ERROR_CONTROL_INTERFACE;
      break;
  }

  return status;
}

int32_t VL53L0X_read_multi(uint8_t address, uint8_t index, uint8_t *pdata, uint32_t count) {

  VL53L0X_Error status = VL53L0X_ERROR_NONE;
  msg_t rdymsg = MSG_OK;

  uint8_t txbuff[1] = {index};

  i2cAcquireBus(&I2CD1);
  rdymsg = i2cMasterTransmit(&I2CD1, address>>1, txbuff, 1, pdata, count);
  i2cReleaseBus(&I2CD1);

  switch(rdymsg){
    case MSG_OK :
      status = VL53L0X_ERROR_NONE;
      break;

    default:
      status = VL53L0X_ERROR_CONTROL_INTERFACE;
      break;
  }

  return status;
}

int32_t VL53L0X_write_byte(uint8_t address, uint8_t index, uint8_t data) {
  return VL53L0X_write_multi(address, index, &data, 1);
}

int32_t VL53L0X_write_word(uint8_t address, uint8_t index, uint16_t data) {
  uint8_t buff[2];
  buff[1] = data & 0xFF;
  buff[0] = data >> 8;
  return VL53L0X_write_multi(address, index, buff, 2);
}

int32_t VL53L0X_write_dword(uint8_t address, uint8_t index, uint32_t data) {
  uint8_t buff[4];

  buff[3] = data & 0xFF;
  buff[2] = data >> 8;
  buff[1] = data >> 16;
  buff[0] = data >> 24;

  return VL53L0X_write_multi(address, index, buff, 4);
}

int32_t VL53L0X_read_byte(uint8_t address, uint8_t index, uint8_t *data) {
  return VL53L0X_read_multi(address, index, data, 1);
}

int32_t VL53L0X_read_word(uint8_t address, uint8_t index, uint16_t *data) {
  uint8_t buff[2];
  int r = VL53L0X_read_multi(address, index, buff, 2);

  uint16_t tmp;
  tmp = buff[0];
  tmp <<= 8;
  tmp |= buff[1];
  *data = tmp;

  return r;
}

int32_t VL53L0X_read_dword(uint8_t address, uint8_t index, uint32_t *data) {
  uint8_t buff[4];
  int r = VL53L0X_read_multi(address, index, buff, 4);

  uint32_t tmp;
  tmp = buff[0];
  tmp <<= 8;
  tmp |= buff[1];
  tmp <<= 8;
  tmp |= buff[2];
  tmp <<= 8;
  tmp |= buff[3];

  *data = tmp;

  return r;
}
