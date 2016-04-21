#include <string.h>
#include "unique_id.h"

#define UNIQUE_ID_BASE ((void *)0x1FFF7A10)


void unique_id_read(uint8_t buffer[UNIQUE_ID_SIZE])
{
    memcpy(buffer, UNIQUE_ID_BASE, UNIQUE_ID_SIZE);
}
