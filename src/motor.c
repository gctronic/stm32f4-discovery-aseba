#include <ch.h>
#include <hal.h>
#include "motor.h"
#include "leds.h"

#define MOTOR_TIMER_FREQ    100000 // [Hz]
#define MOTOR_SPEED_LIMIT   1200 // [steps/s]

static const uint8_t step_halt[4] = {0, 0, 0, 0};
static const uint8_t step_table[4][4] = {
    {1, 0, 1, 0},
    {0, 1, 1, 0},
    {0, 1, 0, 1},
    {1, 0, 0, 1},
};

struct stepper_motor_s right_motor;
struct stepper_motor_s left_motor;

static void right_motor_update(const uint8_t *out)
{
    /* right motor */
    out[0] ? palSetPad(GPIOE, GPIOE_MOT_R_IN1) : palClearPad(GPIOE, GPIOE_MOT_R_IN1);
    out[1] ? palSetPad(GPIOE, GPIOE_MOT_R_IN2) : palClearPad(GPIOE, GPIOE_MOT_R_IN2);
    out[2] ? palSetPad(GPIOE, GPIOE_MOT_R_IN3) : palClearPad(GPIOE, GPIOE_MOT_R_IN3);
    out[3] ? palSetPad(GPIOE, GPIOE_MOT_R_IN4) : palClearPad(GPIOE, GPIOE_MOT_R_IN4);
}

static void left_motor_update(const uint8_t *out)
{
    /* left motor */
    out[0] ? palSetPad(GPIOE, GPIOE_MOT_L_IN1) : palClearPad(GPIOE, GPIOE_MOT_L_IN1);
    out[1] ? palSetPad(GPIOE, GPIOE_MOT_L_IN2) : palClearPad(GPIOE, GPIOE_MOT_L_IN2);
    out[2] ? palSetPad(GPIOE, GPIOE_MOT_L_IN3) : palClearPad(GPIOE, GPIOE_MOT_L_IN3);
    out[3] ? palSetPad(GPIOE, GPIOE_MOT_L_IN4) : palClearPad(GPIOE, GPIOE_MOT_L_IN4);
}

static void motor_step(struct stepper_motor_s *m)
{
    uint8_t i;
    if (m->direction == FORWARD) {
        i = (m->step_index + 1) & 3;
        m->update(step_table[i]);
        m->count += 1;
        m->step_index = i;

    } else if (m->direction == BACKWARD) {
        i = (m->step_index - 1) & 3;
        m->update(step_table[i]);
        m->count -= 1;
        m->step_index = i;

    } else {
        m->update(step_halt);
    }
}

static void right_motor_timer_callback(GPTDriver *gptp)
{
    (void) gptp;
    motor_step(&right_motor);
}

static void left_motor_timer_callback(GPTDriver *gptp)
{
    (void) gptp;
    motor_step(&left_motor);
}

/*
static void right_motor_timer_callback(GPTDriver *gptp)
{
    (void) gptp;
    uint8_t i;
    if (right_motor.direction == FORWARD) {
        i = (right_motor.step_index + 1) & 3;
        right_motor.update(step_table[i]);
        right_motor.count += 1;
        right_motor.step_index = i;

    } else if (right_motor.direction == BACKWARD) {
        i = (right_motor.step_index - 1) & 3;
        right_motor.update(step_table[i]);
        right_motor.count -= 1;
        right_motor.step_index = i;

    } else {
        right_motor.update(step_halt);
    }
}

static void left_motor_timer_callback(GPTDriver *gptp)
{
    (void) gptp;
    uint8_t i;
    if (left_motor.direction == BACKWARD) { // Inverted for the two motors
        i = (left_motor.step_index + 1) & 3;
        left_motor.update(step_table[i]);
        left_motor.count += 1;
        left_motor.step_index = i;

    } else if (left_motor.direction == FORWARD) {
        i = (left_motor.step_index - 1) & 3;
        left_motor.update(step_table[i]);
        left_motor.count -= 1;
        left_motor.step_index = i;

    } else {
        left_motor.update(step_halt);
    }
}
*/

void motor_set_speed(struct stepper_motor_s *m, int speed)
{
    /* limit motor speed */
    if (speed > MOTOR_SPEED_LIMIT) {
        speed = MOTOR_SPEED_LIMIT;
    } else if (speed < -MOTOR_SPEED_LIMIT) {
        speed = -MOTOR_SPEED_LIMIT;
    }

    uint16_t interval;
    if (speed == 0) {
        m->direction = HALT;
        interval = 0xffff;
    } else {
        if (speed > 0) {
            m->direction = FORWARD;
        } else {
            m->direction = BACKWARD;
            speed = -speed;
        }
        interval = MOTOR_TIMER_FREQ / speed;
    }

    /* change moter step interval */
    gptChangeInterval(m->timer, interval);
}

uint32_t motor_get_pos(struct stepper_motor_s *m)
{
    return m->count;
}

void motors_init(void)
{
    /* motor struct init */
    right_motor.direction = HALT;
    right_motor.step_index = 0;
    right_motor.count = 0;
    right_motor.update = right_motor_update;
    right_motor.timer = &GPTD12;

    left_motor.direction = HALT;
    left_motor.step_index = 0;
    left_motor.count = 0;
    left_motor.update = left_motor_update;
    left_motor.timer = &GPTD14;

    /* motor init halted*/
    right_motor_update(step_halt);
    left_motor_update(step_halt);

    /* timer init */
    static const GPTConfig tim12_config = {
        MOTOR_TIMER_FREQ,
        right_motor_timer_callback,
        0,  /* CR2 settings */
        0   /* DMA settings */
    };
    gptStart(&GPTD12, &tim12_config);
    gptStartContinuous(&GPTD12, 0xffff);

    static const GPTConfig tim14_config = {
        MOTOR_TIMER_FREQ,
        left_motor_timer_callback,
        0,  /* CR2 settings */
        0   /* DMA settings */
    };
    gptStart(&GPTD14, &tim14_config);
    gptStartContinuous(&GPTD14, 0xffff);
}
