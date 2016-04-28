#include "ch.h"
#include "hal.h"

#include "skel_user.h"

#include "vm/natives.h"
#include "discovery_demo/leds.h"

struct _vmVariables vmVariables;

const AsebaVMDescription vmDescription = {
	BOARD_NAME,
	{
		// {Number of element in array, Name displayed in aseba studio}
		{1, "_id"},
		{1, "event.source"},
		{VM_VARIABLES_ARG_SIZE, "event.args"},
        {2, "_fwversion"},
		{1, "_productId"},

        {6, "leds"},
        {3, "acc"},

		{0, NULL}
	}
};

// Event descriptions
const AsebaLocalEventDescription localEvents[] = {
    {"new_acc", "New accelerometer measurement"},
	{NULL, NULL}
};

// Native functions
static AsebaNativeFunctionDescription AsebaNativeDescription__system_reboot =
{
    "_system.reboot",
    "Reboot the microcontroller",
    {
        {0,0}
    }
};

void AsebaNative__system_reboot(AsebaVMState *vm)
{
    (void) vm;
    NVIC_SystemReset();
}

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

// Native function descriptions
const AsebaNativeFunctionDescription* nativeFunctionsDescription[] = {
	&AsebaNativeDescription__system_reboot,
    &AsebaNativeDescription_set_led,
    &AsebaNativeDescription_clear_all_leds,
    ASEBA_NATIVES_STD_DESCRIPTIONS,
    0
};

// Native function pointers
AsebaNativeFunctionPointer nativeFunctions[] = {
    AsebaNative__system_reboot,
    &set_led,
    &clear_all_leds,
	ASEBA_NATIVES_STD_FUNCTIONS,
};

const int nativeFunctions_length = sizeof(nativeFunctions) / sizeof(nativeFunctions[0]);
