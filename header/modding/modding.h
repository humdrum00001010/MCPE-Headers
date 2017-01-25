#ifndef MODDING_H_
#define MODDING_H_

#include <cstdint>
#include <dlfcn.h>

#define VTABLE_PADDING 8
#define access(ret, target, offset) (*(ret*)(((uintptr_t)target) + ((uint32_t)offset)))
#define vfnc(ret, target, offset, ...) ((ret(*)(__VA_ARGS__))access(void*, access(void**, target, 0), offset))

class Reversing {
public:
	static uintptr_t IMAGEBASE;
	static int PROT_CONST;
	static int PROT_DEFAULT;
	static int PROT_FUNCTION;

	static void binaryPatch(void *addr, uint64_t hexCode, int prot);
	
	static void modify_1(void *addr, uint8_t hexCode, int prot);

	static void modify_2(void *addr, uint16_t hexCode, int prot);

	static void modify_4(void *addr, uint32_t hexCode, int prot);

	static void modify_8(void *addr, uint64_t hexCode, int prot);
};

#endif