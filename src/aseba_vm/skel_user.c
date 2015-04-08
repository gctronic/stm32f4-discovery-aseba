/*
 * Descriptors
 */
const AsebaVMDescription vmDescription = {
	"epuck_base",
	{
		{1, "_id"},
		{1, "event.source"},
		{VM_VARIABLES_ARG_SIZE, "event.args"},
		{1, "led"},
		// { number of element in array, name displayed in aseba studio },
		{0, NULL}
	}
};

static const AsebaLocalEventDescription localEvents[] = {
	/*******
	---> PUT YOUR EVENT DESCRIPTIONS HERE <---
	First value is event "name" (will be used as "onvent name" in asebastudio
	second value is the event description)
	*******/
	{ NULL, NULL }
};

static const AsebaNativeFunctionDescription* nativeFunctionsDescription[] = {
	&AsebaNativeDescription__system_reboot,
	&AsebaNativeDescription__system_settings_read,
	&AsebaNativeDescription__system_settings_write,

	ASEBA_NATIVES_STD_DESCRIPTIONS,
	0
};

static AsebaNativeFunctionPointer nativeFunctions[] = {
	AsebaResetIntoBootloader,
	AsebaNative__system_settings_read,
	AsebaNative__system_settings_write,

	ASEBA_NATIVES_STD_FUNCTIONS,
};
