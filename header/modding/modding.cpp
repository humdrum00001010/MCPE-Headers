#include "modding.h"

#include <sys/mman.h>

#define _ZN19AppPlatformListenerC2Eb_OFFSET (0x0082CF00)

uintptr_t Reversing::IMAGEBASE = (uintptr_t)dlsym(RTLD_DEFAULT, "_ZN19AppPlatformListenerC2Eb") - _ZN19AppPlatformListenerC2Eb_OFFSET;

int Reversing::PROT_CONST = PROT_READ;
int Reversing::PROT_DEFAULT = PROT_CONST | PROT_WRITE;
int Reversing::PROT_FUNCTION = PROT_CONST | PROT_EXEC;

unsigned int getSize(uint64_t integer) {
	if (integer & 0xff == integer) return 1;
	if (integer & 0xffff == integer) return 2;
	if (integer & 0xffffffff == integer) return 4;
	if (integer & 0xffffffffffffffff == integer) return 8;
	return -1;
}

void Reversing::binaryPatch(void *addr, uint64_t hexCode, int prot) {
	int size = getSize(hexCode);

	switch (size) {
		case 1:
		modify_1(addr, hexCode, prot);
		break;
		case 2:
		modify_2(addr, hexCode, prot);
		break;
		case 4:
		modify_4(addr, hexCode, prot);
		break;
		case 8:
		modify_8(addr, hexCode, prot);
		break;
		default:
		return;
		break;
	}
}

void Reversing::modify_1(void *addr, uint8_t hexCode, int prot) {
	addr = (void *)(((int)addr) & (-2));
	mprotect((void *)addr, 1, PROT_READ | PROT_WRITE);
	*(uint8_t *)addr = hexCode;
	mprotect((void *)addr, 1, prot);
}

void Reversing::modify_2(void *addr, uint16_t hexCode, int prot) {
	addr = (void *)(((int)addr) & (-2));
	mprotect((void *)addr, 2, PROT_READ | PROT_WRITE);
	*(uint16_t *)addr = hexCode;
	mprotect((void *)addr, 2, prot);
}

void Reversing::modify_4(void *addr, uint32_t hexCode, int prot) {
	addr = (void *)(((int)addr) & (-2));
	mprotect((void *)addr, 4, PROT_READ | PROT_WRITE);
	*(uint32_t *)addr = hexCode;
	mprotect((void *)addr, 4, prot);
}

void Reversing::modify_8(void *addr, uint64_t hexCode, int prot) {
	addr = (void *)(((int)addr) & (-2));
	mprotect((void *)addr, 8, PROT_READ | PROT_WRITE);
	*(uint64_t *)addr = hexCode;
	mprotect((void *)addr, 8, prot);
}