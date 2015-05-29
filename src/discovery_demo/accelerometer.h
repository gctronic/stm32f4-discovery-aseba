#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float acceleration[3];
} accelerometer_sample_t;

void demo_acc_start(void);
void demo_acc_get_acc(float *acc);

#ifdef __cplusplus
}
#endif

#endif /* ACCELEROMETER_H */
