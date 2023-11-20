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

#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#include "logging.h"
#include "types.h"
#include "device/device.h"
#include "proc/details/file_ops.h"

int proc_open_mem(struct inode* /*p_inode*/, struct file* p_file)
{
    return single_open(p_file, proc_read_mem, NULL);
}

int proc_read_mem(struct seq_file* p_file, void* p_data)
{
    //const int size = p_file->size - 80;
    const int limit = p_file->size - 160;
    hash_node_t* p_cur = NULL;
    unsigned bkt;

    for (int i = 0; i < device_nr_devs; ++i) {
        module_dev_t* p_dev = devices_tbl + i;
        if (mutex_lock_interruptible(&p_dev->lock)) {
            KLOG_DEBUG(LOG_PREFIX "proc::proc_read_mem: failed to lock mutex");
            return -ERESTARTSYS;
        }

        seq_printf(p_file,"Log level: %i\n", GET_LOGF_LEVEL());
        seq_printf(p_file,"Count of hidden processes: %lu\n", p_dev->hash_tbl.size);
        if (p_dev->hash_tbl.size > 0) {
            seq_printf(p_file,"Hidden pids:\n");
        } else {
            seq_printf(p_file,"There are not hidden processes\n");
        }

        hash_for_each(p_dev->hash_tbl.tbl, bkt, p_cur, node) {
            if (p_file->count > limit) {
                seq_printf(p_file,"  -- too many pids to print...\n");
                break;
            }
            seq_printf(p_file,"  -- %i;\n", p_cur->pid);
        }
        mutex_unlock(&p_dev->lock);
    }

    return 0;
}

