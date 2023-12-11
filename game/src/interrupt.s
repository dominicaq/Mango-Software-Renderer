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

.global get_lg_sp_data, get_md_sp_data, get_sm_sp_data
get_lg_sp_data:
    li a5, 8
    ecall
get_md_sp_data:
    li a5, 9
    ecall
get_sm_sp_data:
    li a5, 10
    ecall

.global get_lg_sp_palette, get_md_sp_palette, get_sm_sp_palette
get_lg_sp_palette:
    li a5, 11
    ecall
get_md_sp_palette:
    li a5, 12
    ecall
get_sm_sp_palette:
    li a5, 13
    ecall
    
.global set_lg_sp_controls, set_md_sp_controls, set_sm_sp_controls
set_lg_sp_controls:
    li a5, 14
    ecall
set_md_sp_controls:
    li a5, 15
    ecall
set_sm_sp_controls:
    li a5, 16
    ecall
