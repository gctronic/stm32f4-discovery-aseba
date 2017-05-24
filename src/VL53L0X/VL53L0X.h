#ifndef VL53L0X_H
#define VL53L0X_H

#include "Api/core/inc/vl53l0x_api.h"


VL53L0X_Error VL53L0X_init(VL53L0X_Dev_t* device);

VL53L0X_Error VL53L0X_configAccuracy(VL53L0X_Dev_t* device, VL53L0X_AccuracyMode accuracy);

VL53L0X_Error VL53L0X_startMeasure(VL53L0X_Dev_t* device, VL53L0X_DeviceModes mode);

VL53L0X_Error VL53L0X_getLastMeasure(VL53L0X_Dev_t* device);

VL53L0X_Error VL53L0X_stopMeasure(VL53L0X_Dev_t* device);

#endif /* VL53L0X_H*/