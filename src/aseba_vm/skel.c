#include <string.h>

// ChibiOS includes
#include <hal.h>

// Aseba includes
#include "vm/natives.h"
#include "vm/vm.h"
#include "common/consts.h"
#include "transport/buffer/vm-buffer.h"

#include "skel.h"
#include "skel_user.c"

unsigned int events_flags = 0;


/*
 * VM
 */
struct _vmVariables vmVariables;
static uint16 vmBytecode[VM_BYTECODE_SIZE];
static sint16 vmStack[VM_STACK_SIZE];

AsebaVMState vmState = {
    0,

    VM_BYTECODE_SIZE,
    vmBytecode,

    sizeof(vmVariables) / sizeof(sint16),
    (sint16*)&vmVariables,

    VM_STACK_SIZE,
    vmStack
};


/*
 * Callbacks
 */
void AsebaIdle(void)
{
    chThdSleepMilliseconds(1);
}

void AsebaPutVmToSleep(AsebaVMState *vm)
{
    chThdSleepMilliseconds(500);
}
