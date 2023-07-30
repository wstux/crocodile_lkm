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

#include "systbl/syscall_utils.h"
#include "systbl/details/find_table.h"
#include "systbl/details/memory_protection.h"

/* System call table pointer. */
static sys_call_table_t* _p_sys_call_table = NULL;

/* Original system call table. */
static sys_call_fn_t _orig_syscall_table[NR_syscalls];

/******************************************************************************
 *  Private functions.
 ******************************************************************************/

/*
 * @brief   Requesting a pointer to the system call table.
 * @return  Pointer to the system call table.
 *
 * @details To write.
 */
static sys_call_table_t* get_syscall_table(void)
{
    if (_p_sys_call_table != NULL) {
        return _p_sys_call_table;
    }

    return find_syscall_table();
}

/******************************************************************************
 *  Public functions.
 ******************************************************************************/

/*
 * @details To write.
 */
long init_syscall_table(void)
{
    _p_sys_call_table = get_syscall_table();
    if (! _p_sys_call_table) {
        return -1;
    }

    for (int i = 0; i < NR_syscalls; ++i) {
        _orig_syscall_table[i] = NULL;
    }

    return 0;
}

/*
 * @details To write.
 */
sys_call_fn_t hook_syscall(sys_call_fn_t hook_syscall_fn, int syscall_num)
{
    if (! _p_sys_call_table) {
        return NULL;
    }
    if (syscall_num >= NR_syscalls) {
        return NULL;
    }

    if (! _orig_syscall_table[syscall_num]) {
        _orig_syscall_table[syscall_num] = (sys_call_fn_t)_p_sys_call_table[syscall_num];
    }

    UNSAFE_CALL(_p_sys_call_table[syscall_num] = (unsigned long)hook_syscall_fn);
    return _orig_syscall_table[syscall_num];
}

/*
 * @details To write.
 */
sys_call_fn_t orig_syscall(int syscall_num)
{
    if (! _p_sys_call_table) {
        return NULL;
    }
    if (syscall_num >= NR_syscalls) {
        return NULL;
    }

    if (_orig_syscall_table[syscall_num]) {
        return _orig_syscall_table[syscall_num];
    }

    return (sys_call_fn_t)_p_sys_call_table[syscall_num];
}

/*
 * @details To write.
 */
long restore_orig_syscall(int syscall_num)
{
    if (! _p_sys_call_table) {
        return -1;
    }
    if (syscall_num >= NR_syscalls) {
        return -1;
    }
    if (! _orig_syscall_table[syscall_num]) {
        return 0;
    }

    UNSAFE_CALL(_p_sys_call_table[syscall_num] = (unsigned long)_orig_syscall_table[syscall_num]);

    return 0;
}

#if defined(USE_KALLSYMS_LOOKUP_FUNC)
    #undef USE_KALLSYMS_LOOKUP_FUNC
#endif

#if defined(USE_KPROBE)
    #undef USE_KPROBE
#endif

