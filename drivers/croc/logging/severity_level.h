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

#ifndef _CROCODILE_LKM_LOGGING_SEVERITY_LEVEL_H_
#define _CROCODILE_LKM_LOGGING_SEVERITY_LEVEL_H_

/* 
 * Implementations of log levels.
 * Supported levels:
 * - LVL_EMERG  - the system is unusable;
 * - LVL_FATAL  - actions that must be taken care of immediately;
 * - LVL_CRIT   - critical conditions;
 * - LVL_ERROR  - non-critical error conditions;
 * - LVL_WARN   - warning conditions that should be taken care of;
 * - LVL_NOTICE - normal, but significant events;
 * - LVL_INFO   - informational messages that require no action;
 * - LVL_DEBUG  - kernel debugging messages, output by the kernel if the
 *                developer enabled debugging at compile time.
 *
 * These logging levels duplicate the logging levels supported by the Linux
 * kernel.
 */
#define LVL_EMERG     0
#define LVL_FATAL     1
#define LVL_CRIT      2
#define LVL_ERROR     3
#define LVL_WARN      4
#define LVL_NOTICE    5
#define LVL_INFO      6
#define LVL_DEBUG     7

#define DEFAULT_LOG_LVL     LVL_INFO

#endif /* _CROCODILE_LKM_LOGGING_SEVERITY_LEVEL_H_ */

