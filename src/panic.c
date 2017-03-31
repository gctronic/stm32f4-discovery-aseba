#include <ch.h>
#include <hal.h>

void panic_handler(const char *reason)
{
    (void)reason;

    palSetPad(GPIOD, GPIOD_LED3);
    palSetPad(GPIOD, GPIOD_LED4);
    palSetPad(GPIOD, GPIOD_LED5);
    palSetPad(GPIOD, GPIOD_LED6);
    while (true) {

    }
}
