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

#include <linux/hashtable.h>
#include <linux/slab.h>

#include "device/details/hash_tbl.h"

size_t hash_tbl_capacity(void)
{
    return (1 << HASH_TBL_BITS);
}

int hash_tbl_erase(hash_table_t* p_tbl, pid_t pid)
{
    hash_node_t* p_node = hash_tbl_find(p_tbl, pid);
    if (p_node == NULL) {
        return 0;
    }

    hash_del(&p_node->node);
    kfree(p_node);
    --p_tbl->size;
    return 0;
}

hash_node_t* hash_tbl_find(hash_table_t* p_tbl, pid_t pid)
{
    hash_node_t* p_cur = NULL;
    int is_contains = 0;

    if (p_tbl->size == 0) {
        return NULL;
    }

    hash_for_each_possible(p_tbl->tbl, p_cur, node, pid) {
        is_contains = 1;
        break;
    }
    if (is_contains == 0) {
        return NULL;
    }
    return p_cur;
}

void hash_tbl_init(hash_table_t* p_tbl)
{
    __hash_init(p_tbl->tbl, HASH_SIZE(p_tbl->tbl));
}

int hash_tbl_insert(hash_table_t* p_tbl, pid_t pid)
{
    hash_node_t* p_node;
    if (hash_tbl_is_contains(p_tbl, pid)) {
        return 0;
    }
    if (p_tbl->size >= hash_tbl_capacity()) {
        return -1;
    }

    p_node = kmalloc(sizeof(hash_node_t), GFP_KERNEL);
    hash_add(p_tbl->tbl, &p_node->node, pid);
    ++p_tbl->size;
    return 0;
}

int hash_tbl_is_contains(hash_table_t* p_tbl, pid_t pid)
{
    return (hash_tbl_find(p_tbl, pid) != NULL);
}

