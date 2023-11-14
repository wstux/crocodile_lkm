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

#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#include "logging.h"
#include "proc/proc.h"
#include "proc/details/file_ops.h"

static struct proc_ops mem_proc_fops = {
	.proc_open    = proc_open_mem,
	.proc_read    = seq_read,
	.proc_lseek   = seq_lseek,
	.proc_release = single_release
};

void deregister_proc(void)
{
	remove_proc_entry("log_lvl", NULL /* parent dir */);
}

void register_proc(void)
{
	proc_create_data("log_lvl",
	                 0 /* default mode */,
			         NULL /* parent dir */, 
			         &mem_proc_fops,
			         NULL /* client data */);
}

