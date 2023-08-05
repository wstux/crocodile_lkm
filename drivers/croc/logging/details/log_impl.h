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

#ifndef _CROCODILE_LKM_LOGGING_LOG_IMPL_H_
#define _CROCODILE_LKM_LOGGING_LOG_IMPL_H_

#include "logging/details/severity_level_impl.h"

#define _PLOG_IMPL(logger, level, fmt, ...) \
    logger(_LOG_LEVEL(level) fmt __VA_OPT__(,) __VA_ARGS__)

#define _PLOG(logger, level, fmt, ...) \
    do { \
        if (! _PLOGGER_CAN_LOG(logger, level)) { \
            break; \
        } \
        _PLOG_IMPL(logger, level, fmt "\n", __VA_ARGS__); \
    } \
    while (0)

#endif /* _CROCODILE_LKM_LOGGING_LOG_IMPL_H_ */

