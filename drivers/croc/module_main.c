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

#include <linux/cdev.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/syscalls.h>

#include "params.h"
#include "version.h"
#include "device/device.h"
#include "logging/logger.h"

/*
 * @brief   Module cleanup.
 */
static void __exit cleanup_module_hsyst(void)
{
    int rc = 0;

    KLOG_INFO(LOG_PREFIX "cleanup_module");

    deregister_device();
}

/*
 * @brief   Module initialization.
 */
static int __init init_module_hsyst(void)
{
    int rc = 0;

    INIT_KLOGGER(LVL_INFO);
    KLOG_INFO(LOG_PREFIX "init_module");

    rc = register_device();

    return rc;
}

module_init(init_module_hsyst);
module_exit(cleanup_module_hsyst);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Chistyakov Alexander");
MODULE_DESCRIPTION("Some description...");
MODULE_VERSION( MODULE_RELEASE_VERSION );

