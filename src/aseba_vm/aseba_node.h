#ifndef ASEBA_NODE_H
#define ASEBA_NODE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common/types.h"
#include "vm/vm.h"

/** Number of variables usable by the Aseba script. */
#define VM_VARIABLES_FREE_SPACE 256

/** Maximum number of args an Aseba event can use. */
#define VM_VARIABLES_ARG_SIZE 32

/** Number of opcodes in an aseba bytecode script. */
#define VM_BYTECODE_SIZE (766 + 768)  // PUT HERE 766 + 768 * a, where a is >= 0
#define VM_STACK_SIZE 128

/*
 * In your code, put "SET_EVENT(EVENT_NUMBER)" when you want to trigger an
 * event. This macro is interrupt-safe, you can call it anywhere you want.
 *
 * FIXME: On STM32 This is *not* IRQ safe.
 */
#define SET_EVENT(event) (events_flags |= (1 << event))
#define CLEAR_EVENT(event) (events_flags &= ~(1 << event))
#define IS_EVENT(event) (events_flags & (1 << event))

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

extern struct _vmVariables vmVariables;
extern unsigned int events_flags;
extern AsebaVMState vmState;

void aseba_vm_start(void);
void accelerometer_cb(void);
void aseba_vm_init(void);

#ifdef __cplusplus
}
#endif

#endif
