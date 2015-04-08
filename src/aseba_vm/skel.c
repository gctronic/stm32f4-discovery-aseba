#include <string.h>

// ChibiOS includes
#include <hal.h>

// Aseba includes
#include "vm/natives.h"
#include "vm/vm.h"
#include "common/consts.h"
#include "transport/buffer/vm-buffer.h"
// TODO : include cmsis for reset function

#include "skel.h"

#define INSTRUCTIONS_PER_PAGE 128
#define INSTRUCTIONS_PER_ROW  1

unsigned int events_flags = 0;

static AsebaNativeFunctionDescription AsebaNativeDescription__system_reboot = {
	"_system.reboot",
	"Reboot the microcontroller",
	{
		{0, 0}
	}
};

void AsebaResetIntoBootloader(AsebaVMState *vm)
{
	NVIC_SystemReset();
}

static AsebaNativeFunctionDescription AsebaNativeDescription__system_settings_read = {
	"_system.settings.read",
	"Read a setting",
	{
		{1, "address"},
		{1, "value"},
		{0, 0}
	}
};

static void AsebaNative__system_settings_read(AsebaVMState *vm)
{
	uint16 address = vm->variables[AsebaNativePopArg(vm)];
	uint16 destidx = AsebaNativePopArg(vm);

	if(address > sizeof(settings)/2 - 1) {
		AsebaVMEmitNodeSpecificError(vm, "Address out of settings");
		return;
	}
	vm->variables[destidx] = ((unsigned int *) &settings)[address];
}

static AsebaNativeFunctionDescription AsebaNativeDescription__system_settings_write =
{
	"_system.settings.write",
	"Write a setting",
	{
		{1, "address"},
		{1, "value"},
		{0, 0}
	}
};

static void AsebaNative__system_settings_write(AsebaVMState *vm)
{
	uint16 address = vm->variables[AsebaNativePopArg(vm)];
	uint16 sourceidx = AsebaNativePopArg(vm);
	if(address > sizeof(settings)/2 - 1) {
		AsebaVMEmitNodeSpecificError(vm, "Address out of settings");
		return;
	}
	((unsigned int *) &settings)[address] = vm->variables[sourceidx];
}

static AsebaNativeFunctionDescription AsebaNativeDescription__system_settings_flash = {
	"_system.settings.flash",
	"Write the settings into flash",
	{
		{0, 0}
	}
};

static void AsebaNative__system_settings_flash(AsebaVMState *vm);


#include "skel_user.c"


struct private_settings __attribute__((aligned(2))) settings;


const AsebaLocalEventDescription * AsebaGetLocalEventsDescriptions(AsebaVMState *vm)
{
	return localEvents;
}


/*
 * VM
 */
struct __attribute((far)) _vmVariables vmVariables;

static __attribute((far))  uint16 vmBytecode[VM_BYTECODE_SIZE];

static __attribute((far))  sint16 vmStack[VM_STACK_SIZE];

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
	Idle();
}

void AsebaPutVmToSleep(AsebaVMState *vm)
{

}

void AsebaNativeFunction(AsebaVMState *vm, uint16 id)
{
	nativeFunctions[id](vm);
}


static void received_packet_dropped(void)
{
	/* ---- PUT HERE SOME LED STATE OUTPUT ----- */
}

static void sent_packet_dropped(void)
{
	/* ---- PUT HERE SOME LED STATE OUTPUT ----- */
}

const AsebaVMDescription * AsebaGetVMDescription(AsebaVMState *vm)
{
	return &vmDescription;
}

uint16 AsebaShouldDropPacket(uint16 source, const uint8* data)
{
	return AsebaVMShouldDropPacket(&vmState, source, data);
}


const AsebaNativeFunctionDescription * const * AsebaGetNativeFunctionsDescriptions(AsebaVMState *vm)
{
	return nativeFunctionsDescription;
}


// START of Bytecode into Flash section -----

// A chunk is a bytecode image
#define PAGE_PER_CHUNK ((VM_BYTECODE_SIZE*2+3 + INSTRUCTIONS_PER_PAGE*3 - 1) / (INSTRUCTIONS_PER_PAGE*3))
#if PAGE_PER_CHUNK == 0
#undef PAGE_PER_CHUNK
#define PAGE_PER_CHUNK 1
#endif

#define NUMBER_OF_CHUNK 26

unsigned long aseba_flash_ptr;
unsigned long aseba_settings_ptr;

void AsebaWriteBytecode(AsebaVMState *vm) {
	// Look for the lowest number of use
	unsigned long min = 0xFFFFFFFF;
	unsigned long min_addr = 0;
	unsigned long count;
	unsigned int i;
	unsigned long temp_addr = aseba_flash_ptr;
	unsigned int instr_count;
	unsigned char * bcptr = (unsigned char *) vm->bytecode;
	// take the first minimum value
	for (i = 0; i < NUMBER_OF_CHUNK; i++, temp_addr += INSTRUCTIONS_PER_PAGE * 2 * PAGE_PER_CHUNK) {
		count = flash_read_instr(temp_addr);
		if(count < min) {
			min = count;
			min_addr = temp_addr;
		}
	}
	if(min == 0xFFFFFFFF) {
		AsebaVMEmitNodeSpecificError(vm, "Error: min == 0xFFFFFFFF !");
		return;
	}
	min++;

	// Now erase the pages
	for(i = 0; i < PAGE_PER_CHUNK; i++) {
		flash_erase_page(min_addr + i*INSTRUCTIONS_PER_PAGE * 2);
	}

	// Then write the usage count and the bytecode
	flash_prepare_write(min_addr);
	flash_write_instruction(min);
	flash_write_buffer((unsigned char *) vm->bytecode, VM_BYTECODE_SIZE*2);
	flash_complete_write();

	// Now, check the data
	if(min != flash_read_instr(min_addr)) {
		AsebaVMEmitNodeSpecificError(vm, "Error: Unable to flash bytecode (1) !");
		return;
	}
	min_addr += 2;
	instr_count = (VM_BYTECODE_SIZE*2) / 3;

	for(i = 0; i < instr_count; i++) {
		unsigned char data[3];
		flash_read_chunk(min_addr, 3, data);

		if(memcmp(data, bcptr, 3)) {
			AsebaVMEmitNodeSpecificError(vm, "Error: Unable to flash bytecode (2) !");
			return;
		}
		bcptr += 3;
		min_addr += 2;
	}

	i = (VM_BYTECODE_SIZE * 2) % 3;

	if(i != 0) {
		unsigned char data[2];
		flash_read_chunk(min_addr, i, data);
		if(memcmp(data, bcptr, i)) {
			AsebaVMEmitNodeSpecificError(vm, "Error: Unable to flash bytecode (3) !");
			return;
		}
	}

	AsebaVMEmitNodeSpecificError(vm, "Flashing OK");
}

const static unsigned int _magic_[8] = {0xDE, 0xAD, 0xCA, 0xFE, 0xBE, 0xEF, 0x04, 0x02};
static void AsebaNative__system_settings_flash(AsebaVMState *vm)
{
	// Look for the last "Magic" we know, this is the most up to date conf
	// Then write the next row with the correct magic.
	// If no magic is found, erase the page, and then write the first one
	// If the last magic is found, erase the page and then write the first one

	unsigned long setting_addr = aseba_settings_ptr;
	int i = 0;
	unsigned int mag;
	unsigned long temp;
	for(i = 0; i < 8; i++) {
		mag = flash_read_low(setting_addr + INSTRUCTIONS_PER_ROW * 2 * i);
		if(mag != _magic_[i])
			break;
	}

	if(i == 0 || i == 8) {
		flash_erase_page(setting_addr);
		i = 0;
	}

	setting_addr += INSTRUCTIONS_PER_ROW * 2 * i;

	flash_prepare_write(setting_addr);
	temp = (((unsigned long) *((unsigned char * ) &settings)) << 16) | _magic_[i];

	flash_write_instruction(temp);
	flash_write_buffer(((unsigned char *) &settings) + 1, sizeof(settings) - 1);
	flash_complete_write();
}

static void load_code_from_flash(AsebaVMState *vm)
{
	// Find the last maximum value
	unsigned long max = 0;
	unsigned long max_addr = 0;
	unsigned long temp_addr = aseba_flash_ptr;
	unsigned long count;
	unsigned int i;
	// take the last maximum value
	for (i = 0; i < NUMBER_OF_CHUNK; i++, temp_addr += INSTRUCTIONS_PER_PAGE * 2 * PAGE_PER_CHUNK) {
		count = flash_read_instr(temp_addr);
		if(count >= max) {
			max = count;
			max_addr = temp_addr;
		}
	}
	if(!max)
		// Nothing to load
		return;

	flash_read_chunk(max_addr + 2, VM_BYTECODE_SIZE*2, (unsigned char *) vm->bytecode);

	// Tell the VM to init
	AsebaVMSetupEvent(vm, ASEBA_EVENT_INIT);
}

int load_settings_from_flash(void)
{
	// The the last "known" magic found
	unsigned long temp = aseba_settings_ptr;
	int i = 0;
	unsigned int mag;



	for(i = 0; i < 8; i++) {
		mag = flash_read_low(temp + INSTRUCTIONS_PER_ROW * 2 * i);
		if(mag != _magic_[i])
			break;
	}
	if(i == 0)
		// No settings found
		return -1;
	i--;
	temp += INSTRUCTIONS_PER_ROW * 2 * i;
	*((unsigned char *) &settings) = (unsigned char) (flash_read_high(temp) & 0xFF);
	flash_read_chunk(temp + 2, sizeof(settings) - 1, ((unsigned char *) &settings) + 1);

	return 0;
}
// END of bytecode into flash section


#ifdef ASEBA_ASSERT
void AsebaAssert(AsebaVMState *vm, AsebaAssertReason reason) {
	AsebaVMEmitNodeSpecificError(vm, "VM ASSERT !");
}
#endif

static unsigned int ui2str(char *str, unsigned int value)
{
	unsigned div;
	unsigned int i = 0;
	bool hasSent = false;
	for (div = 10000; div > 0; div /= 10)
	{
		unsigned disp = value / div;
		value %= div;

		if ((disp != 0) || (hasSent) || (div == 1))
		{
			hasSent = true;
			str[i++] =  '0' + disp;
		}
	}
	str[i] = 0;
	return i;
}

static unsigned int ui2strhex(char * str, unsigned int id)
{
	int shift;
	bool hasSent = false;
	unsigned int i = 0;
	for (shift = 12; shift >= 0; shift -= 4)
	{
		unsigned disp = ((id & (0xF << shift)) >> shift);

		if ((disp != 0) || (hasSent) || (shift == 0))
		{
			hasSent = true;
			if(disp > 9)
				str[i++] = 'A' + disp - 0xA;
			else
				str[i++] = '0' + disp;
		}
	}
	str[i] = 0;
	return i;
}

static void __attribute__((noreturn)) error_handler(const char * file, int line, int id, void* __attribute((unused)) arg)
{
	char error_string[255];
	char number[10];
	strcpy(error_string, "Molole error 0x");
	ui2strhex(number, (unsigned int) id);
	strcat(error_string, number);
	strcat(error_string, " in file: ");
	strncat(error_string, file, 200);
	strcat(error_string, ":");
	ui2str(number, line);
	strcat(error_string, number);


	AsebaVMEmitNodeSpecificError(&vmState, error_string);

	AsebaCanFlushQueue();


	for(;;); // Shutup GCC
}

#define FUID3 0xF80016

void aseba_vm_init(void)
{
	unsigned int low, high;
	// Read the Device ID at _FUID3

 	vmState.nodeId = flash_read_low(FUID3);

 	// Get the section start pointer. Cannot get it in C, so use the assember-style
 	aseba_flash_ptr = (unsigned long) high << 16 | low;

 	aseba_settings_ptr = (unsigned long) high << 16 | low;

	AsebaVMInit(&vmState);
	vmVariables.id = vmState.nodeId;

	load_code_from_flash(&vmState);

	error_register_callback(error_handler);

}

void __attribute((noreturn)) run_aseba_main_loop(void) {
	while(1)
	{
		update_aseba_variables_read();

		AsebaVMRun(&vmState, 1000);

		AsebaProcessIncomingEvents(&vmState);

		update_aseba_variables_write();

		// Either we are in step by step, so we go to sleep until further commands, or we are not executing an event,
		// and so we have nothing to do.
		if (AsebaMaskIsSet(vmState.flags, ASEBA_VM_STEP_BY_STEP_MASK) || AsebaMaskIsClear(vmState.flags, ASEBA_VM_EVENT_ACTIVE_MASK))
		{
			unsigned i;
			// Find first bit from right (LSB), check if some CAN packets are pending
			// Atomically, do an Idle() if nothing is found
			// Warning, I'm doing some nasty things with the IPL bits (forcing the value, without safeguard)
			// Second warning: It doesn't disable level 7 interrupt. So if you set an event inside a level 7 interrupt
			// It may get delayed until next interrupt


			// If a local event is pending, then execute it.
			// Else, we waked up from idle because of an interrupt, so re-execute the whole thing
			// FIXME: do we want to kill execution upon local events? that would be consistant so Steph votes yes, but
			// we may discuss further
			if(i && !(AsebaMaskIsSet(vmState.flags, ASEBA_VM_STEP_BY_STEP_MASK) &&  AsebaMaskIsSet(vmState.flags, ASEBA_VM_EVENT_ACTIVE_MASK))) {
			// that is the same as (thx de Morgan)
			//if(i && (AsebaMaskIsClear(vmState.flags, ASEBA_VM_STEP_BY_STEP_MASK) ||  AsebaMaskIsClear(vmState.flags, ASEBA_VM_EVENT_ACTIVE_MASK))) {
				i--;
				CLEAR_EVENT(i);
				vmVariables.source = vmState.nodeId;
				AsebaVMSetupEvent(&vmState, ASEBA_EVENT_LOCAL_EVENTS_START - i);
			}
		}
	}
}
