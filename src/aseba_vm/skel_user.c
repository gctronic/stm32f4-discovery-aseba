#include "ch.h"
#include "hal.h"

#include "skel.h"

/*
 * Descriptors
 */
const AsebaVMDescription vmDescription = {
	"Epuck 2",
	{
		// { number of element in array, name displayed in aseba studio }
		{1, "_id"},
		{1, "event.source"},
		{VM_VARIABLES_ARG_SIZE, "event.args"},
		{2, "_fwversion"},
		{1, "_productId"},

		{1, "led"},

		{0, NULL}
	}
};

// Event descriptions
static const AsebaLocalEventDescription localEvents[] = {
    {"led", "led blink"},
	{NULL, NULL}
};


static AsebaNativeFunctionDescription AsebaNativeDescription_led_toggle =
{
	"led_toggle",
	"Toggles the LED",
	{
		{0,0}
	}
};

void AsebaNative_led_toggle(AsebaVMState *vm)
{
	palTogglePad(GPIOD, GPIOD_LED4);
	vmVariables.led = !(vmVariables.led);
}

// Native function descriptions
static const AsebaNativeFunctionDescription* nativeFunctionsDescription[] = {
	&AsebaNative_led_toggle,
	ASEBA_NATIVES_STD_DESCRIPTIONS,
	0
};

// Native function pointers
static AsebaNativeFunctionPointer nativeFunctions[] = {
	AsebaNative_led_toggle,
	ASEBA_NATIVES_STD_FUNCTIONS,
};


