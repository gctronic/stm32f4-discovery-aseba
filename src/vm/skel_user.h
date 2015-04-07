#ifndef SKEL_USER_H
#define SKEL_USER_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Send queue size may be smaller (64 is the smallest value) something like 70-80 is better
 * 128 is luxury
 */
#define SEND_QUEUE_SIZE 128

/*
 * Warning, at least an aseba message MUST be able to fit into this RECV_QUEUE_SIZE buffer
 * 256 is IMHO the minimum, but maybe it can be lowered with a lot of caution.
 * The bigger you have, the best it is. Fill the empty ram with it :)
 */
#define RECV_QUEUE_SIZE 756

/*
 * This is the number of "private" variable the aseba script can have
 */
#define VM_VARIABLES_FREE_SPACE 256

/*
 * This is the maximum number of argument an aseba event can recieve
 */
#define VM_VARIABLES_ARG_SIZE 32


/*
 * The number of opcode an aseba script can have
 */
#define VM_BYTECODE_SIZE 766  // PUT HERE 766 + 768 * a, where a is >= 0
#define VM_STACK_SIZE 32



struct _vmVariables {
	// NodeID
	sint16 id;
	// Source
	sint16 source;
	// Args
	sint16 args[VM_VARIABLES_ARG_SIZE];

	// Variables

	// Free space
	sint16 freeSpace[VM_VARIABLES_FREE_SPACE];
};


enum Events
{
	YOUR_FIRST_EVENT = 0,
	YOUR_SECOND_EVENT,
	/****
	---> PUT YOUR EVENT NUMBER HERE <---
	Must be in the same order as in skel.c
	****/
	EVENTS_COUNT // Do not touch
};

// The content of this structure is implementation-specific.
// The glue provide a way to store and retrive it from flash.
// The only way to write it is to do it from inside the VM (native function)
// The native function access it as a integer array. So, use only int inside this structure
struct private_settings {
	/* ADD here the settings to save into flash */
	/* The minimum size is one integer, the maximum size is 95 integer (Check done at compilation) */
	int first_setting;
	int second_setting;
};


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


#ifdef __cplusplus
}
#endif

#endif /* SKEL_USER_H */
