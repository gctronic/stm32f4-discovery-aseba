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
    chprintf((BaseSequentialStream *)&SD2, "aseba idle\n");
    chThdSleepMilliseconds(1);
}

void AsebaPutVmToSleep(AsebaVMState *vm)
{
    chprintf((BaseSequentialStream *)&SD2, "aseba sleep\n");
    chThdSleepMilliseconds(500);
}

void AsebaSendBuffer(AsebaVMState *vm, const uint8* data, uint16 length)
{
    chnWrite(&SDU1, data, length);
    chprintf((BaseSequentialStream *)&SD2, "send_buffer\n");
    //chSequentialStreamWrite((BaseSequentialStream *)&SD2, data, length);
}

uint16 AsebaGetBuffer(AsebaVMState *vm, uint8* data, uint16 maxLength, uint16* source)
{
    static uint16_t len = 0;
    static size_t data_available;

    data_available = chnReadTimeout(&SDU1, &len, 2, TIME_IMMEDIATE);

    chprintf((BaseSequentialStream *)&SD2, "get_buffer of size %d\n", len);

    if(data_available >= 2) {
        chnRead(&SDU1, source, 2);

        len += 2;
        if (len > maxLength) {
            len = maxLength;
        }

        chnRead(&SDU1, data, len);
    }

    //chSequentialStreamRead((BaseSequentialStream *)&SD2, data, maxLength);

    return len;
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
