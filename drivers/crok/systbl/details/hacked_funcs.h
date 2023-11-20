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

#ifndef _CROK_LKM_SYSTBL_HACKED_FUNCS_H_
#define _CROK_LKM_SYSTBL_HACKED_FUNCS_H_

#include "types.h"

asmlinkage long hacked_execve(const struct pt_regs* p_regs);

asmlinkage long hacked_getdents(const struct pt_regs* p_regs);

asmlinkage long hacked_getdents64(const struct pt_regs* p_regs);

#endif /* _CROK_LKM_SYSTBL_HACKED_FUNCS_H_ */

