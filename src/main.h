#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "parameter/parameter.h"
#include "msgbus/messagebus.h"

#define PROXIMITY_NB_CHANNELS 8
/** Struct containing a proximity measurment message. */
typedef struct {
    /** Ambient light level (LED is OFF). */
    unsigned int ambient[PROXIMITY_NB_CHANNELS];

    /** Reflected light level (LED is ON). */
    unsigned int reflected[PROXIMITY_NB_CHANNELS];

    /** Difference between ambient and reflected. */
    unsigned int delta[PROXIMITY_NB_CHANNELS];
} proximity_msg_t;

/** Robot wide IPC bus. */
extern messagebus_t bus;
extern proximity_msg_t proxMsg;

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
