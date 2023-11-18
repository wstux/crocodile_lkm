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

#ifndef _CROCODILE_LKM_PROC_FILE_OPS_H_
#define _CROCODILE_LKM_PROC_FILE_OPS_H_

#include "types.h"

struct file;
struct inode;
struct seq_file;

int proc_open_mem(struct inode* p_inode, struct file* p_file);

int proc_read_mem(struct seq_file* p_file, void* p_data);

#endif /* _CROCODILE_LKM_PROC_FILE_OPS_H_ */

