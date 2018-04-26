include mkinc/gmsl

.SUFFIXES:

build_dir = build

disk_index = 1

SHELL=/bin/bash

AS = nasm

ASBIN = -f bin

ASELF = -f elf32

AR = $(GCC_PREFIX)ar

all:
	#make clean
	#make bochs
	make write_all_progs


GCC_PREFIX = /opt/cross/gcc-i386/bin/i386-elf-

OBJDUMP = $(GCC_PREFIX)objdump

CC = $(GCC_PREFIX)gcc
CFLAGS =  	-m32 -march=i486 \
			-nostdlib -ffreestanding \
			-lgcc \
			-fno-builtin -O0 -fno-omit-frame-pointer \
			-Wall -Wno-format -Wno-unused -Wno-write-strings -Werror \
			-fno-tree-ch -fno-stack-protector \
			-std=gnu11  -g \
			-I . -I include

CXX = $(GCC_PREFIX)g++

CXXFLAGS =  -m32 -march=i386 \
			-nostdlib -ffreestanding \
			-lgcc \
			-fno-builtin -O0 -fno-omit-frame-pointer \
			-Wall -Wno-format -Wno-unused -Wno-write-strings -Werror \
			-fno-tree-ch -fno-stack-protector \
			-std=c++17  -g \
			-I . -I include

LD  = $(GCC_PREFIX)ld

LDFLAGS =  -static -nostdlib --nmagic

bootloader_src = boot/bootloader.asm

bootloader_bin = $(build_dir)/$(bootloader_src:.asm=.bin)

include boot/Makefrag

kernel_obj = $(build_dir)/kernel/kernel_start.o \
			 $(build_dir)/kernel/kernel_main.o \
			 $(build_dir)/driver/vga.o \
			 $(build_dir)/driver/kb.o \
			 $(build_dir)/driver/ide.o \
			 $(build_dir)/driver/cmos.o \
			 $(build_dir)/proc/proc.o \


include kernel/Makefrag
include driver/Makefrag
include proc/Makefrag

-include $(wildcard $(build_dir)/kernel/*.d)

kernel_bin = $(build_dir)/kernel/kernel.bin

# asm_src = kernel_lib/asm_lib.asm

# asm_obj = $(build_dir)/$(asm_src:.asm=.o)

# c_obj   = $(build_dir)/kernel_lib/sys_io.o \
# 		  $(build_dir)/kernel_lib/cmos.o

kernel_lib_obj = $(build_dir)/kernel_lib/pm_utility.o \
		         $(build_dir)/kernel_lib/debug_printf.o \
				 $(build_dir)/kernel_lib/gdt.o \
				 $(build_dir)/kernel_lib/idt.o \
				 $(build_dir)/kernel_lib/isr.o \
				 $(build_dir)/kernel_lib/irq.o \
				 $(build_dir)/kernel_lib/fault.o \
				 $(build_dir)/kernel_lib/sys_utility.o \
				 $(build_dir)/kernel_lib/sys_utility_asm.o




kernel_lib = $(build_dir)/kernel_lib/libkernel.a

# $(kernel_lib): $(asm_obj) $(c_obj)
# 	$(AR) rcs $@ $(asm_obj) $(c_obj)

$(kernel_lib): $(kernel_lib_obj)
	$(AR) rcs $@ $(kernel_lib_obj)

include kernel_lib/Makefrag

libc_obj = $(build_dir)/libc/string.o \
		   $(build_dir)/libc/ctype.o \
		   $(build_dir)/libc/stdlib.o \
		   $(build_dir)/libc/stdio.o \
		   $(build_dir)/libc/time.o


libc = $(build_dir)/libc/libc.a

$(libc) : $(libc_obj)
	$(AR) rcs $@ $(libc_obj) 

include libc/Makefrag

obj_link_list:= $(kernel_obj)

lib_ar_list := $(libc) $(kernel_lib)

LDFLAGS += -L $(build_dir)/libc -L $(build_dir)/kernel_lib

LDFLAGS_LIB =  -lkernel -lc

kernel_mod : $(wildcard kernel/*.h)

NM = $(GCC_PREFIX)nm
kernel_sym = $(build_dir)/kernel/kernel.map

kernel_img : $(kernel_mod) $(obj_link_list) $(lib_ar_list)
	$(LD)  $(LDFLAGS) -Tlinker.ld --oformat binary $(obj_link_list) $(LDFLAGS_LIB) -o $(kernel_bin)
	$(LD)  $(LDFLAGS) -Tlinker.ld $(lib_search_list) $(obj_link_list) $(LDFLAGS_LIB) -o $(kernel_bin).elf
	$(NM)  $(kernel_bin).elf | awk '{print "0x"$$1,$$3}' > $(kernel_sym)
	$(OBJDUMP) --adjust-vma=0x7e00 -D -Mintel,i386 -b binary -m i386 $(kernel_bin) >kernel_bin_dump
	$(OBJDUMP) -S $(kernel_bin).elf > kernel_elf_dump
rm_kernel:
	rm -f $(kernel_bin)
	rm -f $(kernel_bin).elf

disk_index = 64

define DO_write
dd if=$(strip $(1)) of=$(hard_disk_file) bs=512 seek=$(strip $(disk_index)) conv=notrunc
$(eval disk_index = $(call plus,$(disk_index),64))

endef

include usr/Makefrag



user_bin = $(build_dir)/usr/help.bin   \
           $(build_dir)/usr/record.bin \
           $(build_dir)/usr/bc.bin \
           $(build_dir)/usr/sleep.bin  \
           $(build_dir)/usr/test.bin \
           $(build_dir)/usr/usr1.bin \
           $(build_dir)/usr/usr2.bin \
           $(build_dir)/usr/usr3.bin \
           $(build_dir)/usr/usr4.bin \
           $(build_dir)/usr/user1.bin  \
           $(build_dir)/usr/user2.bin  \
           $(build_dir)/usr/user3.bin  \
           $(build_dir)/usr/user4.bin

write_usr_progs: $(user_bin)
	$(foreach user_prog, $(user_bin), $(call DO_write, $(user_prog)))

floppyfile = $(build_dir)/disk.img
hard_disk_file = $(build_dir)/hard_disk.img

clean_disk:
	@mkdir -p build
	dd if=/dev/zero of=$(floppyfile) bs=512 count=2880
	rm -f $(hard_disk_file)
	bximage $(hard_disk_file) -hd=10M -imgmode=flat -mode=create -q

write_kernel: clean_disk kernel_img $(bootloader_bin)
	dd if=$(bootloader_bin) of=$(floppyfile) conv=notrunc
	dd if=$(kernel_bin) of=$(floppyfile) bs=512 seek=1 conv=notrunc

write_all_progs: write_kernel write_usr_progs



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
QEMUOPTS = -fda $(floppyfile) -hda $(hard_disk_file) -smp $(CPUS) -m 32 $(QEMUEXTRA)

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
qemu-gdb: .gdbinit write_all_progs 
	@echo "*** Now run 'gdb'." 1>&2
	$(QEMU) -serial mon:stdio $(QEMUOPTS) -S $(QEMUGDB)

#qemu命令行界面连接gdb
qemu-nox-gdb: .gdbinit write_all_progs 
	@echo "*** Now run 'gdb'." 1>&2
	$(QEMU) -nographic $(QEMUOPTS) -S $(QEMUGDB)

#启动gdb
gdb: .gdbinit
	/opt/cross/gcc-i386/bin/i386-elf-gdb -n -x .gdbinit

#启动bochs虚拟机
bochs: write_all_progs
	export FLOPPYFILE=$(floppyfile) && bochs -qf bochsrc

#清理
clean:
	rm -rf build
	rm -f kernel_bin_dump
	rm -f kernel_elf_dump
