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

#ifndef _CROK_LKM_LOGGING_SEVERITY_LEVEL_IMPL_H_
#define _CROK_LKM_LOGGING_SEVERITY_LEVEL_IMPL_H_

#if defined(__KERNEL__)
    #include <linux/kern_levels.h>
#endif

/*
 * Implementation details of logging levels.
 */
#if defined(__KERNEL__)
    #define _IMPL_LOG_LVL_0     KERN_EMERG
    #define _IMPL_LOG_LVL_1     KERN_ALERT
    #define _IMPL_LOG_LVL_2     KERN_CRIT
    #define _IMPL_LOG_LVL_3     KERN_ERR
    #define _IMPL_LOG_LVL_4     KERN_WARNING
    #define _IMPL_LOG_LVL_5     KERN_NOTICE
    #define _IMPL_LOG_LVL_6     KERN_INFO
    #define _IMPL_LOG_LVL_7     KERN_DEBUG
#else
    #define _IMPL_LOG_LVL_0     "[EMERG] "
    #define _IMPL_LOG_LVL_1     "[FATAL] "
    #define _IMPL_LOG_LVL_2     "[CRIT ] "
    #define _IMPL_LOG_LVL_3     "[ERROR] "
    #define _IMPL_LOG_LVL_4     "[WARN ] "
    #define _IMPL_LOG_LVL_5     "[NOTIC] "
    #define _IMPL_LOG_LVL_6     "[INFO ] "
    #define _IMPL_LOG_LVL_7     "[DEBUG] "
#endif

inline int _can_log(int lvl);
inline int _get_log_level(void);
inline int _init_logger(int lvl);
inline void _set_log_level(int lvl);

#define _INIT_LOGGERF_IMPL(logger, level)   _init_logger(level)
#define _LOG_LEVEL(level)                   _IMPL_LOG_LVL_ ## level
#define _LOGGERF_CAN_LOG(logger, level)     _can_log(level)
#define _GET_LOGF_LEVEL_IMPL()              _get_log_level()
#define _SET_LOGF_LEVEL_IMPL(level)         _set_log_level(level)

#endif /* _CROK_LKM_LOGGING_SEVERITY_LEVEL_IMPL_H_ */

