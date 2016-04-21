#ifndef UNIQUE_ID_H
#define UNIQUE_ID_H

#include <stdint.h>

#define UNIQUE_ID_SIZE 12

/** Reads the chip unique ID. */
void unique_id_read(uint8_t buffer[UNIQUE_ID_SIZE]);

#endif
