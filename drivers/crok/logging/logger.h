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

#ifndef _CROK_LKM_LOGGING_LOGGER_H_
#define _CROK_LKM_LOGGING_LOGGER_H_

#if defined(__KERNEL__)
    #include <linux/kernel.h>
    #include <linux/kern_levels.h>
#endif

#include "logging/severity_level.h"
#include "logging/details/log_impl.h"
#include "logging/details/severity_level_impl.h"

/*
 * Runtime logger initialization. The default logging level value is a LVL_WARN.
 */
#define INIT_LOGGERF(logger, level) _INIT_LOGGERF_IMPL(logger, level)
#define GET_LOGF_LEVEL()            _GET_LOGF_LEVEL_IMPL()
#define SET_LOGF_LEVEL(level)       _SET_LOGF_LEVEL_IMPL(level)

#if defined(__KERNEL__)
    /*
     * Runtime kernel logger initialization. The default logging level value
     * is a LVL_WARN.
     */
    #define INIT_KLOGGER(level)     INIT_LOGGERF(printk, level)
#endif


#define LOGF_EMERG(logger, fmt, ...)    _LOGF(logger, LVL_EMERG, fmt, __VA_ARGS__)
#define LOGF_FATAL(logger, fmt, ...)    _LOGF(logger, LVL_FATAL, fmt, __VA_ARGS__)
#define LOGF_CRIT(logger, fmt, ...)     _LOGF(logger, LVL_CRIT, fmt, __VA_ARGS__)
#define LOGF_ERROR(logger, fmt, ...)    _LOGF(logger, LOG_ERROR, fmt, __VA_ARGS__)
#define LOGF_WARN(logger, fmt, ...)     _LOGF(logger, LVL_WARN, fmt, __VA_ARGS__)
#define LOGF_NOTICE(logger, fmt, ...)   _LOGF(logger, LVL_NOTICE, fmt, __VA_ARGS__)
#define LOGF_INFO(logger, fmt, ...)     _LOGF(logger, LVL_INFO, fmt, __VA_ARGS__)
#define LOGF_DEBUG(logger, fmt, ...)    _LOGF(logger, LVL_DEBUG, fmt, __VA_ARGS__)


#if defined(__KERNEL__)
    #define KLOG_EMERG(fmt, ...)    LOGF_EMERG(printk, fmt, __VA_ARGS__)
    #define KLOG_FATAL(fmt, ...)    LOGF_FATAL(printk, fmt, __VA_ARGS__)
    #define KLOG_CRIT(fmt, ...)     LOGF_CRIT(printk, fmt, __VA_ARGS__)
    #define KLOG_ERROR(fmt, ...)    LOGF_ERROR(printk, fmt, __VA_ARGS__)
    #define KLOG_WARN(fmt, ...)     LOGF_WARN(printk, fmt, __VA_ARGS__)
    #define KLOG_NOTICE(fmt, ...)   LOGF_NOTICE(printk, fmt, __VA_ARGS__)
    #define KLOG_INFO(fmt, ...)     LOGF_INFO(printk, fmt, __VA_ARGS__)
    #define KLOG_DEBUG(fmt, ...)    LOGF_DEBUG(printk, fmt, __VA_ARGS__)
#endif

#endif /* _CROK_LKM_LOGGING_LOGGER_H_ */

