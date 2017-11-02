#ifndef EXTI_H
#define EXTI_H

#ifdef __cplusplus
extern "C" {
#endif

extern event_source_t exti_events;

/** Starts the external interrupt processing service. */
void exti_start(void);

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


#ifdef __cplusplus
}
#endif



#endif /* EXTI_H */
