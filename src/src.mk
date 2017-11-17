
INCDIR += ./ChibiOS_ext/os/hal/include

INCDIR += ./ChibiOS_ext/os/hal/ports/STM32/STM32F4xx

INCDIR += ./fatfs/src/

INCDIR += src/


CSRC += ./ChibiOS_ext/os/hal/ports/STM32/STM32F4xx/dcmi_lld.c
CSRC += ./ChibiOS_ext/os/hal/src/dcmi.c
CSRC += ./fatfs/src/ff.c
CSRC += ./src/audio/audio_thread.c
CSRC += ./src/audio/microphone.c
CSRC += ./src/audio/mp45dt02_processing.c
CSRC += ./src/camera/dcmi_camera.c
CSRC += ./src/camera/po8030.c
CSRC += ./src/cmd.c
CSRC += ./src/epuck1x/Asercom.c
CSRC += ./src/epuck1x/epuck1x-wrapper.c
CSRC += ./src/exti.c
CSRC += ./src/fatfs_diskio.c
CSRC += ./src/fatfs_syscall.c
CSRC += ./src/flash/flash.c
CSRC += ./src/i2c_bus.c
CSRC += ./src/leds.c
CSRC += ./src/main.c
CSRC += ./src/memory_protection.c
CSRC += ./src/motor.c
CSRC += ./src/panic.c
CSRC += ./src/sdcard.c
CSRC += ./src/sensors/imu.c
CSRC += ./src/sensors/mpu9250.c
CSRC += ./src/sensors/proximity.c
CSRC += ./src/spi_comm.c
CSRC += ./src/usbcfg.c
CSRC += ./src/utility.c
CSRC += src/chibios-syscalls/malloc_lock.c
CSRC += src/chibios-syscalls/newlib_syscalls.c
CSRC += ./src/config_flash_storage.c
CSRC += ./src/msgbus/examples/chibios/port.c
CSRC += ./src/msgbus/messagebus.c
CSRC += ./src/sensors/VL53L0X/Api/core/src/vl53l0x_api.c
CSRC += ./src/sensors/VL53L0X/Api/core/src/vl53l0x_api_calibration.c
CSRC += ./src/sensors/VL53L0X/Api/core/src/vl53l0x_api_core.c
CSRC += ./src/sensors/VL53L0X/Api/core/src/vl53l0x_api_ranging.c
CSRC += ./src/sensors/VL53L0X/Api/core/src/vl53l0x_api_strings.c
CSRC += ./src/sensors/VL53L0X/Api/platform/src/vl53l0x_i2c_platform.c
CSRC += ./src/sensors/VL53L0X/Api/platform/src/vl53l0x_platform.c
CSRC += ./src/sensors/VL53L0X/VL53L0X.c
CSRC += src/cmp/cmp.c
CSRC += src/cmp_mem_access/cmp_mem_access.c
CSRC += src/crc/crc16.c
CSRC += src/crc/crc32.c
CSRC += src/parameter/parameter.c
CSRC += src/parameter/parameter_msgpack.c
CSRC += src/parameter/parameter_print.c
