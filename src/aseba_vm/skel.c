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
#include "aseba_vm/aseba_node.h"

#include "aseba_vm/skel_user.h"
#include "aseba_vm/aseba_bridge.h"


void AsebaNativeFunction(AsebaVMState *vm, uint16 id)
{
    if (id < nativeFunctions_length) {
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

void AsebaWriteBytecode(AsebaVMState *vm)
{
    extern char _aseba_bytecode_start;

    flash_unlock();
    flash_sector_erase(&_aseba_bytecode_start);
    flash_write(&_aseba_bytecode_start, &vm->bytecodeSize, sizeof(uint16));
    flash_write(&_aseba_bytecode_start + sizeof(uint16), vm->bytecode, vm->bytecodeSize);
    flash_lock();
}
