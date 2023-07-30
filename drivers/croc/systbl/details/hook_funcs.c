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

#include "logging/logger.h"
#include "systbl/hook_funcs.h"
#include "systbl/syscall_utils.h"

asmlinkage long hack_execve(const struct pt_regs* p_regs)
{
    int exec_res;
    char __user* filename = (char*)p_regs->di;
    char kfilename[NAME_MAX] = {0};
    sys_call_fn_t orig_execve = NULL;

    if (strncpy_from_user(kfilename, filename, NAME_MAX) > 0) {
        KLOG_INFO("executable file is '%s'\n", kfilename);
    }

    orig_execve = orig_syscall(__NR_execve);
    exec_res = orig_execve(p_regs);
    /*
     * #define current get_current()
     * static inline struct task_struct * get_current(void) - return context of current task.
     */
    KLOG_INFO("execve '%s' process\n", current->comm);
    return exec_res;
}

