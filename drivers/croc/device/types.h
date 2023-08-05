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

#ifndef _CROCODILE_LKM_DEVICE_TYPES_H_
#define _CROCODILE_LKM_DEVICE_TYPES_H_

#include <linux/cdev.h>
#include <linux/types.h>

/******************************************************************************
 *  Device types.
 ******************************************************************************/

struct module_dev
{
    struct mutex lock;  /* mutual exclusion semaphore   */
    struct cdev cdev;   /* char device structure        */
};

typedef struct module_dev   module_dev_t;

#endif /* _CROCODILE_LKM_DEVICE_TYPES_H_ */

