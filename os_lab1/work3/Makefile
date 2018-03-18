#目标文件名
OBJFILE = lab1

#软盘文件名
FLOPPYFILE = floppy.img

#编译规则
obj: $(OBJFILE).asm
	nasm $(OBJFILE).asm -o $(OBJFILE)

#生成软盘规则
floppy: obj
	dd if=/dev/zero of=$(FLOPPYFILE) bs=1024 count=1440
	dd if=$(OBJFILE) of=$(FLOPPYFILE)  conv=notrunc

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
QEMUOPTS = -drive file=floppy.img,index=0,format=raw -smp $(CPUS) -m 4 $(QEMUEXTRA)

#执行qemu虚拟机
qemu: floppy
	$(QEMU) -serial mon:stdio $(QEMUOPTS)

#执行qemu命令行界面
qemu-nox: $(FLOPPYFILE)
	$(QEMU) -nographic $(QEMUOPTS)

#生成gdb启动脚本
.gdbinit: .gdbinit.tmpl
	sed "s/localhost:1234/localhost:$(GDBPORT)/" < $^ > $@

#qemu连接gdb
qemu-gdb: .gdbinit $(FLOPPYFILE)
	@echo "*** Now run 'gdb'." 1>&2
	$(QEMU) -serial mon:stdio $(QEMUOPTS) -S $(QEMUGDB)

#qemu命令行界面连接gdb
qemu-nox-gdb: .gdbinit $(FLOPPYFILE)
	@echo "*** Now run 'gdb'." 1>&2
	$(QEMU) -nographic $(QEMUOPTS) -S $(QEMUGDB)

#启动gdb
gdb:
	/opt/cross/gcc-jos/bin/i386-jos-elf-gdb -n -x .gdbinit

#启动bochs虚拟机
bochs: $(FLOPPYFILE)
	export FLOPPYFILE=$(FLOPPYFILE) && bochs -f bochsrc

#清理
clean:
	rm $(OBJFILE) $(FLOPPYFILE)
