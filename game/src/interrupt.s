.section .text, "ax"

.global  get_mtime_low, printf, malloc, memcpy, set_mode, get_controller
get_mtime_low:
    li a5, 0
    ecall
printf:
    li a5, 1
    ecall
memcpy:
    li a5, 2
    ecall
set_mode:
    li a5, 3
    ecall
get_controller:
    li a5, 4
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
