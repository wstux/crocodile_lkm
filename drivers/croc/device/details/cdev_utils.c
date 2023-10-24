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

#include "params.h"
#include "device/details/cdev_utils.h"
#include "logging/logger.h"

void cdev_setup(module_dev_t* p_dev, int major, int minor, int idx, 
                struct file_operations* p_fops)
{
    int err;
    int devno = MKDEV(major, minor + idx);

    cdev_init(&p_dev->cdev, p_fops);
    p_dev->cdev.owner = THIS_MODULE;
    err = cdev_add(&p_dev->cdev, devno, 1);

    /* Fail gracefully if need be */
    if (err) {
        KLOG_NOTICE(LOG_PREFIX "error %d adding " MODULE_NAME "%d", err, idx);
    }
}

int cdev_trim(module_dev_t* p_dev)
{
    /* Do nothing now. */
    return 0;
}

