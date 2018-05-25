include gmsl
disk_index = 1
SHELL=/bin/bash
AS = nasm
ASFLAG = -f bin
user_src = $(sort $(wildcard user*.asm))
user_bin = $(user_src:.asm=.o)
user%.o : user%.asm common.asm
	$(AS) $(ASFLAG) $< -o $@
kernal_src = myos1.asm
kernal_bin = $(kernal_src:.asm=.o)
kernal : $(kernal_src)
	$(AS) $(ASFLAG) $(kernal_src) -o $(kernal_bin)
floppyfile = disk.img
clean_disk:
	dd if=/dev/zero of=$(floppyfile) bs=512 count=2880
write_kernal: clean_disk kernal
	dd if=$(kernal_bin) of=$(floppyfile) conv=notrunc
define DO_write
dd if=$(strip $(1)) of=$(floppyfile) bs=512 seek=$(strip $(disk_index)) conv=notrunc
$(eval disk_index = $(call plus,$(disk_index),1))

endef
write_all_progs: write_kernal $(user_bin)
	$(foreach user_prog, $(user_bin), $(call DO_write, $(user_prog)))
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
QEMUOPTS = -drive file=$(floppyfile),index=0,format=raw -smp $(CPUS) -m 4 $(QEMUEXTRA)

#执行qemu虚拟机
qemu: write_all_progs
	$(QEMU) -serial mon:stdio $(QEMUOPTS)

#执行qemu命令行界面
qemu-nox: write_all_progs
	$(QEMU) -nographic $(QEMUOPTS)

#生成gdb启动脚本
.gdbinit: .gdbinit.tmpl
	sed "s/localhost:1234/localhost:$(GDBPORT)/" < $^ > $@

#qemu连接gdb
qemu-gdb: .gdbinit write_all_progs
	@echo "*** Now run 'gdb'." 1>&2
	$(QEMU) -serial mon:stdio $(QEMUOPTS) -S $(QEMUGDB)

#qemu命令行界面连接gdb
qemu-nox-gdb: .gdbinit write_all_progs
	@echo "*** Now run 'gdb'." 1>&2
	$(QEMU) -nographic $(QEMUOPTS) -S $(QEMUGDB)

#启动gdb
gdb:
	/opt/cross/gcc-jos/bin/i386-jos-elf-gdb -n -x .gdbinit

#启动bochs虚拟机
bochs: write_all_progs
	export FLOPPYFILE=$(floppyfile) && bochs -q -f bochsrc

#启动bochs虚拟机 on ubuntu
bochs_ubuntu: write_all_progs
	export FLOPPYFILE=$(floppyfile) && bochs -q -f bochsrc-ubuntu

#清理
clean:
	rm *.o $(floppyfile)
