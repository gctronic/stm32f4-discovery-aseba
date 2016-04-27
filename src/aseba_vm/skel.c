#include <string.h>

// ChibiOS includes
#include "ch.h"
#include "hal.h"

// Aseba includes
#include "vm/natives.h"
#include "vm/vm.h"
#include "common/consts.h"
#include "transport/buffer/vm-buffer.h"

#include "flash/flash.h"

#include "aseba_vm/skel.h"
#include "aseba_vm/skel_user.c"
#include "aseba_vm/aseba_bridge.h"

unsigned int events_flags = 0;


/*
 * VM
 */
struct _vmVariables vmVariables;
static uint16 vmBytecode[VM_BYTECODE_SIZE];
static sint16 vmStack[VM_STACK_SIZE];

AsebaVMState vmState = {
    .nodeId=0, /* changed by aseba_vm_init() */

    .bytecodeSize=VM_BYTECODE_SIZE,
    .bytecode=vmBytecode,

    .variablesSize=sizeof(vmVariables) / sizeof(sint16),
    .variables=(sint16*)&vmVariables,

    .stackSize=VM_STACK_SIZE,
    .stack=vmStack,
    .flags=0, .pc=0, .sp=0,
    .breakpoints={0}, .breakpointsCount=0,
};


/*
 * Callbacks
 */
void AsebaIdle(void)
{
    chThdYield();
}

void AsebaPutVmToSleep(AsebaVMState *vm)
{
    (void) vm;
    chThdSleepMilliseconds(1000);
}

void AsebaResetIntoBootloader(AsebaVMState *vm)
{
    (void) vm;
    NVIC_SystemReset();
}

void AsebaNativeFunction(AsebaVMState *vm, uint16 id)
{
    if (id < sizeof(nativeFunctions)/(sizeof(nativeFunctions[0]))) {
        nativeFunctions[id](vm);
    } else {
        AsebaVMEmitNodeSpecificError(vm, "Invalid native function.");
    }
}

const AsebaNativeFunctionDescription * const * AsebaGetNativeFunctionsDescriptions(AsebaVMState *vm)
{
    (void) vm;
    return nativeFunctionsDescription;
}


const AsebaVMDescription* AsebaGetVMDescription(AsebaVMState *vm)
{
    (void) vm;
    return &vmDescription;
}

const AsebaLocalEventDescription * AsebaGetLocalEventsDescriptions(AsebaVMState *vm)
{
    (void) vm;
    return localEvents;
}

uint16 AsebaShouldDropPacket(uint16 source, const uint8* data)
{
    /* Accept all packets in bridge mode. */
    if (aseba_is_bridge()) {
        return 0;
    }

    return AsebaVMShouldDropPacket(&vmState, source, data);
}

// Used to write bytecode in the flash, not implemented yet
void AsebaWriteBytecode(AsebaVMState *vm)
{
    extern char _aseba_bytecode_start;

    flash_unlock();
    flash_sector_erase(&_aseba_bytecode_start);
    flash_write(&_aseba_bytecode_start, &vm->bytecodeSize, sizeof(uint16));
    flash_write(&_aseba_bytecode_start + sizeof(uint16), vm->bytecode, vm->bytecodeSize);
    flash_lock();
}
