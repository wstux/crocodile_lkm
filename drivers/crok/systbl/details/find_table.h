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

#ifndef _CROK_LKM_SYSTBL_FIND_TABLE_H_
#define _CROK_LKM_SYSTBL_FIND_TABLE_H_

#include "types.h"

/*
 * @brief   Finding a pointer to the system call table.
 * @return  Pointer to the system call table.
 */
sys_call_table_t* find_syscall_table(void);

#endif /* _CROK_LKM_SYSTBL_FIND_TABLE_H_ */

