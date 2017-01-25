/* Cydia Substrate - Powerful Code Insertion Platform
 * Copyright (C) 2008-2011  Jay Freeman (saurik)
*/

/* GNU Lesser General Public License, Version 3 {{{ */
/*
 * Substrate is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * Substrate is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Substrate.  If not, see <http://www.gnu.org/licenses/>.
**/
/* }}} */

#include <dlfcn.h>
#include <mach/mach.h>

#include <mach/machine/thread_status.h>

#include <pthread.h>
#include <unistd.h>

#include "CydiaSubstrate.h"

#include "Log.hpp"

#include "Baton.hpp"
#include "Trampoline.t.hpp"

extern "C" void __pthread_set_self(pthread_t);

#define _krncall(expr) \
    do { \
        kern_return_t _krnstatus((expr)); \
        if (_krnstatus != KERN_SUCCESS) { \
            MSLog(MSLogLevelError, "MS:Error: _krncall(%s)", #expr); \
            return false; \
        } \
    } while (false)

_extern bool MSHookProcess(pid_t pid, const char *library) {
    if (library[0] != '/') {
        MSLog(MSLogLevelError, "MSError: require absolute path to %s", library);
        return false;
    }

    static const size_t Stack_(8 * 1024);
    size_t length(strlen(library) + 1), depth(sizeof(Baton) + length);
    depth = (depth + sizeof(uintptr_t) + 1) / sizeof(uintptr_t) * sizeof(uintptr_t);

    uint8_t local[depth];
    Baton *baton(reinterpret_cast<Baton *>(local));


    // the dyld shared cache is only shuffled once per boot, allowing us to assume no ASLR
    // however, it is important that we restrict ourselves to those in cached libraries
    // XXX: it would be preferable to do this in a cross-architecture way, remotely

    baton->__pthread_set_self = &__pthread_set_self;

    baton->pthread_create = &pthread_create;
    baton->pthread_join = &pthread_join;

    baton->mach_thread_self = &mach_thread_self;
    baton->thread_terminate = &thread_terminate;

    baton->dlerror = &dlerror;
    baton->dlsym = &dlsym;


    memcpy(baton->library, library, length);

    vm_size_t size(depth + Stack_);

    mach_port_t self(mach_task_self()), task;
    _krncall(task_for_pid(self, pid, &task));

    vm_address_t stack;
    _krncall(vm_allocate(task, &stack, size, true));
    vm_address_t data(stack + Stack_);

    _krncall(vm_write(task, data, reinterpret_cast<vm_address_t>(baton), depth));

    thread_act_t thread;
    _krncall(thread_create(task, &thread));


    // XXX: look into using thread_get_state(THREAD_STATE_FLAVOR_LIST) to look up flavor

    thread_state_flavor_t flavor;
    mach_msg_type_number_t count;
    size_t push;

    Trampoline *trampoline;

#if defined(__arm__)
    trampoline = &Trampoline_armv6_;
    arm_thread_state_t state;
    flavor = ARM_THREAD_STATE;
    count = ARM_THREAD_STATE_COUNT;
    push = 0;
#elif defined(__i386__)
    trampoline = &Trampoline_i386_;
    i386_thread_state_t state;
    flavor = i386_THREAD_STATE;
    count = i386_THREAD_STATE_COUNT;
    push = 5;
#elif defined(__x86_64__)
    trampoline = &Trampoline_x86_64_;
    x86_thread_state64_t state;
    flavor = x86_THREAD_STATE64;
    count = x86_THREAD_STATE64_COUNT;
    push = 2;
#else
    #error XXX: implement
#endif


    vm_address_t code;
    _krncall(vm_allocate(task, &code, trampoline->size_, true));
    _krncall(vm_write(task, code, reinterpret_cast<vm_address_t>(trampoline->data_), trampoline->size_));
    _krncall(vm_protect(task, code, trampoline->size_, false, VM_PROT_READ | VM_PROT_EXECUTE));

    uint32_t frame[push];
    if (sizeof(frame) != 0)
        memset(frame, 0, sizeof(frame));
    memset(&state, 0, sizeof(state));

    mach_msg_type_number_t read(count);
    _krncall(thread_get_state(thread, flavor, reinterpret_cast<thread_state_t>(&state), &read));

    if (read != count) {
        MSLog(MSLogLevelError, "MSError: thread_get_state(%d) == %d", count, read);
        return false;
    }


    // this code is very similar to that found in Libc/pthread's _pthread_setup

#if defined(__arm__)
    state.__r[0] = data;
    state.__sp = stack + Stack_;
    state.__pc = code + trampoline->entry_;


    // ARM has two execution states: ARM (32-bit) and Thumb (16/32-bit), using different instruction sets
    // for addressing, we tell using the least significant bit: off-aligned addresses are assumed to be Thumb
    // however, despite the CPU interpreting this bit during branches, it stores this information in CPSR

    if ((state.__pc & 0x1) != 0) {
        state.__pc &= ~0x1;
        state.__cpsr |= 0x20;
    }
#elif defined(__i386__)
    frame[1] = data;

    state.__eip = code + trampoline->entry_;
    state.__esp = stack + Stack_ - sizeof(frame);
#elif defined(__x86_64__)
    // XXX: I do not remember why this is here
    frame[0] = 0xdeadbeef;

    state.__rdi = data;
    state.__rip = code + trampoline->entry_;
    state.__rsp = stack + Stack_ - sizeof(frame);
#else
    #error XXX: implement
#endif


    if (sizeof(frame) != 0)
        _krncall(vm_write(task, stack + Stack_ - sizeof(frame), reinterpret_cast<vm_address_t>(frame), sizeof(frame)));

    _krncall(thread_set_state(thread, flavor, reinterpret_cast<thread_state_t>(&state), count));
    _krncall(thread_resume(thread));


    // XXX: I feel like there must be a way to get an event when the thread dies, rather than poll
    // XXX: all we now know is that the thread finished, but what we really care about is whether it worked

    do loop: switch (kern_return_t status = thread_get_state(thread, flavor, reinterpret_cast<thread_state_t>(&state), &(read = count))) {
        case KERN_SUCCESS:
            usleep(10000);
            goto loop;

        case KERN_TERMINATED:
        case MACH_SEND_INVALID_DEST:
            break;

        default:
            MSLog(MSLogLevelError, "MSError: thread_get_state() == %d", status);
            return false;
    } while (false);


    _krncall(mach_port_deallocate(self, thread));

    _krncall(vm_deallocate(task, code, trampoline->size_));
    _krncall(vm_deallocate(task, stack, size));

    _krncall(mach_port_deallocate(self, task));

    return true;
}
