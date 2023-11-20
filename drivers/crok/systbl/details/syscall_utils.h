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

#ifndef _CROK_LKM_SYSTBL_SYSCALL_UTILS_H_
#define _CROK_LKM_SYSTBL_SYSCALL_UTILS_H_

#include "types.h"

/*
 * @brief   System call table initialization.
 * @return  On success, 0 is returned. On failure, -1 is returned.
 */
long init_syscall_table(void);

/*
 * @brief   Replacement system call a custom function.
 * @param   hook_syscall_fn - custom function.
 * @param   syscall_num - system call number.
 * @return  On success, 0 is returned. On failure, -1 is returned.
 */
long hook_syscall(sys_call_fn_t hook_syscall_fn, int syscall_num);

/*
 * @brief   Get original system call function.
 * @param   syscall_num - system call number.
 * @return  On success, original system call is returned. On failure, NULL is returned.
 */
sys_call_fn_t orig_syscall(int syscall_num);

/*
 * @brief   Restore all the original system calls.
 * @return  On success, 0 is returned. On failure, -1 is returned.
 */
long restore_all_syscalls(void);

/*
 * @brief   Restore the original system call.
 * @param   syscall_num - system call number.
 * @return  On success, 0 is returned. On failure, -1 is returned.
 */
long restore_orig_syscall(int syscall_num);

#endif /* _CROK_LKM_SYSTBL_SYSCALL_UTILS_H_ */

