.section .text, "ax"
.global GetTicks, GetController, GetCmd, clear_cmd 
.global load_sprite_sys, load_sprite_data_sys, load_palette_sys, display_sprite_sys 
.global delete_sprite_sys, change_sprite_palette_sys, load_background_palette_sys, load_background_data_pixel_sys
.global load_background_sys, delete_background_sys
.global threadCreate, threadYield, threadExit
.global get_controller_sys
.global create_square_sys
.global malloc_sys, free_sys

GetTicks:
    li a5, 1
    ecall
clear_cmd:
    li a5, 2
    ecall
GetCmd:
    li a5, 3
    ecall
load_sprite_sys:
    li a5, 4
    ecall
load_sprite_data_sys:
    li a5, 5
    ecall
load_palette_sys:
    li a5, 6
    ecall
display_sprite_sys:
    li a5, 7
    ecall
delete_sprite_sys:
    li a5, 8
    ecall
change_sprite_palette_sys:
    li a5, 9
    ecall
load_background_palette_sys:
    li a5, 10
    ecall
load_background_data_pixel_sys:
    li a5, 11
    ecall
load_background_sys:
    li a5, 12
    ecall
delete_background_sys:
    li a5, 13
    ecall

threadCreate:
    li a5, 14
    ecall
threadYield:
    mv a0, ra
    jal _saveTCBtoStack
    li a5, 15
    ecall
threadExit:
    li a5, 16
    ecall

get_controller_sys:
    li a5, 20
    ecall

malloc_sys:
    li a5, 30
    ecall
free_sys:
    li a5, 31
    ecall

create_square_sys:
    li a5, 35
    ecall
.section .text, "ax"
.global _saveTCBtoStack

_saveTCBtoStack:
    addi	sp,sp,-52
    
    sw	    a0,48(sp)   # arg0 contains ra to jump back to in execution
    
    sw	    tp,44(sp)
    sw	    t0,40(sp)
    sw	    t1,36(sp)
    sw	    t2,32(sp)
    sw	    s0,28(sp)
    sw	    s1,24(sp)
    sw	    a0,20(sp)
    sw	    a1,16(sp)
    sw	    a2,12(sp)
    sw	    a3,8(sp)
    sw	    a4,4(sp)
    sw	    a5,0(sp)

    mv      a0, sp       # Moving stack pointer to arg0

    # mv      gp, a3    # Set global pointer back to cartridge

    ret
