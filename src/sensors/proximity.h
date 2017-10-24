#ifndef PROXIMITY_H
#define PROXIMITY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define PROXIMITY_NB_CHANNELS 8

/** Struct containing a proximity measurment message. */
//typedef struct {
//    /** Ambient light level (LED is OFF). */
//    unsigned int ambient[PROXIMITY_NB_CHANNELS];
//
//    /** Reflected light level (LED is ON). */
//    unsigned int reflected[PROXIMITY_NB_CHANNELS];
//
//    /** Difference between ambient and reflected. */
//    unsigned int delta[PROXIMITY_NB_CHANNELS];
//} proximity_msg_t;


void proximity_start(void);
void getProx0(uint16_t *ambient, uint16_t *reflected, uint16_t *delta);

#ifdef __cplusplus
}
#endif

#endif /* PROXIMITY_H */
