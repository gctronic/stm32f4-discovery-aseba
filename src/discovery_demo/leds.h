#ifndef LEDS_H
#define LEDS_H

#ifdef __cplusplus
extern "C" {
#endif

void demo_led_init(void);
void demo_led_set(int led, int brightness);

#ifdef __cplusplus
}
#endif

#endif /* LEDS_H */
