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

#include <linux/fs.h>
#include <linux/module.h>

#include "logging.h"
#include "ctrl/details/hash_tbl.h"
#include "device/device.h"
#include "device/params.h"
#include "device/details/cdev_utils.h"
#include "device/details/file_ops.h"

static int device_major     = DEVICE_MAJOR_DFL;
static int device_minor     = DEVICE_MINOR_DFL;
static int device_nr_devs   = DEVICE_NR_DEVS_DFL;

static module_dev_t* devices_tbl = NULL;    /* allocated in register_device */

static struct file_operations device_fops = {
	.owner          = THIS_MODULE,
	.llseek         = NULL,
	.read           = dev_read,
	.write          = dev_write,
	.unlocked_ioctl = dev_ioctl,
	.open           = dev_open,
	.release        = dev_release
};

int deregister_device(void)
{
    dev_t devno = MKDEV(device_major, device_minor);
    /* Get rid of our char dev entries */
    if (devices_tbl != NULL) {
        for (int i = 0; i < device_nr_devs; ++i) {
            cdev_trim(devices_tbl + i);
            cdev_del(&devices_tbl[i].cdev);
        }
        kfree(devices_tbl);
    }

    /* cleanup_module is never called if registering failed */
    unregister_chrdev_region(devno, device_nr_devs);

    return 0;
}

int register_device(void)
{
    int rc = 0;
    dev_t dev = 0;

    if (device_major != 0) {
        dev = MKDEV(device_major, device_minor);
        rc = register_chrdev_region(dev, device_nr_devs, MODULE_NAME);
    } else {
        rc = alloc_chrdev_region(&dev, device_minor, device_nr_devs, MODULE_NAME);
        device_major = MAJOR(dev);
    }
    if (rc < 0) {
        KLOG_WARN(LOG_PREFIX "can't register device %d", device_major);
        return rc;
    }

    /* 
     * allocate the devices -- we can't have them static, as the number
     * can be specified at load time
     */
    devices_tbl = kmalloc(device_nr_devs * sizeof(module_dev_t), GFP_KERNEL);
    if (! devices_tbl) {
        rc = -ENOMEM;
        deregister_device();  /* Make this more graceful */
        return rc;
    }
    memset(devices_tbl, 0, device_nr_devs * sizeof(module_dev_t));

    /* Initialize each device. */
    for (int i = 0; i < device_nr_devs; ++i) {
        mutex_init(&devices_tbl[i].lock);
        cdev_setup(&devices_tbl[i], device_major, device_minor, i, &device_fops);
        hash_tbl_init(&devices_tbl[i].hash_tbl);
    }
    return 0;
}

