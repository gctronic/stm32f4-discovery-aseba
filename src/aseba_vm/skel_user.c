#include <string.h>
#include "ch.h"
#include "hal.h"

#include "skel_user.h"

#include "vm/natives.h"
#include "discovery_demo/leds.h"
#include "common/productids.h"
#include "common/consts.h"

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
        {SETTINGS_COUNT, "settings"},

		{0, NULL}
	}
};

// Event descriptions
const AsebaLocalEventDescription localEvents[] = {
    {"new_acc", "New accelerometer measurement"},
	{NULL, NULL}
};

void aseba_variables_init(AsebaVMState *vm)
{
    /* Initializes constant variables. */
    memset(&vmVariables, 0, sizeof(vmVariables));
    vmVariables.id = vm->nodeId;

    vmVariables.productId = ASEBA_PID_UNDEFINED;
    vmVariables.fwversion[0] = 0;
    vmVariables.fwversion[1] = 1;
}

void aseba_read_variables_from_system(AsebaVMState *vm)
{
    ASEBA_UNUSED(vm);
}

void aseba_write_variables_to_system(AsebaVMState *vm)
{
    ASEBA_UNUSED(vm);
    int i;
    for(i = 3; i <= 6; i++) {
        demo_led_set(i, vmVariables.leds[i - 1]);
    }
}


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
    ASEBA_UNUSED(vm);
    NVIC_SystemReset();
}

static AsebaNativeFunctionDescription AsebaNativeDescription_settings_save =
{
    "settings.save",
    "Save settings into flash",
    {
        {0,0}
    }
};

void AsebaNative_settings_save(AsebaVMState *vm)
{
    AsebaVMEmitNodeSpecificError(vm, "Not implemented (yet!).");
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
    ASEBA_UNUSED(vm);
    int i;

    for(i = 3; i <= 6; i++) {
        demo_led_set(i, 0);
        vmVariables.leds[i - 1] = 0;
    }
}



// Native function descriptions
const AsebaNativeFunctionDescription* nativeFunctionsDescription[] = {
	&AsebaNativeDescription__system_reboot,
	&AsebaNativeDescription_settings_save,
    &AsebaNativeDescription_clear_all_leds,
    ASEBA_NATIVES_STD_DESCRIPTIONS,
    0
};

// Native function pointers
AsebaNativeFunctionPointer nativeFunctions[] = {
    AsebaNative__system_reboot,
    AsebaNative_settings_save,
    clear_all_leds,
	ASEBA_NATIVES_STD_FUNCTIONS,
};

const int nativeFunctions_length = sizeof(nativeFunctions) / sizeof(nativeFunctions[0]);
