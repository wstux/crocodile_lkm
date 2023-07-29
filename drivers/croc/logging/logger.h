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

#ifndef _CROCODILE_LKM_LOGGING_LOGGER_H_
#define _CROCODILE_LKM_LOGGING_LOGGER_H_

#if defined(__KERNEL__)
    #include <linux/kern_levels.h>
#endif

#include "logging/severity_level.h"
#include "logging/details/log_impl.h"
#include "logging/details/log_stubs.h"
#include "logging/details/severity_level_impl.h"

/*
 * Runtime logger initialization. The default logging level value is a LVL_WARN.
 */
#define INIT_PLOGGER(logger, level) _INIT_PLOGGER_IMPL(logger, level)

#if defined(__KERNEL__)
    /*
     * Runtime kernel logger initialization. The default logging level value
     * is a LVL_WARN.
     */
    #define INIT_KLOGGER(level)     INIT_PLOGGER(printk, level)
#endif


#if defined(LOGGER_USE_EMERG_LEVEL)
    #define PLOG_EMERG(logger, fmt, ...) \
        _PLOG(logger, LVL_EMERG, fmt, __VA_ARGS__)
#endif

#if defined(LOGGER_USE_FATAL_LEVEL)
    #define PLOG_FATAL(logger, fmt, ...) \
        _PLOG(logger, LVL_FATAL, fmt, __VA_ARGS__)
#endif

#if defined(LOGGER_USE_CRIT_LEVEL)
    #define PLOG_CRIT(logger, fmt, ...) \
        _PLOG(logger, LVL_CRIT, fmt, __VA_ARGS__)
#endif

#if defined(LOGGER_USE_ERROR_LEVEL)
    #define PLOG_ERROR(logger, fmt, ...) \
        _PLOG(logger, LOG_ERROR, fmt, __VA_ARGS__)
#endif

#if defined(LOGGER_USE_WARN_LEVEL)
    #define PLOG_WARN(logger, fmt, ...) \
        _PLOG(logger, LVL_WARN, fmt, __VA_ARGS__)
#endif

#if defined(LOGGER_USE_NOTICE_LEVEL)
    #define PLOG_NOTICE(logger, fmt, ...) \
        _PLOG(logger, LVL_NOTICE, fmt, __VA_ARGS__)
#endif

#if defined(LOGGER_USE_INFO_LEVEL)
    #define PLOG_INFO(logger, fmt, ...) \
        _PLOG(logger, LVL_INFO, fmt, __VA_ARGS__)
#endif

#if defined(LOGGER_USE_DEBUG_LEVEL)
    #define PLOG_DEBUG(logger, fmt, ...) \
        _PLOG(logger, LVL_DEBUG, fmt, __VA_ARGS__)
#endif


#if defined(__KERNEL__)
    #define KLOG_EMERG(fmt, ...)    PLOG_EMERG(printk, fmt, __VA_ARGS__)
    #define KLOG_FATAL(fmt, ...)    PLOG_FATAL(printk, fmt, __VA_ARGS__)
    #define KLOG_CRIT(fmt, ...)     PLOG_CRIT(printk, fmt, __VA_ARGS__)
    #define KLOG_ERROR(fmt, ...)    PLOG_ERROR(printk, fmt, __VA_ARGS__)
    #define KLOG_WARN(fmt, ...)     PLOG_WARN(printk, fmt, __VA_ARGS__)
    #define KLOG_NOTICE(fmt, ...)   PLOG_NOTICE(printk, fmt, __VA_ARGS__)
    #define KLOG_INFO(fmt, ...)     PLOG_INFO(printk, fmt, __VA_ARGS__)
    #define KLOG_DEBUG(fmt, ...)    PLOG_DEBUG(printk, fmt, __VA_ARGS__)
#endif

#endif /* _CROCODILE_LKM_LOGGING_LOGGER_H_ */

