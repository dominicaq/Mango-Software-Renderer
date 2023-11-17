set(LDFILE riscv32-console.ld)

add_executable(c_software_renderer main.c)

target_link_options(c_software_renderer PRIVATE -Wl --gcc-sections -Wl, -T $(LDFILE))
target_compile_options(c_software_renderer PRIVATE -ffreestanding -nostartfiles -nostdlib -nodefaultlibs)


