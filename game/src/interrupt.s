.section .text, "ax"
get_mtime_low:
    li a5, 4
    ecall
printf:
    ret
copyMemory:
    li a5, 50
    ecall
set_mode:
    li a5, 97
    ecall
get_controller_:
    li a5, 2
    ecall

.global get_pixel_bg_data, set_pixel_bg_controls, get_bg_palette
get_pixel_bg_data:
    li a5, 5
    ecall
set_pixel_bg_controls:
    li a5, 6
    ecall
get_bg_palette:
    li a5, 7
    ecall
