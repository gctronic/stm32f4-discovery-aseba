#include <string.h>

// ChibiOS includes
#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "usbcfg.h"

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

void AsebaSendBuffer(AsebaVMState *vm, const uint8* data, uint16 length)
{
    chnWrite(&SDU1, data, length);
    chprintf((BaseSequentialStream *)&SDU1, "send_buffer\n");
    //chSequentialStreamWrite((BaseSequentialStream *)&SDU1, data, length);
}

uint16 AsebaGetBuffer(AsebaVMState *vm, uint8* data, uint16 maxLength, uint16* source)
{
    chnReadTimeout(&SDU1, data, maxLength, 100);
    chprintf((BaseSequentialStream *)&SDU1, "get_buffer %s of size %d\n", data, strlen((char *)data));
    //chSequentialStreamRead((BaseSequentialStream *)&SDU1, data, maxLength);
    memcpy(source, data, 2);
    return strlen(data);
}

void AsebaResetIntoBootloader(AsebaVMState *vm)
{

}

void AsebaNativeFunction(AsebaVMState *vm, uint16 id)
{
    nativeFunctions[id](vm);
}

const AsebaNativeFunctionDescription * const * AsebaGetNativeFunctionsDescriptions(AsebaVMState *vm)
{
    return nativeFunctionsDescription;
}


const AsebaVMDescription* AsebaGetVMDescription(AsebaVMState *vm)
{
    return &vmDescription;
}

const AsebaLocalEventDescription * AsebaGetLocalEventsDescriptions(AsebaVMState *vm)
{
    return localEvents;
}


void AsebaWriteBytecode(AsebaVMState *vm)
{

}
