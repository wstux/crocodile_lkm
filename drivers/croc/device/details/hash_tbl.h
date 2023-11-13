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

#ifndef _CROCODILE_LKM_DEVICE_HASH_TBL_H_
#define _CROCODILE_LKM_DEVICE_HASH_TBL_H_

#include "types.h"

size_t hash_tbl_capacity(void);

int hash_tbl_erase(hash_table_t* p_tbl, pid_t pid);

hash_node_t* hash_tbl_find(hash_table_t* p_tbl, pid_t pid);

void hash_tbl_init(hash_table_t* p_tbl);

int hash_tbl_insert(hash_table_t* p_tbl, pid_t pid);

int hash_tbl_is_contains(hash_table_t* p_tbl, pid_t pid);

#endif /* _CROCODILE_LKM_DEVICE_HASH_TBL_H_ */

