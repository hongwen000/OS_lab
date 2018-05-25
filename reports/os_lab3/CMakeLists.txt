CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)
project (HHOS C CXX)
set (Tutorial_VERSION_MAJOR 1)
set (Tutorial_VERSION_MINOR 3)

ADD_SUBDIRECTORY(boot)
ADD_SUBDIRECTORY(kernel)
ADD_SUBDIRECTORY(basic_lib)

enable_language(ASM_NASM)

set(floopyfile disk.img)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/disk.img
    DEPENDS kernel.bin bootlader.bin
	COMMAND dd if=/dev/zero of=${floppyfile} bs=512 count=2880
    )