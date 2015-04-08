#ifndef SKEL_H
#define SKEL_H

#ifdef __cplusplus
extern "C" {
#endif


// ChibiOS includes
#include <hal.h>

// Aseba include
#include "vm/natives.h"
#include "vm/vm.h"
#include "common/types.h"

#include "skel_user.h"

extern struct _vmVariables vmVariables;

extern unsigned int events_flags;

extern AsebaVMState vmState;

/*
 * In your code, put "SET_EVENT(EVENT_NUMBER)" when you want to trigger an
 * event. This macro is interrupt-safe, you can call it anywhere you want.
 */
#define SET_EVENT(event) atomic_or(&events_flags, 1 << event)
#define CLEAR_EVENT(event) atomic_and(&events_flags, ~(1 << event))
#define IS_EVENT(event) (events_flags & (1 << event))


// Call this when everything is initialised and you are ready to give full control to the VM
void __attribute((noreturn)) run_aseba_main_loop(void);

// Call this to init aseba. Beware, this will:
// 1. init the CAN module
// 2. clear vmVariables.
// 3. Load any bytecode in flash if present
void aseba_vm_init(void);

// This function must update the variables to match the microcontroller state
// It is called _BEFORE_ running the VM, so it's a {Microcontroller state} -> {Aseba Variable}
// synchronisation
// Implement it yourself
void update_aseba_variables_read(void);

// This function must update the microcontrolleur state to match the variables
// It is called _AFTER_ running the VM, so it's a {Aseba Variables} -> {Microcontroller state}
// synchronisation
// Implement it yourself
void update_aseba_variables_write(void);

// This function load the settings structure from flash. Call it _AFTER_ init_aseba_and_can()
// return 0 if the settings were loaded
// return non-zero if the settings were NOT found (settings is non-initilised)
int load_settings_from_flash(void);

extern struct private_settings settings;


#ifdef __cplusplus
}
#endif

#endif /* SKEL_H */
