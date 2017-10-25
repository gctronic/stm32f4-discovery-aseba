#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "parameter/parameter.h"
#include "msgbus/messagebus.h"

/** Robot wide IPC bus. */
extern messagebus_t bus;

extern parameter_namespace_t parameter_root;

#define MAX_BUFF_SIZE 76800 // Bytes.
#define CAPTURE_ONE_SHOT 0
#define CAPTURE_CONTINUOUS 1
extern const DCMIConfig dcmicfg;
extern uint8_t capture_mode;
extern uint8_t *sample_buffer;
extern uint8_t *sample_buffer2;
extern uint8_t double_buffering;
extern uint8_t txComplete;

#ifdef __cplusplus
}
#endif

#endif
