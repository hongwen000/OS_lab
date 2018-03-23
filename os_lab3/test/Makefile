include mkinc/gmsl

.SUFFIXES:

build_dir = build

disk_index = 1

SHELL=/bin/bash

AS = nasm

ASBIN = -f bin

ASELF = -f elf32

GCC_PREFIX = /opt/cross/gcc-i386/bin/i386-elf-

CC = $(GCC_PREFIX)gcc
CFLAGS =  	-m32 -march=i386 \
			-nostdlib -ffreestanding \
			-mpreferred-stack-boundary=2 -lgcc \
			-fno-builtin -Os -fno-omit-frame-pointer \
			-Wall -Wno-format -Wno-unused -Wno-write-strings -Werror \
			-fno-tree-ch -fno-stack-protector \
			-std=gnu11  -g \
			-I . -I include

CXX = $(GCC_PREFIX)g++

CXXFLAGS =  -m32 -march=i386 \
			-nostdlib -ffreestanding \
			-mpreferred-stack-boundary=2 -lgcc \
			-fno-builtin -Os -fno-omit-frame-pointer \
			-Wall -Wno-format -Wno-unused -Wno-write-strings -Werror \
			-fno-tree-ch -fno-stack-protector \
			-std=c++17  -g \
			-I . -I include

LD  = $(GCC_PREFIX)ld

LDFLAGS =  -static -Tlinker.ld -nostdlib --nmagic

bootloader_src = boot/bootloader.asm

bootloader_bin = $(build_dir)/$(bootloader_src:.asm=.bin)

include boot/Makefrag


kernel_obj = $(build_dir)/kernel/kernel_start.o \
			 $(build_dir)/kernel/kernel_main.o

include kernel/Makefrag

kernel_bin = $(build_dir)/kernel/kernel.bin

asm_src = basic_lib/asm_lib.asm

asm_obj = $(build_dir)/$(asm_src:.asm=.o)

c_src   = basic_lib/c_lib.c

c_obj = $(build_dir)/$(c_src:.c=.o)

include basic_lib/Makefrag

CRTI_OBJ=$(build_dir)/kernel/crti.o
CRTBEGIN_OBJ:=$(shell $(CC) $(CFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ:=$(shell $(CC) $(CFLAGS) -print-file-name=crtend.o)
CRTN_OBJ=$(build_dir)/kernel/crtn.o
OBJS = $(kernel_obj) $(asm_obj) $(c_obj) 
OBJ_LINK_LIST:=$(CRTI_OBJ) $(CRTBEGIN_OBJ) $(OBJS) $(CRTEND_OBJ) $(CRTN_OBJ)

kernel_img : $(OBJ_LINK_LIST)
	$(LD)  $(LDFLAGS) --oformat binary $(OBJ_LINK_LIST) -o $(kernel_bin)

kernel_elf : $(OBJ_LINK_LIST)
	$(LD)  $(LDFLAGS) $(OBJ_LINK_LIST) -o $(kernel_bin).elf

#kernel_elf : $(kernel_obj) $(asm_obj) $(c_obj)
#	$(LD)  $(LDFLAGS) $(kernel_obj) $(asm_obj) $(c_obj) -o $(kernel_bin).elf

floppyfile = $(build_dir)/disk.img

clean_disk:
	@mkdir -p build
	dd if=/dev/zero of=$(floppyfile) bs=512 count=2880

write_kernel: clean_disk kernel_img $(bootloader_bin)
	dd if=$(bootloader_bin) of=$(floppyfile) conv=notrunc
	dd if=$(kernel_bin) of=$(floppyfile) bs=512 seek=1 conv=notrunc

write_all_progs: write_kernel

#指定qemu程序
QEMU = qemu-system-i386

#随机生成一个gdb端口
GDBPORT = $(shell expr `id -u` % 5000 + 25000)

#qemu连接gdb指令
QEMUGDB = $(shell echo "-gdb tcp::$(GDBPORT)";)

#虚拟机CPU数
ifndef CPUS
CPUS := 1
endif

#qemu执行参数
QEMUOPTS = -fda $(floppyfile) -smp $(CPUS) -m 4 $(QEMUEXTRA)

#执行qemu虚拟机
qemu: write_all_progs
	$(QEMU) -serial mon:stdio $(QEMUOPTS)

#执行qemu命令行界面
qemu-nox: write_all_progs
	$(QEMU) -nographic $(QEMUOPTS)

#生成gdb启动脚本
.gdbinit: gdbinit.part gdbdash
	cat gdbdash > $@
	sed "s/localhost:1234/localhost:$(GDBPORT)/" < gdbinit.part >> $@

#qemu连接gdb
qemu-gdb: .gdbinit write_all_progs kernel_elf
	@echo "*** Now run 'gdb'." 1>&2
	$(QEMU) -serial mon:stdio $(QEMUOPTS) -S $(QEMUGDB)

#qemu命令行界面连接gdb
qemu-nox-gdb: .gdbinit write_all_progs kernel_elf
	@echo "*** Now run 'gdb'." 1>&2
	$(QEMU) -nographic $(QEMUOPTS) -S $(QEMUGDB)

#启动gdb
gdb: .gdbinit
	/opt/cross/gcc-i386/bin/i386-elf-gdb -n -x .gdbinit

#启动bochs虚拟机
bochs: clean write_all_progs
	export FLOPPYFILE=$(floppyfile) && bochs -q -f bochsrc

dump: clean kernel_img kernel_elf
	objdump --adjust-vma=0x7e00 -D -Mintel,i386 -b binary -m i386 $(kernel_bin) >kernel_bin_dump
	objdump -D $(kernel_bin).elf > kernel_elf_dump
#清理
clean:
	rm -rf build | mkdir -p build
