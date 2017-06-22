#include "po8030.h"
#include "ch.h"
#include "hal.h"
#include "usbcfg.h"
#include "chprintf.h"
#include "main.h"

i2cflags_t errors = 0;
static struct po8030_configuration po8030_conf;

/**********************************************************************/
static format_t currFormat = FORMAT_YCBYCR;
static subsampling_t currSubsamplingX = SUBSAMPLING_X1;
static subsampling_t currSubsamplingY = SUBSAMPLING_X1;
// Utility functions used with the shell.
void po8030_save_current_format(format_t fmt) {
	currFormat = fmt;
}

format_t po8030_get_saved_format(void) {
	return currFormat;
}

void po8030_save_current_subsampling(subsampling_t x, subsampling_t y) {
	currSubsamplingX = x;
	currSubsamplingY = y;
}

subsampling_t po8030_get_saved_subsampling_x(void) {
	return currSubsamplingX;
}

subsampling_t po8030_get_saved_subsampling_y(void) {
	return currSubsamplingY;
}
/**********************************************************************/

i2cflags_t get_last_i2c_error(void) {
    return errors;
}

int8_t read_reg(uint8_t addr, uint8_t reg, uint8_t *value) {
	systime_t timeout = MS2ST(4); // 4 ms
	uint8_t txbuf[1] = {reg};
	uint8_t rxbuf[1];
    I2CDriver * I2C = NULL;

    if(current_camera == CAMERA_1){
        I2C = &I2CD1;
    }else{
        I2C = &I2CD3;
    }
	i2cAcquireBus(I2C);
	msg_t status = i2cMasterTransmitTimeout(I2C, addr, txbuf, 1, rxbuf, 1, timeout);
	i2cReleaseBus(I2C);

	if (status != MSG_OK){
        errors = i2cGetErrors(I2C);
		return status;
	}

	*value = rxbuf[0];

    return MSG_OK;
}


int8_t write_reg(uint8_t addr, uint8_t reg, uint8_t value) {
	systime_t timeout = MS2ST(4); // 4 ms
	uint8_t txbuf[2] = {reg, value};
	uint8_t rxbuf[1];
    I2CDriver * I2C = NULL;
    
    if(current_camera == CAMERA_1){
        I2C = &I2CD1;
    }else{
        I2C = &I2CD3;
    }
	i2cAcquireBus(I2C);
	msg_t status = i2cMasterTransmitTimeout(I2C, addr, txbuf, 2, rxbuf, 0, timeout);
	i2cReleaseBus(I2C);

	if (status != MSG_OK){
        errors = i2cGetErrors(I2C);
		return status;
	}

    return MSG_OK;
}

void po8030_init(void) {

    //we don't want to have the cameras in standby
    //the PO8030d is in Hi-Z at startup
    palWritePad(GPIOD, GPIOD_StandBy_1, PAL_LOW);
    palWritePad(GPIOE, GPIOE_StandBy_2, PAL_LOW);
    /*
     * PWM configuration structure.
     * Cyclic callback enabled, channels 1 and 4 enabled without callbacks,
     * the active state is a logic one.
     */
    static const PWMConfig pwmcfg = {
        168000000,                                   /* 168MHz PWM timer frequency.  */
        8,                                      /* PWM frequency = 168/8 = 21MHz.    */
        NULL,
        {
         {PWM_OUTPUT_ACTIVE_HIGH, NULL},
         {PWM_OUTPUT_DISABLED, NULL},
         {PWM_OUTPUT_DISABLED, NULL},
         {PWM_OUTPUT_DISABLED, NULL}
        },
        /* HW dependent part.*/
        0,
        0
    };

    pwmStart(&PWMD1, &pwmcfg);
    //set duty cycle to 50% of 8 (second argument of pwmcfg)
    pwmEnableChannel(&PWMD1, 0, (pwmcnt_t) 4);

    //config for I2C1
    static const I2CConfig i2cfg1 = {
        OPMODE_I2C,
        400000,
        FAST_DUTY_CYCLE_2,
    };
    i2cStart(&I2CD1, &i2cfg1);

    //config for I2C3
    static const I2CConfig i2cfg3 = {
        OPMODE_I2C,
        400000,
        FAST_DUTY_CYCLE_2,
    };
    i2cStart(&I2CD3, &i2cfg3);

    // Keep reset pin low for at least 8 x MCLK cycles...100 ms is more than enough.
    palWritePad(GPIOE, GPIOE_Reset_1, PAL_HIGH);
    chThdSleepMilliseconds(10);
    palWritePad(GPIOE, GPIOE_Reset_1, PAL_LOW);
    chThdSleepMilliseconds(100);
    palWritePad(GPIOE, GPIOE_Reset_1, PAL_HIGH);
    //need to wait for the reset signal to go high
    chThdSleepMilliseconds(100);

    // Keep reset pin low for at least 8 x MCLK cycles...100 ms is more than enough.
    palWritePad(GPIOD, GPIOD_Reset_2, PAL_HIGH);
    chThdSleepMilliseconds(10);
    palWritePad(GPIOD, GPIOD_Reset_2, PAL_LOW);
    chThdSleepMilliseconds(100);
    palWritePad(GPIOD, GPIOD_Reset_2, PAL_HIGH);
    //need to wait for the reset signal to go high
    chThdSleepMilliseconds(100);

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
	
	po8030_conf.curr_format = fmt;	

    return MSG_OK;
}

int8_t po8030_set_vga(void) {
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
    if((err = write_reg(PO8030_ADDR, PO8030_REG_WINDOWX2_H, 0x02)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_WINDOWX2_L, 0x80)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_WINDOWY2_H, 0x01)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_WINDOWY2_L, 0xE0)) != MSG_OK) {
        return err;
    }
    // AE full window selection.
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_FWX1_H, 0x00)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_FWX1_L, 0x01)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_FWX2_H, 0x02)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_FWX2_L, 0x80)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_FWY1_H, 0x00)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_FWY1_L, 0x01)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_FWY2_H, 0x01)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_FWY2_L, 0xE0)) != MSG_OK) {
        return err;
    }
    // AE center window selection.
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_CWX1_H, 0x00)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_CWX1_L, 0xD6)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_CWX2_H, 0x01)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_CWX2_L, 0xAB)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_CWY1_H, 0x00)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_CWY1_L, 0xA1)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_CWY2_H, 0x01)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_CWY2_L, 0x40)) != MSG_OK) {
        return err;
    }

    if((err = po8030_set_bank(BANK_B)) != MSG_OK) {
        return err;
    }
    // Scale settings.
    if((err = write_reg(PO8030_ADDR, PO8030_REG_SCALE_X, 0x20)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_SCALE_Y, 0x20)) != MSG_OK) {
        return err;
    }

	po8030_conf.width = 640;
	po8030_conf.height = 480;
	po8030_conf.curr_subsampling_x = SUBSAMPLING_X1;
	po8030_conf.curr_subsampling_y = SUBSAMPLING_X1;
	
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
	
	po8030_conf.width = 320;
	po8030_conf.height = 240;
	po8030_conf.curr_subsampling_x = SUBSAMPLING_X1;
	po8030_conf.curr_subsampling_y = SUBSAMPLING_X1;

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

	po8030_conf.width = 160;
	po8030_conf.height = 120;
	po8030_conf.curr_subsampling_x = SUBSAMPLING_X1;
	po8030_conf.curr_subsampling_y = SUBSAMPLING_X1;
	
    return MSG_OK;
}

int8_t po8030_set_size(image_size_t imgsize) {
    if(imgsize == SIZE_VGA) {
        return po8030_set_vga();
    } else if(imgsize == SIZE_QVGA) {
        return po8030_set_qvga();
    } else if(imgsize == SIZE_QQVGA) {
        return po8030_set_qqvga();
    } else {
		return -1;
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
    if((err = write_reg(PO8030_ADDR, PO8030_REG_PAD_CONTROL, PO8030_HI_Z_MODE)) != MSG_OK) {
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

int8_t po8030_advanced_config(format_t fmt, unsigned int x1, unsigned int y1, unsigned int width, unsigned int height, subsampling_t subsampling_x, subsampling_t subsampling_y) {
    int8_t err = 0;
	unsigned int x2 = x1 + width - 1;
	unsigned int y2 = y1 + height - 1;
	unsigned int auto_cw_x1 = 0, auto_cw_x2 = 0;
	unsigned int auto_cw_y1 = 0, auto_cw_y2 = 0;
	unsigned int scale_th;
	float scale_th_f;
	
	if(x1==0 || x1>PO8030_MAX_WIDTH) {
		return -2;
	}
	
	if(y1==0 || y1>PO8030_MAX_HEIGHT) {
		return -3;
	}
	
	if(x2<=x1 || x2>(PO8030_MAX_WIDTH)) {
		return -4;
	}
	
	if(y2<=y1 || y2>(PO8030_MAX_HEIGHT)) {
		return -5;
	}	
	
	switch(subsampling_x) {
		case SUBSAMPLING_X1:
			auto_cw_x1 = x1 + (width/3);
			auto_cw_x2 = x1 + (width*2/3);
			break;
			
		case SUBSAMPLING_X2:
			// Check if width is a multiple of the sub-sampling factor.
			if(width%2) {
				return -6;
			} else {
				x1 = x1/2 + 1;
				x2 = x1 + (width/2) - 1;
				auto_cw_x1 = x1 + (width/6);
				auto_cw_x2 = x1 + (width*2/6);
			}
			break;
			
		case SUBSAMPLING_X4:
			// Check if width is a multiple of the sub-sampling factor.
			if(width%4) {
				return -6;
			} else {
				x1 = x1/4 + 1;
				x2 = x1 + (width/4) - 1;
				auto_cw_x1 = x1 + (width/12);
				auto_cw_x2 = x1 + (width*2/12);
			}
			break;
	}
	
	switch(subsampling_y) {
		case SUBSAMPLING_X1:
			auto_cw_y1 = y1 + (height/3);
			auto_cw_y2 = y1 + (height*2/3);
			break;
			
		case SUBSAMPLING_X2:
			// Check if width is a multiple of the sub-sampling factor.
			if(height%2) {
				return -7;
			} else {
				y1 = y1/2 + 1;
				y2 = y1 + (height/2) - 1;
				auto_cw_y1 = y1 + (height/6);
				auto_cw_y2 = y1 + (height*2/6);
			}
			break;
			
		case SUBSAMPLING_X4:
			// Check if width is a multiple of the sub-sampling factor.
			if(height%4) {
				return -7;
			} else {
				y1 = y1/4 + 1;
				y2 = y1 + (height/4) - 1;
				auto_cw_y1 = y1 + (height/12);
				auto_cw_y2 = y1 + (height*2/12);
			}
			break;
	}
	
    if((err = po8030_set_bank(BANK_A)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_PAD_CONTROL, PO8030_HI_Z_MODE)) != MSG_OK) {
        return err;
    }

    if((err = po8030_set_format(fmt)) != MSG_OK) {
        return err;
    }
	
    if((err = po8030_set_bank(BANK_A)) != MSG_OK) {
        return err;
    }
    // Window settings.
    if((err = write_reg(PO8030_ADDR, PO8030_REG_WINDOWX1_H, (x1>>8))) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_WINDOWX1_L, (x1&0xFF))) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_WINDOWY1_H, (y1>>8))) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_WINDOWY1_L, (y1&0xFF))) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_WINDOWX2_H, (x2>>8))) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_WINDOWX2_L, (x2&0xFF))) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_WINDOWY2_H, (y2>>8))) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_WINDOWY2_L, (y2&0xFF))) != MSG_OK) {
        return err;
    }
    // AE full window selection.
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_FWX1_H, (x1>>8))) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_FWX1_L, (x1&0xFF))) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_FWX2_H, (x2>>8))) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_FWX2_L, (x2&0xFF))) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_FWY1_H, (y1>>8))) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_FWY1_L, (y1&0xFF))) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_FWY2_H, (y2>>8))) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_FWY2_L, (y2&0xFF))) != MSG_OK) {
        return err;
    }
    // AE center window selection.
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_CWX1_H, (auto_cw_x1>>8))) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_CWX1_L, (auto_cw_x1&0xFF))) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_CWX2_H, (auto_cw_x2>>8))) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_CWX2_L, (auto_cw_x2&0xFF))) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_CWY1_H, (auto_cw_y1>>8))) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_CWY1_L, (auto_cw_y1&0xFF))) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_CWY2_H, (auto_cw_y2>>8))) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_AUTO_CWY2_L, (auto_cw_y2&0xFF))) != MSG_OK) {
        return err;
    }

    if((err = po8030_set_bank(BANK_B)) != MSG_OK) {
        return err;
    }
    // Scale settings.
    if((err = write_reg(PO8030_ADDR, PO8030_REG_SCALE_X, subsampling_x)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_SCALE_Y, subsampling_y)) != MSG_OK) {
        return err;
    }	
	
	// Set scale buffer.
    if((err = po8030_set_bank(BANK_B)) != MSG_OK) {
        return err;
    }

    if(fmt == FORMAT_YYYY) {
		scale_th_f = (648.0-(float)(x2-x1))*((float)(x2-x1)+8.0)/(656.0);
		scale_th = (unsigned int)scale_th_f;
	} else {
		scale_th_f = ((648.0-(float)(x2-x1))*2.0)*((float)(x2-x1)*2.0+8.0)/(1304.0);
		scale_th = (unsigned int)scale_th_f;
	}
	
	if((err = write_reg(PO8030_ADDR, PO8030_REG_SCALE_TH_H, (scale_th>>8))) != MSG_OK) {
		return err;
	}
	if((err = write_reg(PO8030_ADDR, PO8030_REG_SCALE_TH_L, (scale_th&0xFF))) != MSG_OK) {
		return err;
	}
	
	po8030_conf.width = x2 - x1 + 1;
	po8030_conf.height = y2 - y1 + 1;
	po8030_conf.curr_format = fmt;
	po8030_conf.curr_subsampling_x = subsampling_x;
	po8030_conf.curr_subsampling_y = subsampling_y;
	
    return MSG_OK;
}


/*! Set brightness.
 * \param value Brightness => [7]:[6:0] = Sign:Magnitude: luminance = Y*contrast + brightness. Default=0, max=127, min=-128.
 */
int8_t po8030_set_brightness(uint8_t value) {
	int8_t err = 0;
    if((err = po8030_set_bank(BANK_B)) != MSG_OK) {
        return err;
    }
	return write_reg(PO8030_ADDR, PO8030_REG_BRIGHTNESS, value);
}

/*! Set contrast.
 * \param value Contrast => [7:0] = Magnitude: luminance = Y*contrast + brightness. Default=64, max=255, min=0.
 */
int8_t po8030_set_contrast(uint8_t value) {
	int8_t err = 0;
    if((err = po8030_set_bank(BANK_B)) != MSG_OK) {
        return err;
    }
	return write_reg(PO8030_ADDR, PO8030_REG_CONTRAST, value);
}

/*! Set mirroring for both vertical and horizontal orientations.
 * \param vertical: 1 to enable vertical mirroring
 * \param horizontal: 1 to enable horizontal mirroring
 */
int8_t po8030_set_mirror(uint8_t vertical, uint8_t horizontal) {
	int8_t err = 0;
	uint8_t value = 0;
	
    if((err = po8030_set_bank(BANK_A)) != MSG_OK) {
        return err;
    }
	
	if(vertical == 1) {
		value |= 0x80;
	}
	if(horizontal == 1) {
		value |= 0x40;
	}
	
	return write_reg(PO8030_ADDR, PO8030_REG_BAYER_CONTROL_01, value);
}

/*! Enable/disable auto white balance.
 * \param awb: 1 to enable auto white balance.
 */
int8_t po8030_set_awb(uint8_t awb) {
	int8_t err = 0;
	uint8_t value = 0;
	
    if((err = po8030_set_bank(BANK_C)) != MSG_OK) {
        return err;
    }
	
	if((err = read_reg(PO8030_ADDR, PO8030_REG_AUTO_CONTROL_1, &value)) != MSG_OK) {
        return err;
    }
	
	if(awb == 1) {
		value &= ~0x04;
	} else {
		value |= 0x04;
	}
	
	return write_reg(PO8030_ADDR, PO8030_REG_AUTO_CONTROL_1, value);
}

/*! Set white balance red, green, blue gain. 
 *	These values are considered only when auto white balance is disabled, so this function also disables auto white balance.
 * \param r: red gain: bit7=x2, bit6=x1, bit5=1/2, bit4=1/4, bit3=1/8, bit2=1/16, bit1=1/32, bit0=1/64. Default is 0x5E.
 * \param g: green gain: bit7=x2, bit6=x1, bit5=1/2, bit4=1/4, bit3=1/8, bit2=1/16, bit1=1/32, bit0=1/64. Default is 0x40.
 * \param b: blue gain: bit7=x2, bit6=x1, bit5=1/2, bit4=1/4, bit3=1/8, bit2=1/16, bit1=1/32, bit0=1/64. Default is 0x5D.
 */
int8_t po8030_set_rgb_gain(uint8_t r, uint8_t g, uint8_t b) {
	int8_t err = 0;
	
	if((err = po8030_set_awb(0)) != MSG_OK) {
        return err;
    }
	
    if((err = po8030_set_bank(BANK_A)) != MSG_OK) {
        return err;
    }
	
    if((err = write_reg(PO8030_ADDR, PO8030_REG_WB_RGAIN, r)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_WB_GGAIN, g)) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_WB_BGAIN, b)) != MSG_OK) {
        return err;
    }	

	return MSG_OK;
}

/*! Enable/disable auto exposure.
 * \param ae: 1 to enable auto exposure.
 */
int8_t po8030_set_ae(uint8_t ae) {
	int8_t err = 0;
	uint8_t value = 0;
	
    if((err = po8030_set_bank(BANK_C)) != MSG_OK) {
        return err;
    }
	
	if((err = read_reg(PO8030_ADDR, PO8030_REG_AUTO_CONTROL_1, &value)) != MSG_OK) {
        return err;
    }
	
	if(ae == 1) {
		value &= ~0x03;
	} else {
		value |= 0x03;
	}
	
	return write_reg(PO8030_ADDR, PO8030_REG_AUTO_CONTROL_1, value);
}

/*!	Set integration time. Total integration time is: (integral + fractional/256) line time. 
 *	These values are considered only when auto exposure is disabled, so this function also disables auto exposure.
 * \param integral: unit is line time. Default is 0x0080 (128).
 * \param fractional: unit is 1/256 line time. Default is 0x00 (0).
 */
int8_t po8030_set_exposure(uint16_t integral, uint8_t fractional) {
	int8_t err = 0;
	
	if((err = po8030_set_ae(0)) != MSG_OK) {
        return err;
    }
	
    if((err = po8030_set_bank(BANK_A)) != MSG_OK) {
        return err;
    }
	
    if((err = write_reg(PO8030_ADDR, PO8030_REG_INTTIME_H, (integral>>8))) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_INTTIME_M, (integral&0xFF))) != MSG_OK) {
        return err;
    }
    if((err = write_reg(PO8030_ADDR, PO8030_REG_INTTIME_L, fractional)) != MSG_OK) {
        return err;
    }
	
	return MSG_OK;
}

/*!	Return the current image size in bytes.
 */
uint32_t po8030_get_image_size(void) {
	if(po8030_conf.curr_format == FORMAT_YYYY) {
		return (uint32_t)po8030_conf.width * (uint32_t)po8030_conf.height;
	} else {
		return (uint32_t)po8030_conf.width * (uint32_t)po8030_conf.height * 2;
	}
}

void select_camera(uint8_t camera){
    if(camera == CAMERA_1){

        current_camera = CAMERA_2;
        po8030_set_bank(BANK_A);
        write_reg(PO8030_ADDR, PO8030_REG_PAD_CONTROL, O8030_HI_Z_ACTIVATED);
        //wait for the camera to enter into HI-Z mode
        chThdSleepMilliseconds(10);
        current_camera = CAMERA_1;
        po8030_set_bank(BANK_A);
        write_reg(PO8030_ADDR, PO8030_REG_PAD_CONTROL, PO8030_HI_Z_DESACTIVATED);
    }else if(camera == CAMERA_2){

        current_camera = CAMERA_1;
        po8030_set_bank(BANK_A);
        write_reg(PO8030_ADDR, PO8030_REG_PAD_CONTROL, PO8030_HI_Z_ACTIVATED);
        //wait for the camera to enter into HI-Z mode
        chThdSleepMilliseconds(10);
        current_camera = CAMERA_2;
        po8030_set_bank(BANK_A);
        write_reg(PO8030_ADDR, PO8030_REG_PAD_CONTROL, PO8030_HI_Z_DESACTIVATED);

    }
}

void toggle_camera(void){
    if(current_camera == CAMERA_1){
        select_camera(CAMERA_2);
    }else if(current_camera == CAMERA_2){
        select_camera(CAMERA_1);
    }
}



