# crocodile_lkm

Hack_syscall_tbl is a LKM rootkit for Linux Kernels 4.x(>=4.4.0)/5.x

## Features
- to write;

## Install
Compile
```
make
```
Load the module(as root)
```
insmod build_release/drivers/croc/croc.ko
```

## Uninstall
Remove the module(as root)
```
rmmod croc
```

## References
1. [Wikipedia Rootkit](https://en.wikipedia.org/wiki/Rootkit)
2. [Linux Device Drivers](http://lwn.net/Kernel/LDD3/)
3. [LKM HACKING](https://web.archive.org/web/20140701183221/https://www.thc.org/papers/LKM_HACKING.html)
4. [Memset's blog](http://memset.wordpress.com/)
5. [Linux on-the-fly kernel patching without LKM](http://phrack.org/issues/58/7.html)
6. [WRITING A SIMPLE ROOTKIT FOR LINUX](https://web.archive.org/web/20160620231623/http://big-daddy.fr/repository/Documentation/Hacking/Security/Malware/Rootkits/writing-rootkit.txt)
7. [Linux Cross Reference](http://lxr.free-electrons.com/)
8. [zizzu0 LinuxKernelModules](https://github.com/zizzu0/LinuxKernelModules/)
9. [Linux Rootkits: New Methods for Kernel 5.7+](https://xcellerator.github.io/posts/linux_rootkits_11/)
10. [m0nad](https://github.com/m0nad/)
11. [stackoverflow](https://stackoverflow.com/questions/31648126/disable-write-protection-for-memory-pages-in-arm)
12. [linux kernel system call hooking example](https://stackoverflow.com/questions/2103315/linux-kernel-system-call-hooking-example)

