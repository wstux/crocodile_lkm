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

#ifndef _CROCODILE_LKM_LOGGING_LOG_STUBS_H_
#define _CROCODILE_LKM_LOGGING_LOG_STUBS_H_

/*
 * Init stubs for logging macros.
 * The required minimum logging level is enabled by a compilation flag of the
 * form LOGGER_USE_<level>_LEVEL. In order not to waste time on checks, the
 * disabled logging level is defined by a stub that does nothing.
 */
#define PLOG_EMERG(logger, fmt, ...)
#define PLOG_FATAL(logger, fmt, ...)
#define PLOG_CRIT(logger, fmt, ...)
#define PLOG_ERROR(logger, fmt, ...)
#define PLOG_WARN(logger, fmt, ...)
#define PLOG_NOTICE(logger, fmt, ...)
#define PLOG_INFO(logger, fmt, ...)
#define PLOG_DEBUG(logger, fmt, ...)


/*
 * Enabling logging depending on the set compilation flags.
 * The idea is that each logging level that is enabled undefined its logging
 * stub and includes the top logging level for itself, so that all logging
 * levels from enabled and above are enabled.
 */

/*
 * If LVL_DEBUG is enabled, the stub will be undefined and the next logging
 * level enabled.
 */
#if defined(LOGGER_USE_DEBUG_LEVEL)
    #undef PLOG_DEBUG
    #if ! defined(LOGGER_USE_INFO_LEVEL)
        #define LOGGER_USE_INFO_LEVEL
    #endif
#endif
/*
 * If LVL_INFO is enabled, the stub will be undefined and the next logging
 * level enabled.
 */
#if defined(LOGGER_USE_INFO_LEVEL)
    #undef PLOG_INFO
    #if ! defined(LOGGER_USE_NOTICE_LEVEL)
        #define LOGGER_USE_NOTICE_LEVEL
    #endif
#endif
/*
 * If LVL_NOTICE is enabled, the stub will be undefined and the next logging
 * level enabled.
 */
#if defined(LOGGER_USE_NOTICE_LEVEL)
    #undef PLOG_NOTICE
    #if ! defined(LOGGER_USE_WARNING_LEVEL)
        #define LOGGER_USE_WARNING_LEVEL
    #endif
#endif
/*
 * If LVL_WARN is enabled, the stub will be undefined and the next logging
 * level enabled.
 */
#if defined(LOGGER_USE_WARN_LEVEL)
    #undef PLOG_WARN
    #if ! defined(LOGGER_USE_ERROR_LEVEL)
        #define LOGGER_USE_ERROR_LEVEL
    #endif
#endif
/*
 * If LVL_ERROR is enabled, the stub will be undefined and the next logging
 * level enabled.
 */
#if defined(LOGGER_USE_ERROR_LEVEL)
    #undef PLOG_ERROR
    #if ! defined(LOGGER_USE_CRIT_LEVEL)
        #define LOGGER_USE_CRIT_LEVEL
    #endif
#endif
/*
 * If LVL_CRIT is enabled, the stub will be undefined and the next logging
 * level enabled.
 */
#if defined(LOGGER_USE_CRIT_LEVEL)
    #undef PLOG_CRIT
    #if ! defined(LOGGER_USE_FATAL_LEVEL)
        #define LOGGER_USE_FATAL_LEVEL
    #endif
#endif
/*
 * If LVL_FATAL is enabled, the stub will be undefined and the next logging
 * level enabled.
 */
#if defined(LOGGER_USE_FATAL_LEVEL)
    #undef PLOG_FATAL
    #if ! defined(LOGGER_USE_EMERG_LEVEL)
        #define LOGGER_USE_EMERG_LEVEL
    #endif
#endif
/*
 * If LVL_EMERG is enabled, the stub will be undefined.
 */
#if defined(LOGGER_USE_EMERG_LEVEL)
    #undef PLOG_EMERG
#endif

#endif /* _CROCODILE_LKM_LOGGING_LOG_STUBS_H_ */

