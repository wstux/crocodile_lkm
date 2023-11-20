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

#ifndef _CROK_LKM_TYPES_H_
#define _CROK_LKM_TYPES_H_

#include <linux/cdev.h>
#include <linux/hashtable.h>
#include <linux/linkage.h>
#include <linux/mutex.h>
#include <linux/types.h>

/******************************************************************************
 *  Common types.
 ******************************************************************************/

/******************************************************************************
 *  Device types.
 ******************************************************************************/

#define HASH_TBL_BITS       8

/**/
struct hash_node
{
    pid_t pid;
    struct hlist_node node;
};
typedef struct hash_node    hash_node_t;

/**/
struct hash_table
{
    size_t              size;
    DECLARE_HASHTABLE(tbl, HASH_TBL_BITS);
};
typedef struct hash_table   hash_table_t;

/**/
struct module_dev
{
    struct mutex    lock;   /* mutual exclusion semaphore   */
    struct cdev     cdev;   /* char device structure        */

    hash_table_t    hash_tbl;
};
typedef struct module_dev   module_dev_t;

/******************************************************************************
 *  System table types.
 ******************************************************************************/

/* Forward declaration of the system structure. */
struct pt_regs;

/* Type of system call table pointer. */
typedef unsigned long       sys_call_table_t;

/* Type of system call functions. */
typedef asmlinkage long (*sys_call_fn_t)(const struct pt_regs*);

#endif /* _CROK_LKM_TYPES_H_ */

