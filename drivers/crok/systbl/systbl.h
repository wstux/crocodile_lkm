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

#ifndef _CROK_LKM_SYSTBL_SYSTBL_H_
#define _CROK_LKM_SYSTBL_SYSTBL_H_

#include "systbl/details/hacked_funcs.h"
#include "systbl/details/syscall_utils.h"

#define HOOK_SYSCALL(syscall, fn_ptr)                           \
    ({                                                          \
        int rc = 0;                                             \
        if (hook_syscall(fn_ptr, __NR_##syscall) != 0) {        \
            restore_orig_syscall(__NR_##syscall);               \
            rc = -1;                                            \
        }                                                       \
        rc;                                                     \
    })

#endif /* _CROK_LKM_SYSTBL_SYSTBL_H_ */

