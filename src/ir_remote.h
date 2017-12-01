#ifndef IR_REMOTE_H
#define IR_REMOTE_H

#include <stdint.h>
#include <hal.h>

void ir_remote_start(void);

/**
 * \brief Return the last toggle bit.
 * \return toggle toggle bit of the signal
 */
uint8_t ir_remote_get_toggle(void);

/**
 * \brief Return the address of the last command.
 * \return address address part of the signal
 */
uint8_t ir_remote_get_address(void);

/**
 * \brief Return the data of the last command.
 * \return data_ir data part of the signal
 */
uint8_t ir_remote_get_data(void);

#endif /* IR_REMOTE_H */
