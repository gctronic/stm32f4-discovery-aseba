#include "aseba_vm/aseba_node.h"
#include "vm/natives.h"
#include "common/types.h"
#include "discovery_demo/leds.h"


AsebaNativeFunctionDescription AsebaNativeDescription_set_led = {
    "leds.set",
    "Set the specified LED",
    {
        {1, "led"},
        {1, "brightness"},
        {0, 0}
    }
};

void set_led(AsebaVMState *vm)
{
    int led = vm->variables[AsebaNativePopArg(vm)];
    int brightness = vm->variables[AsebaNativePopArg(vm)];

    demo_led_set(led, brightness);

    vmVariables.leds[led - 1] = brightness;
}

AsebaNativeFunctionDescription AsebaNativeDescription_clear_all_leds = {
    "leds.clear_all",
    "Clear all the LEDs",
    {
        {0, 0}
    }
};

void clear_all_leds(AsebaVMState *vm)
{
    (void) vm;
    int i;

    for(i = 3; i <= 6; i++) {
        demo_led_set(i, 0);
        vmVariables.leds[i - 1] = 0;
    }
}
