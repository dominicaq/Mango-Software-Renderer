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
