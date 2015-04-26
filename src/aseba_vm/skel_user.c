/*
 * Descriptors
 */
const AsebaVMDescription vmDescription = {
	"EpuckBase",
	{
		// { number of element in array, name displayed in aseba studio }
		{1, "_id"},
		{1, "event.source"},
		{VM_VARIABLES_ARG_SIZE, "event.args"},
		{1, "_productId"},

		{1, "led"},

		{0, NULL}
	}
};

// Event descriptions
static const AsebaLocalEventDescription localEvents[] = {
	{NULL, NULL}
};

// Native function descriptions
static const AsebaNativeFunctionDescription* nativeFunctionsDescription[] = {
	ASEBA_NATIVES_STD_DESCRIPTIONS,
	0
};

// Native function pointers
static AsebaNativeFunctionPointer nativeFunctions[] = {
	ASEBA_NATIVES_STD_FUNCTIONS,
};
