set(CMAKE_C_COMPILER_TARGET riscv32-unknown-elf)


set(LDFILE riscv32-console.ld)

add_executable(cartrige main.c)

target_link_options(cartrige PRIVATE -Wl --gcc-sections -Wl, -T $(LDFILE))
target_compile_options(cartrige PRIVATE -ffreestanding -nostartfiles -nostdlib -nodefaultlibs)


