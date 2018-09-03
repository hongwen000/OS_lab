# OS_lab
本操作系统是2018年凌应标老师的操作系统实验课课程作业。

This OS is the course project of Operating Systems Laboratory in SYSU taught by Professor Lin Yingbiao.
## Feature

Written in C++. Completely from scratch, started with making a real mode kernel, then dove into protected mode, and reached a code quantity of 9,000.

Implemented:
- Paging
- Multiprocessing
- Semaphore
- FAT32 file system
- PS2 keyboard and ATA-IDE drivers
- A bash-like shell.

## Usage

This OS is developed under Mac OS 10.12. You must have a gcc targeting **i386-elf** to complie this OS and bochs to run it.

You can refer to 
`http://www.theairportwiki.com/index.php?title=Building_a_cross_compile_of_GCC_for_MIPS_on_OS_X` to see how to build a cross compile of GCC.

The `GCC_PREFIX` variable in Makefile should be edited before you `make`.

I used hdiutil and newfs_msdos to make a FAT32 hard disk image. As the tools might not be available in other *nix OS, you may need some alternatives.

A complete list of tools used in my development is here.

- C++ compiler：g++ 7.3.0
- linker：ld 2.30
- Binary/ELF file related：objdump 2.30, nm 2.3.0, readelf 2.3.0, strip 2.3.0
- Editor/IDE: Vim 8.0.1400, VS Code 1.21.0, Clion 2018.1
- Assembler: Nasm 2.13.02
- VM(including debugger): Bochs 2.6.9
- Build System: GNU Make 3.8.1
- Disk utilities: dd, hdiutil, newfs_msdos

## Documentation
There are 9 labs, each of them has detailed documentation provided in `reports` folder (Chinese Only).

## Reference
I refered and borrowed code from [XV6](https://github.com/mit-pdos/xv6-public) and [OS67](https://github.com/SilverRainZ/OS67) to complete my project.

The [OSDev](https://wiki.osdev.org/Expanded_Main_Page) forum helped me a lot.
