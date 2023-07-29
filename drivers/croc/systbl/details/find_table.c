/*
 * crocodile_lkm
 * Copyright (C) 2023  Chistyakov Alexander
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/syscalls.h>
#include <linux/version.h>

#include "systbl/details/find_table.h"

#if LINUX_VERSION_CODE <= KERNEL_VERSION(4, 16, 0)
    #error "Usupported kernel version"
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,7,0)
    #define USE_KPROBE 1
#endif

#if LINUX_VERSION_CODE > KERNEL_VERSION(4, 4, 0)
    #define USE_KALLSYMS_LOOKUP_FUNC 1
#endif

#if defined(USE_KALLSYMS_LOOKUP_FUNC) && defined(USE_KPROBE)
    #include <linux/kprobes.h>
#endif

sys_call_table_t* find_syscall_table(void)
{
#ifdef USE_KPROBE
    typedef unsigned long (*kallsyms_lookup_name_t)(const char* name);

    kallsyms_lookup_name_t kallsyms_lookup_name;
    struct kprobe kp_kallsyms_lookup = { .symbol_name = "kallsyms_lookup_name" };
#endif

    sys_call_table_t* p_syscall_table = NULL;

#ifdef USE_KALLSYMS_LOOKUP_FUNC
    #ifdef USE_KPROBE
        register_kprobe(&kp_kallsyms_lookup);
        kallsyms_lookup_name = (kallsyms_lookup_name_t)kp_kallsyms_lookup.addr;
        unregister_kprobe(&kp_kallsyms_lookup);
    #endif // USE_KPROBE

    p_syscall_table = (sys_call_table_t*)kallsyms_lookup_name("sys_call_table");
#else // USE_KALLSYMS_LOOKUP_FUNC
    unsigned long int i;

    for (i = (sys_call_table_t)sys_close; i < ULONG_MAX; i += sizeof(void*)) {
        if(((sys_call_table_t*)i)[__NR_close] == (sys_call_table_t)sys_close) {
            p_syscall_table = (sys_call_table_t*)i;
            break;
        }
    }
#endif // USE_KALLSYMS_LOOKUP_FUNC

    return p_syscall_table;
}

