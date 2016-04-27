#ifndef SKEL_USER_H
#define SKEL_USER_H

#ifdef __cplusplus
extern "C" {
#endif

/** Number of variables usable by the Aseba script. */
#define VM_VARIABLES_FREE_SPACE 256

/** Maximum number of args an Aseba event can use. */
#define VM_VARIABLES_ARG_SIZE 32

/** Number of opcodes in an aseba bytecode script. */
#define VM_BYTECODE_SIZE (766 + 768)  // PUT HERE 766 + 768 * a, where a is >= 0
#define VM_STACK_SIZE 128


struct _vmVariables {
	sint16 id; 							// NodeID
	sint16 source; 						// Source
	sint16 args[VM_VARIABLES_ARG_SIZE]; // Args
	sint16 fwversion[2];				// Firmware version
	sint16 productId;					// Product ID

	// Variables
	uint16 leds[6];
	sint16 acc[3];

	// Free space
	sint16 freeSpace[VM_VARIABLES_FREE_SPACE];
};


enum Events
{
	EVENT_ACC = 0, // New accelerometer measurement
	EVENTS_COUNT   // Do not touch
};

#ifdef __cplusplus
}
#endif

#endif /* SKEL_USER_H */
