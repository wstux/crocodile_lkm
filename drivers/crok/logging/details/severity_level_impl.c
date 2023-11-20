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

#include <linux/atomic.h>

#include "logging/severity_level.h"
#include "logging/details/severity_level_impl.h"

#define _LVL_COUNT  LVL_DEBUG + 1

/* Default severity level. */
static atomic_t _severity_level = ATOMIC_INIT(LVL_WARN); 

int _can_log(int lvl)
{
    return (lvl <= atomic_read(&_severity_level));
}

int _get_log_level(void)
{
    return atomic_read(&_severity_level);
}

int _init_logger(int lvl)
{
    if ((lvl < 0) || (lvl > _LVL_COUNT)) {
        return -1;
    }
    atomic_set(&_severity_level, lvl);
    return 0;
}

void _set_log_level(int lvl)
{
    if ((lvl < 0) || (lvl > _LVL_COUNT)) {
        return;
    }
    atomic_set(&_severity_level, lvl);
}

#undef _LVL_COUNT

