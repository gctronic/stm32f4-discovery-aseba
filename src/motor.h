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
    GPTDriver *timer;
};

extern struct stepper_motor_s right_motor;
extern struct stepper_motor_s left_motor;

/** Set motor speed in steps per second. */
void motor_set_speed(struct stepper_motor_s *m, int speed);

/** Read motor position counter */
uint32_t motor_get_pos(struct stepper_motor_s *m);

/** Initialize motors */
void motors_init(void);

#endif /* MOTOR_H */
