#include "aseba_vm/skel.h"
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
