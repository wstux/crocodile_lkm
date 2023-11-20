/*
 * crok_lkm
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

#ifndef _CROK_LKM_SYSTBL_MEMORY_PROTECTION_H_
#define _CROK_LKM_SYSTBL_MEMORY_PROTECTION_H_

#include <linux/syscalls.h>

inline void write_cr0_forced(unsigned long val)
{
    unsigned long force_order;

    asm volatile(
        "mov %0, %%cr0"
        : "+r"(val), "+m"(force_order));
}

#define PAGE_RW 0x00010000

#define PROTECT_MEMORY()    \
    write_cr0_forced((read_cr0() & ~PAGE_RW) ? read_cr0() | PAGE_RW : read_cr0())

#define UNPROTECT_MEMORY()  \
    write_cr0_forced(read_cr0() & ~PAGE_RW)

#define UNSAFE_CALL(func)   \
    UNPROTECT_MEMORY();     \
    (void)(func);           \
    PROTECT_MEMORY()

#endif /* _CROK_LKM_SYSTBL_MEMORY_PROTECTION_H_ */

