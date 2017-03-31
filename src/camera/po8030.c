#include "po8030.h"
#include "ch.h"
#include "usbcfg.h"
#include "chprintf.h"

i2cflags_t errors = 0;

i2cflags_t get_last_i2c_error(void) {
    return errors;
}

int8_t read_reg(uint8_t addr, uint8_t reg, uint8_t *value) {
	systime_t timeout = MS2ST(4); // 4 ms
	uint8_t txbuf[1] = {reg};
	uint8_t rxbuf[1];

	i2cAcquireBus(&I2CD1);
	msg_t status = i2cMasterTransmitTimeout(&I2CD1, addr, txbuf, 1, rxbuf, 1, timeout);
	i2cReleaseBus(&I2CD1);

	if (status != MSG_OK){
        errors = i2cGetErrors(&I2CD1);
		return status;
	}

	*value = rxbuf[0];

    return MSG_OK;
}


int8_t write_reg(uint8_t addr, uint8_t reg, uint8_t value) {
	systime_t timeout = MS2ST(4); // 4 ms
	uint8_t txbuf[2] = {reg, value};
	uint8_t rxbuf[1];

	i2cAcquireBus(&I2CD1);
	msg_t status = i2cMasterTransmitTimeout(&I2CD1, addr, txbuf, 2, rxbuf, 0, timeout);
	i2cReleaseBus(&I2CD1);

	if (status != MSG_OK){
        errors = i2cGetErrors(&I2CD1);
		return status;
	}

    return MSG_OK;
}

void po8030_init(void) {
    // Generate the master clock for the camera.
    palSetPadMode(GPIOA, 8, PAL_MODE_ALTERNATE(0)); // MCO1 output from HSI (16 MHz) => see "mcuconf.h".

    // Configure I2C communication.
    palSetPadMode(GPIOB, 8, PAL_MODE_ALTERNATE(4)); // I2C1 SCL.
    //palSetPadMode(GPIOB, 9, PAL_MODE_ALTERNATE(4)); // I2C1 SDA. Already setup in the board file (ChibiOS/os/hal/boards/ST_STM32F4_DISCOVERY/board.h).
    static const I2CConfig i2cfg1 = {
        OPMODE_I2C,
        400000,
        FAST_DUTY_CYCLE_2,
    };
    i2cStart(&I2CD1, &i2cfg1);

    // Keep reset pin low for at least 8 x MCLK cycles...100 ms is more than enough.
    palSetPadMode(GPIOC, 10, PAL_MODE_OUTPUT_PUSHPULL); // PO8030 RST pin.
    palWritePad(GPIOC, 10, PAL_HIGH);
    chThdSleepMilliseconds(10);
    palWritePad(GPIOC, 10, PAL_LOW);
    chThdSleepMilliseconds(100);
    palWritePad(GPIOC, 10, PAL_HIGH);

}

int8_t po8030_read_id(uint16_t *id) {
    uint8_t regValue[2] = {0};
    int8_t err = 0;

    if((err = read_reg(PO8030_ADDR, REG_DEVICE_ID_H, &regValue[0])) != MSG_OK) {
        return err;
    }
    if((err = read_reg(PO8030_ADDR, REG_DEVICE_ID_L, &regValue[1])) != MSG_OK) {
        return err;
    }
    *id = regValue[0]|regValue[1];

    return MSG_OK;
}

int8_t po8030_set_bank(uint8_t bank) {
    return write_reg(PO8030_ADDR, REG_BANK, bank);
}

int8_t po8030_set_format(format_t fmt) {
    int8_t err = 0;

    if((err = po8030_set_bank(BANK_B)) != MSG_OK) {
        return err;
    }

    if((err = write_reg(PO8030_ADDR, PO8030_REG_FORMAT, fmt)) != MSG_OK) {
        return err;
    }

    if(fmt == FORMAT_YYYY) {
        if((err = write_reg(PO8030_ADDR, PO8030_REG_SYNC_CONTROL0, 0x01)) != MSG_OK) {
            return err;
        }
        if((err = po8030_set_bank(BANK_A)) != MSG_OK) {
            return err;
        }
        if((err = write_reg(PO8030_ADDR, PO8030_REG_VSYNCSTARTROW_L, 0x03)) != MSG_OK) {
            return err;
        }
    } else {
        if((err = write_reg(PO8030_ADDR, PO8030_REG_SYNC_CONTROL0, 0x00)) != MSG_OK) {
            return err;
        }
        if((err = po8030_set_bank(BANK_A)) != MSG_OK) {
            return err;
        }
        if((err = write_reg(PO8030_ADDR, PO8030_REG_VSYNCSTARTROW_L, 0x0A)) != MSG_OK) {
            return err;
        }
    }

    return MSG_OK;
}

int8_t po8030_set_vga(void) {
    // To be implemented...
    return MSG_OK;
}

int8_t po8030_set_qvga(void) {
    int8_t err = 0;

    if((err = po8030_set_bank(BANK_A)) != MSG_OK) {
        return err;
    }
    // Window settings.
    if((err = write_reg(PO8030_ADDR, PO8030_REG_WINDOWX1_H, 0x00)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_WINDOWX1_L, 0x01)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_WINDOWY1_H, 0x00)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_WINDOWY1_L, 0x01)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_WINDOWX2_H, 0x01)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_WINDOWX2_L, 0x40)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_WINDOWY2_H, 0x00)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_WINDOWY2_L, 0xF0)) != MSG_OK) {
        return err;
    }
    // AE full window selection.
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_FWX1_H, 0x00)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_FWX1_L, 0x01)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_FWX2_H, 0x01)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_FWX2_L, 0x40)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_FWY1_H, 0x00)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_FWY1_L, 0x01)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_FWY2_H, 0x00)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_FWY2_L, 0xF0)) != MSG_OK) {
        return err;
    }
    // AE center window selection.
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_CWX1_H, 0x00)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_CWX1_L, 0x6B)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_CWX2_H, 0x00)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_CWX2_L, 0xD6)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_CWY1_H, 0x00)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_CWY1_L, 0x50)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_CWY2_H, 0x00)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_CWY2_L, 0xA0)) != MSG_OK) {
        return err;
    }

    if((err = po8030_set_bank(BANK_B)) != MSG_OK) {
        return err;
    }
    // Scale settings.
    if((err = write_reg(PO8030_ADDR, PO8030_REG_SCALE_X, 0x40)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_SCALE_Y, 0x40)) != MSG_OK) {
        return err;
    }

    return MSG_OK;
}

int8_t po8030_set_qqvga(void) {
    int8_t err = 0;

    if((err = po8030_set_bank(BANK_A)) != MSG_OK) {
        return err;
    }
    // Window settings.
    if((err = write_reg(PO8030_ADDR, PO8030_REG_WINDOWX1_H, 0x00)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_WINDOWX1_L, 0x01)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_WINDOWY1_H, 0x00)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_WINDOWY1_L, 0x01)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_WINDOWX2_H, 0x00)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_WINDOWX2_L, 0xA0)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_WINDOWY2_H, 0x00)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_WINDOWY2_L, 0x78)) != MSG_OK) {
        return err;
    }
    // AE full window selection.
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_FWX1_H, 0x00)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_FWX1_L, 0x01)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_FWX2_H, 0x00)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_FWX2_L, 0xA0)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_FWY1_H, 0x00)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_FWY1_L, 0x01)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_FWY2_H, 0x00)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_FWY2_L, 0x78)) != MSG_OK) {
        return err;
    }
    // AE center window selection.
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_CWX1_H, 0x00)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_CWX1_L, 0x36)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_CWX2_H, 0x00)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_CWX2_L, 0x6B)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_CWY1_H, 0x00)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_CWY1_L, 0x29)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_CWY2_H, 0x00)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_CWY2_L, 0x50)) != MSG_OK) {
        return err;
    }

    if((err = po8030_set_bank(BANK_B)) != MSG_OK) {
        return err;
    }
    // Scale settings.
    if((err = write_reg(PO8030_ADDR, PO8030_REG_SCALE_X, 0x80)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_SCALE_Y, 0x80)) != MSG_OK) {
        return err;
    }

    return MSG_OK;
}

int8_t po8030_set_size(image_size_t imgsize) {
    if(imgsize == SIZE_VGA) {
        return po8030_set_vga();
    } else if(imgsize == SIZE_QVGA) {
        return po8030_set_qvga();
    } else if(imgsize == SIZE_QQVGA) {
        return po8030_set_qqvga();
    }
}

int8_t po8030_set_scale_buffer_size(format_t fmt, image_size_t imgsize) {
    // Scale buffer size depends on both format and size
    int8_t err = 0;

    if((err = po8030_set_bank(BANK_B)) != MSG_OK) {
        return err;
    }

    if(fmt == FORMAT_YYYY) {
        switch(imgsize) {
            case SIZE_VGA:
                if((err = write_reg(PO8030_ADDR, PO8030_REG_SCALE_TH_H, 0x00)) != MSG_OK) {
                    return err;
                }
                if((err = write_reg(PO8030_ADDR, PO8030_REG_SCALE_TH_L, 0x08)) != MSG_OK) {
                    return err;
                }
                break;

            case SIZE_QVGA:
                if((err = write_reg(PO8030_ADDR, PO8030_REG_SCALE_TH_H, 0x00)) != MSG_OK) {
                    return err;
                }
                if((err = write_reg(PO8030_ADDR, PO8030_REG_SCALE_TH_L, 0xA4)) != MSG_OK) {
                    return err;
                }
                break;

            case SIZE_QQVGA:
                if((err = write_reg(PO8030_ADDR, PO8030_REG_SCALE_TH_H, 0x00)) != MSG_OK) {
                    return err;
                }
                if((err = write_reg(PO8030_ADDR, PO8030_REG_SCALE_TH_L, 0x7C)) != MSG_OK) {
                    return err;
                }
                break;
        }
    } else {
        switch(imgsize) {
            case SIZE_VGA:
                if((err = write_reg(PO8030_ADDR, PO8030_REG_SCALE_TH_H, 0x00)) != MSG_OK) {
                    return err;
                }
                if((err = write_reg(PO8030_ADDR, PO8030_REG_SCALE_TH_L, 0x0A)) != MSG_OK) {
                    return err;
                }
                break;

            case SIZE_QVGA: // To be tested...
                if((err = write_reg(PO8030_ADDR, PO8030_REG_SCALE_TH_H, 0x01)) != MSG_OK) {
                    return err;
                }
                if((err = write_reg(PO8030_ADDR, PO8030_REG_SCALE_TH_L, 0x46)) != MSG_OK) {
                    return err;
                }
                break;

            case SIZE_QQVGA:
                if((err = write_reg(PO8030_ADDR, PO8030_REG_SCALE_TH_H, 0x00)) != MSG_OK) {
                    return err;
                }
                if((err = write_reg(PO8030_ADDR, PO8030_REG_SCALE_TH_L, 0xF5)) != MSG_OK) {
                    return err;
                }
                break;
        }
    }

    return MSG_OK;
}

int8_t po8030_config(format_t fmt, image_size_t imgsize) {

    int8_t err = 0;

    if((err = po8030_set_bank(BANK_A)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_PAD_CONTROL, 0x00)) != MSG_OK) {
        return err;
    }

    if((err = po8030_set_format(fmt)) != MSG_OK) {
        return err;
    }

    if((err = po8030_set_size(imgsize)) != MSG_OK) {
        return err;
    }

    if((err = po8030_set_scale_buffer_size(fmt, imgsize)) != MSG_OK) {
        return err;
    }

    return MSG_OK;
}


