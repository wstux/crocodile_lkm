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

#ifndef _CROCODILE_LKM_SYSTBL_HOOK_FUNCS_H_
#define _CROCODILE_LKM_SYSTBL_HOOK_FUNCS_H_

#include "systbl/types.h"

asmlinkage long hack_execve(const struct pt_regs* p_regs);

#endif /* _CROCODILE_LKM_SYSTBL_HOOK_FUNCS_H_ */

