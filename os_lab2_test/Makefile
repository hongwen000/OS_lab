AS = nasm
ASFLAG = -f bin
user_src = $(sort $(wildcard user*.asm))
user_bin = $(user_src:.asm=.o)
user%.o : user%.asm
	$(AS) $(ASFLAG) $< -o $@
kernal_src = myos1.asm
kernal_bin = $(kernal_src:.asm=.o)
kernal : $(kernal_src)
	$(AS) $(ASFLAG) $(kernal_src) -o $(kernal_bin)
floppyfile = disk.img
clean_disk:
	dd if=/dev/zero of=$(floppyfile) bs=512 count=2880
write_kernal: clean_disk
	dd if=$(kernal_bin) of=$(floppyfile) conv=notrunc
define DO_write
dd if=$(strip $(1)) of=$(floppyfile) conv=notrunc 

endef
write_all_progs: write_kernal $(user_bin)
	$(foreach user_prog, $(user_bin), $(call DO_write, $(user_prog)))
