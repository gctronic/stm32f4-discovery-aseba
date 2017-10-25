#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>
#include <hal.h>

struct stepper_motor_s {
    enum {
        HALT=0,
        FORWARD=1,
        BACKWARD=2
    } direction;
    uint8_t step_index;
    int32_t count;
    void (*update)(const uint8_t *out);
    void (*enable_power_save)(void);
    void (*disable_power_save)(void);
    PWMDriver *timer;
};

/** Set motor speed in steps per second. */
void left_motor_set_speed(int speed);
void right_motor_set_speed(int speed);

/** Read motor position counter */
uint32_t left_motor_get_pos(void);
uint32_t right_motor_get_pos(void);

/** Initialize motors */
void motors_init(void);

#endif /* MOTOR_H */
