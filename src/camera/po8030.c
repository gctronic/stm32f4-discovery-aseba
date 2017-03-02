#include "po8030.h"

#include "ch.h"
#include "hal.h"
#include "usbcfg.h"
#include "chprintf.h"

#define PO8030_ADDR 0x6E

i2cflags_t errors = 0;

int32_t read_reg(uint8_t addr, uint8_t reg, uint8_t *value) {
	systime_t timeout = MS2ST(4); // 4 ms
	uint8_t txbuf[1] = {reg};
	uint8_t rxbuf[1];

	i2cAcquireBus(&I2CD1);
	msg_t status = i2cMasterTransmitTimeout(&I2CD1, addr, txbuf, 1, rxbuf, 1, timeout);
	i2cReleaseBus(&I2CD1);

	if (status != MSG_OK){
        errors = i2cGetErrors(&I2CD1);
		return -1;
	}

	*value = rxbuf[0];

	return 0;
}


int32_t write_reg(uint8_t addr, uint8_t reg, uint8_t value) {
	systime_t timeout = MS2ST(4); // 4 ms
	uint8_t txbuf[2] = {reg, value};
	uint8_t rxbuf[1];

	i2cAcquireBus(&I2CD1);
	msg_t status = i2cMasterTransmitTimeout(&I2CD1, addr, txbuf, 2, rxbuf, 0, timeout);
	i2cReleaseBus(&I2CD1);

	if (status != MSG_OK){
        errors = i2cGetErrors(&I2CD1);
		return -1;
	}

	return 0;
}

void po8030_init(void) {

    uint8_t regValue[2] = {0};

    if(read_reg(0x4A, 0x01, &regValue[0]) < 0) { // DAC reg id.
//        chprintf((BaseSequentialStream *)&SDU1, "tx error = %d\r\n", errors);
    } else {
//        chprintf((BaseSequentialStream *)&SDU1, "DAC ID = %X\r\n", regValue[0]);
    }

    if(write_reg(PO8030_ADDR, REG_BANK, BANK_A) < 0) {
        chprintf((BaseSequentialStream *)&SDU1, "REG_BANK error = %d\r\n", errors);
    }
    if(write_reg(PO8030_ADDR, PO8030_REG_PAD_CONTROL, 0x00) < 0) {
        chprintf((BaseSequentialStream *)&SDU1, "REG_PAD_CONTROL error = %d\r\n", errors);
    }

    if(read_reg(PO8030_ADDR, REG_DEVICE_ID_H, &regValue[0]) < 0) {
        chprintf((BaseSequentialStream *)&SDU1, "tx error = %d\r\n", errors);
    }
    if(read_reg(PO8030_ADDR, REG_DEVICE_ID_L, &regValue[1]) < 0) {
        chprintf((BaseSequentialStream *)&SDU1, "tx error = %d\r\n", errors);
    }
//    chprintf((BaseSequentialStream *)&SDU1, "CAM ID = %X %X\r\n", regValue[0], regValue[1]);

    // RGB565 format setup.
//    chprintf((BaseSequentialStream *)&SDU1, "Setting color format...\r\n");
    if(write_reg(PO8030_ADDR, REG_BANK, BANK_B) < 0) {
        chprintf((BaseSequentialStream *)&SDU1, "REG_BANK error = %d\r\n", errors);
    }
//    if(write_reg(PO8030_ADDR, PO8030_REG_FORMAT, 0x00) < 0) { // cbycry
//        chprintf((BaseSequentialStream *)&SDU1, "REG_FORMAT error = %d\r\n", errors);
//    }
//    if(write_reg(PO8030_ADDR, PO8030_REG_FORMAT, 0x01) < 0) { // crycby
//        chprintf((BaseSequentialStream *)&SDU1, "REG_FORMAT error = %d\r\n", errors);
//    }
    if(write_reg(PO8030_ADDR, PO8030_REG_FORMAT, 0x02) < 0) { // ycbycr
        chprintf((BaseSequentialStream *)&SDU1, "REG_FORMAT error = %d\r\n", errors);
    }
//    if(write_reg(PO8030_ADDR, PO8030_REG_FORMAT, 0x03) < 0) { // ycrycb
//        chprintf((BaseSequentialStream *)&SDU1, "REG_FORMAT error = %d\r\n", errors);
//    }
//    if(write_reg(PO8030_ADDR, PO8030_REG_FORMAT, 0x30) < 0) { // rgb565
//        chprintf((BaseSequentialStream *)&SDU1, "REG_FORMAT error = %d\r\n", errors);
//    }
//    if(write_reg(PO8030_ADDR, PO8030_REG_FORMAT, 0x31) < 0) { // rgb565 (byte swap)
//        chprintf((BaseSequentialStream *)&SDU1, "REG_FORMAT error = %d\r\n", errors);
//    }
//    if(write_reg(PO8030_ADDR, PO8030_REG_FORMAT, 0x32) < 0) { // bgr565
//        chprintf((BaseSequentialStream *)&SDU1, "REG_FORMAT error = %d\r\n", errors);
//    }
//    if(write_reg(PO8030_ADDR, PO8030_REG_FORMAT, 0x44) < 0) { // yyyy
//        chprintf((BaseSequentialStream *)&SDU1, "REG_FORMAT error = %d\r\n", errors);
//    }
//    if(write_reg(PO8030_ADDR, PO8030_REG_SYNC_CONTROL0, 0x02) < 0) {
//        chprintf((BaseSequentialStream *)&SDU1, "REG_SYNC_CONTROL0 error = %d\r\n", errors);
//    }


    // QQVGA output image size setup.
//    chprintf((BaseSequentialStream *)&SDU1, "Setting QQVGA...\r\n");
    if(write_reg(PO8030_ADDR, REG_BANK, BANK_A) < 0) {
        chprintf((BaseSequentialStream *)&SDU1, "REG_BANK error = %d\r\n", errors);
    }
    if(write_reg(PO8030_ADDR, PO8030_REG_WINDOWX1_H, 0x00) < 0) {
        chprintf((BaseSequentialStream *)&SDU1, "REG_WINDOWX1_H error = %d\r\n", errors);
    }
    if(write_reg(PO8030_ADDR, PO8030_REG_WINDOWX1_L, 0x01) < 0) {
        chprintf((BaseSequentialStream *)&SDU1, "REG_WINDOWX1_L error = %d\r\n", errors);
    }
    if(write_reg(PO8030_ADDR, PO8030_REG_WINDOWY1_H, 0x00) < 0) {
        chprintf((BaseSequentialStream *)&SDU1, "REG_WINDOWY1_H error = %d\r\n", errors);
    }
    if(write_reg(PO8030_ADDR, PO8030_REG_WINDOWY1_L, 0x01) < 0) {
        chprintf((BaseSequentialStream *)&SDU1, "REG_WINDOWY1_L error = %d\r\n", errors);
    }
    if(write_reg(PO8030_ADDR, PO8030_REG_WINDOWX2_H, 0x00) < 0) {
        chprintf((BaseSequentialStream *)&SDU1, "REG_WINDOWX2_H error = %d\r\n", errors);
    }
    if(write_reg(PO8030_ADDR, PO8030_REG_WINDOWX2_L, 0xA0) < 0) {
        chprintf((BaseSequentialStream *)&SDU1, "REG_WINDOWX2_L error = %d\r\n", errors);
    }
    if(write_reg(PO8030_ADDR, PO8030_REG_WINDOWY2_H, 0x00) < 0) {
        chprintf((BaseSequentialStream *)&SDU1, "REG_WINDOWY2_H error = %d\r\n", errors);
    }
    if(write_reg(PO8030_ADDR, PO8030_REG_WINDOWY2_L, 0x78) < 0) {
        chprintf((BaseSequentialStream *)&SDU1, "REG_WINDOWY2_L error = %d\r\n", errors);
    }
    if(write_reg(PO8030_ADDR, PO8030_REG_AUTO_FWX1_H, 0x00) < 0) {
        chprintf((BaseSequentialStream *)&SDU1, "REG_AUTO_FWX1_H error = %d\r\n", errors);
    }
    if(write_reg(PO8030_ADDR, PO8030_REG_AUTO_FWX1_L, 0x01) < 0) {
        chprintf((BaseSequentialStream *)&SDU1, "REG_AUTO_FWX1_L error = %d\r\n", errors);
    }
    if(write_reg(PO8030_ADDR, PO8030_REG_AUTO_FWX2_H, 0x00) < 0) {
        chprintf((BaseSequentialStream *)&SDU1, "REG_AUTO_FWX2_H error = %d\r\n", errors);
    }
    if(write_reg(PO8030_ADDR, PO8030_REG_AUTO_FWX2_L, 0xA0) < 0) {
        chprintf((BaseSequentialStream *)&SDU1, "REG_AUTO_FWX2_L error = %d\r\n", errors);
    }
    if(write_reg(PO8030_ADDR, PO8030_REG_AUTO_FWY1_H, 0x00) < 0) {
        chprintf((BaseSequentialStream *)&SDU1, "REG_AUTO_FWY1_H error = %d\r\n", errors);
    }
    if(write_reg(PO8030_ADDR, PO8030_REG_AUTO_FWY1_L, 0x01) < 0) {
        chprintf((BaseSequentialStream *)&SDU1, "REG_AUTO_FWY1_L error = %d\r\n", errors);
    }
    if(write_reg(PO8030_ADDR, PO8030_REG_AUTO_FWY2_H, 0x00) < 0) {
        chprintf((BaseSequentialStream *)&SDU1, "REG_AUTO_FWY2_H error = %d\r\n", errors);
    }
    if(write_reg(PO8030_ADDR, PO8030_REG_AUTO_FWY2_L, 0x78) < 0) {
        chprintf((BaseSequentialStream *)&SDU1, "REG_AUTO_FWY2_L error = %d\r\n", errors);
    }
    if(write_reg(PO8030_ADDR, PO8030_REG_AUTO_CWX1_H, 0x00) < 0) {
        chprintf((BaseSequentialStream *)&SDU1, "REG_AUTO_CWX1_H error = %d\r\n", errors);
    }
    if(write_reg(PO8030_ADDR, PO8030_REG_AUTO_CWX1_L, 0x36) < 0) {
        chprintf((BaseSequentialStream *)&SDU1, "REG_AUTO_CWX1_L error = %d\r\n", errors);
    }
    if(write_reg(PO8030_ADDR, PO8030_REG_AUTO_CWX2_H, 0x00) < 0) {
        chprintf((BaseSequentialStream *)&SDU1, "REG_AUTO_CWX2_H error = %d\r\n", errors);
    }
    if(write_reg(PO8030_ADDR, PO8030_REG_AUTO_CWX2_L, 0x6B) < 0) {
        chprintf((BaseSequentialStream *)&SDU1, "REG_AUTO_CWX2_L error = %d\r\n", errors);
    }
    if(write_reg(PO8030_ADDR, PO8030_REG_AUTO_CWY1_H, 0x00) < 0) {
        chprintf((BaseSequentialStream *)&SDU1, "REG_AUTO_CWY1_H error = %d\r\n", errors);
    }
    if(write_reg(PO8030_ADDR, PO8030_REG_AUTO_CWY1_L, 0x29) < 0) {
        chprintf((BaseSequentialStream *)&SDU1, "REG_AUTO_CWY1_L error = %d\r\n", errors);
    }
    if(write_reg(PO8030_ADDR, PO8030_REG_AUTO_CWY2_H, 0x00) < 0) {
        chprintf((BaseSequentialStream *)&SDU1, "REG_AUTO_CWY2_H error = %d\r\n", errors);
    }
    if(write_reg(PO8030_ADDR, PO8030_REG_AUTO_CWY2_L, 0x50) < 0) {
        chprintf((BaseSequentialStream *)&SDU1, "REG_AUTO_CWY2_L error = %d\r\n", errors);
    }
    if(write_reg(PO8030_ADDR, REG_BANK, BANK_B) < 0) {
        chprintf((BaseSequentialStream *)&SDU1, "REG_BANK error = %d\r\n", errors);
    }
    if(write_reg(PO8030_ADDR, PO8030_REG_SCALE_X, 0x80) < 0) {
        chprintf((BaseSequentialStream *)&SDU1, "REG_SCALE_X error = %d\r\n", errors);
    }
    if(write_reg(PO8030_ADDR, PO8030_REG_SCALE_Y, 0x80) < 0) {
        chprintf((BaseSequentialStream *)&SDU1, "REG_SCALE_Y error = %d\r\n", errors);
    }
    if(write_reg(PO8030_ADDR, PO8030_REG_SCALE_TH_H, 0x00) < 0) {
        chprintf((BaseSequentialStream *)&SDU1, "REG_SCALE_TH_H error = %d\r\n", errors);
    }
    if(write_reg(PO8030_ADDR, PO8030_REG_SCALE_TH_L, 0xF5) < 0) {
        chprintf((BaseSequentialStream *)&SDU1, "REG_SCALE_TH_L error = %d\r\n", errors);
    }
    //if(write_reg(PO8030_ADDR, PO8030_REG_SYNC_CONTROL0, 0x03) < 0) {
    //    chprintf((BaseSequentialStream *)&SDU1, "REG_SYNC_CONTROL0 error = %d\r\n", errors);
    //}

//    // Turn on camera.
////    chprintf((BaseSequentialStream *)&SDU1, "Turning on camera...\r\n");
//    if(write_reg(PO8030_ADDR, REG_BANK, BANK_A) < 0) {
//        chprintf((BaseSequentialStream *)&SDU1, "REG_BANK error = %d\r\n", errors);
//    }
//    if(read_reg(PO8030_ADDR, PO8030_REG_PAD_CONTROL, &regValue[0]) < 0) {
//        chprintf((BaseSequentialStream *)&SDU1, "tx error = %d\r\n", errors);
//    } else {
////        chprintf((BaseSequentialStream *)&SDU1, "PAD_CONTROL = %X\r\n", regValue[0]);
//    }
//    regValue[0] &= 0x3F;
//    if(write_reg(PO8030_ADDR, PO8030_REG_PAD_CONTROL, regValue[0]) < 0) {
//        chprintf((BaseSequentialStream *)&SDU1, "REG_PAD_CONTROL error = %d\r\n", errors);
//    }
//    if(read_reg(PO8030_ADDR, PO8030_REG_PAD_CONTROL, &regValue[0]) < 0) {
//        chprintf((BaseSequentialStream *)&SDU1, "tx error = %d\r\n", errors);
//    } else {
////        chprintf((BaseSequentialStream *)&SDU1, "PAD_CONTROL = %X\r\n", regValue[0]);
//    }

}


