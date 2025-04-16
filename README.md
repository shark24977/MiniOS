# MiniOS
一个面向RISCV的MiniOS。
# 环境
Ubuntu 22.04 环境，执行如下命令：
```
$ sudo apt update
$ sudo apt install build-essential gcc make perl dkms git gcc-riscv64-unknown-elf gdb-multiarch qemu-system-mis
```
# 编译
```
$ cd MiniOS/kernel/src
$ make
$ make run
```

本项目为在校期间自学使用，现有时间，整理开源，参考了如下网络资源，在此表示感谢 :)
- RVOS：<https://gitee.com/unicornx/riscv-operating-system-mooc>
- mini-riscv-os: <https://github.com/cccriscv/mini-riscv-os>
- Xv6, a simple Unix-like teaching operating system：<https://pdos.csail.mit.edu/6.828/2020/xv6.html>

