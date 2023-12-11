.section .text, "ax"
.global _interrupt_handler, InitThread, SwitchThread, grid
_interrupt_handler:
    addi    sp,sp,-40
    sw        ra,36(sp)
    sw        t0,32(sp)
    sw        t1,28(sp)
    sw        t2,24(sp)
    sw        a0,20(sp)
    sw        a1,16(sp)
    sw        a2,12(sp)
    sw        a3,8(sp)
    sw        a4,4(sp)
    sw        a5,0(sp)
    call    c_interrupt_handler
    lw        ra,36(sp)
    lw        t0,32(sp)
    lw        t1,28(sp)
    lw        t2,24(sp)
    lw        a0,20(sp)
    lw        a1,16(sp)
    lw        a2,12(sp)
    lw        a3,8(sp)
    lw        a4,4(sp)
    lw        a5,0(sp)
    addi    sp,sp,40
    mret
InitThread:
    addi    a0,a0,-52
    sw        a1,48(a0)
    sw        zero,44(a0)
    sw        zero,40(a0)
    sw        zero,36(a0)
    sw        zero,32(a0)
    sw        zero,28(a0)
    sw        zero,24(a0)
    sw        a2,20(a0)
    sw        zero,16(a0)
    sw        zero,12(a0)
    sw        zero,8(a0)
    sw        zero,4(a0)
    sw        zero,0(a0)
    ret
SwitchThread:
    addi    sp,sp,-52
    sw        ra,48(sp)
    sw        tp,44(sp)
    sw        t0,40(sp)
    sw        t1,36(sp)
    sw        t2,32(sp)
    sw        s0,28(sp)
    sw        s1,24(sp)
    sw        a0,20(sp)
    sw        a1,16(sp)
    sw        a2,12(sp)
    sw        a3,8(sp)
    sw        a4,4(sp)
    sw        a5,0(sp)

    sw      sp,0(a0)
    mv      sp,a1

    lw        ra,48(sp)
    lw        tp,44(sp)
    lw        t0,40(sp)
    lw        t1,36(sp)
    lw        t2,32(sp)
    lw        s0,28(sp)
    lw        s1,24(sp)
    lw        a0,20(sp)
    lw        a1,16(sp)
    lw        a2,12(sp)
    lw        a3,8(sp)
    lw        a4,4(sp)
    lw        a5,0(sp)
    addi    sp,sp,52
    ret
.section .text, "ax"
.global getTicks, getController, setSmallSpriteControl, toggleVideoInterrupt, getMachineTime, getVideoTime, startTimer, pauseTimer, resumeTimer, stopTimer, placeLargeSpriteOnGrid, placeSmallSpriteOnGrid, drawPixelText, placeSmallSprite, placeLargeSprite, clearScreen, drawLargeSprite, setPaletteLarge, initGrid, drawSmallSprite, setPaletteSmall, updateCell, getCellFromCoords, getCellType, getLargeCorner1, getLargeCorner2, getCellTypeFromCoords, deleteSmallSprite, deleteLargeSprite, getTime, isTimerOn, getPausedTime, toggleCommandInterrupt, moveLargeSprite, numLargeFreeSpaces, numSmallFreeSpaces, numLargeUsedSpaces, numSmallUsedSpaces, moveSmallSprite, createTimer, destroyTimer, getSizeLargeSprite, getSizeSmallSprite, getSmallCorner1, getSmallCorner2, collided, delay, allocateMemory, fillMemory, printNum, copyMemory, getSquareDims, getRowSize, changeCanvas, placeMediumSprite, drawMediumSprite, setPaletteMedium, deleteMediumSprite, moveMediumSprite, numMediumFreeSpaces, numMediumUsedSpaces, getSizeMediumSprite, getMediumCorner1, getMediumCorner2, placeMediumSpriteOnGrid, setBackgroundControlPixel, setBackgroundControlTile, deallocate, setBackgroundPalette, drawBackgroundPixels, mergeXY, spinlockInit, spinlockLock, spinlockUnlock, getSmallControl, getSmallPalette, getMediumControl, getMediumPalette, getLargeControl, getLargePalette, print, drawChar, createUser, deleteUser, setHighScore, setName, getName, getHighScore, setCurrentScore, getCurrentScore, getBackgroundControl, getBackgroundPalette, colorGridCell, deleteBackgroundSprite, clearBackground, removeBGPixels, removeBGPixelsByGrid, switchMode, clearAllText, randomNumber, setSeed
getTicks:
    li a5, 1
    ecall
getController:
    li a5, 2
    ecall
toggleVideoInterrupt:
    li a5, 3
    ecall
getMachineTime:
    li a5, 4
    ecall
getVideoTime:
    li a5, 5
    ecall
startTimer:
    li a5, 6
    ecall
pauseTimer:
    li a5, 7
    ecall
resumeTimer:
    li a5, 8
    ecall
stopTimer:
    li a5, 9
    ecall
placeLargeSpriteOnGrid:
    li a5, 10
    ecall
placeSmallSpriteOnGrid:
    li a5, 11
    ecall
drawPixelText:
    li a5, 12
    ecall
placeSmallSprite:
    li a5, 13
    ecall
placeLargeSprite:
    li a5, 14
    ecall
clearScreen:
    li a5, 15
    ecall
drawLargeSprite:
    li a5, 16
    ecall
setPaletteLarge:
    li a5, 17
    ecall
initGrid:
    li a5, 18
    ecall
drawSmallSprite:
    li a5, 19
    ecall
setPaletteSmall:
    li a5, 20
    ecall
updateCell:
    li a5, 21
    ecall
getCellFromCoords:
    li a5, 22
    ecall
getCellType:
    li a5, 23
    ecall
getLargeCorner1:
    li a5, 24
    ecall
getLargeCorner2:
    li a5, 25
    ecall
getCellTypeFromCoords:
    li a5, 26
    ecall
deleteSmallSprite:
    li a5, 27
    ecall
deleteLargeSprite:
    li a5, 28
    ecall
getTime:
    li a5, 29
    ecall
isTimerOn:
    li a5, 30
    ecall
getPausedTime:
    li a5, 31
    ecall
toggleCommandInterrupt:
    li a5, 32
    ecall
moveLargeSprite:
    li a5, 33
    ecall
numLargeFreeSpaces:
    li a5, 34
    ecall
numSmallFreeSpaces:
    li a5, 35
    ecall
numLargeUsedSpaces:
    li a5, 36
    ecall
numSmallUsedSpaces:
    li a5, 37
    ecall
moveSmallSprite:
    li a5, 38
    ecall
createTimer:
    li a5, 39
    ecall
destroyTimer:
    li a5, 40
    ecall
getSizeLargeSprite:
    li a5, 41
    ecall
getSizeSmallSprite:
    li a5, 42
    ecall
getSmallCorner1:
    li a5, 43
    ecall
getSmallCorner2:
    li a5, 44
    ecall
collided:
    li a5, 45
    ecall
delay:
    li a5, 46
    ecall
allocateMemory:
    li a5, 47
    ecall
fillMemory:
    li a5, 48
    ecall
printNum:
    li a5, 49
    ecall
copyMemory:
    li a5, 50
    ecall
getSquareDims:
    li a5, 51
    ecall
getRowSize:
    li a5, 52
    ecall
changeCanvas:
    li a5, 53
    ecall
placeMediumSprite:
    li a5, 54
    ecall
drawMediumSprite:
    li a5, 55
    ecall
setPaletteMedium:
    li a5, 56
    ecall
deleteMediumSprite:
    li a5, 57
    ecall
moveMediumSprite:
    li a5, 58
    ecall
numMediumFreeSpaces:
    li a5, 59
    ecall
numMediumUsedSpaces:
    li a5, 60
    ecall
getSizeMediumSprite:
    li a5, 61
    ecall
getMediumCorner1:
    li a5, 62
    ecall
getMediumCorner2:
    li a5, 63
    ecall
placeMediumSpriteOnGrid:
    li a5, 64
    ecall
setBackgroundControlPixel:
    li a5, 65
    ecall
setBackgroundControlTile:
    li a5, 66
    ecall
deallocate:
    li a5, 67
    ecall
setBackgroundPalette:
    li a5, 68
    ecall
drawBackgroundPixels:
    li a5, 69
    ecall
mergeXY:
    li a5, 70
    ecall
spinlockInit:
    li a5, 71
    ecall
spinlockLock:
    li a5, 72
    ecall
spinlockUnlock:
    li a5, 73
    ecall
getSmallControl:
    li a5, 74
    ecall
getSmallPalette:
    li a5, 75
    ecall
getMediumControl:
    li a5, 76
    ecall
getMediumPalette:
    li a5, 77
    ecall
getLargeControl:
    li a5, 78
    ecall
getLargePalette:
    li a5, 79
    ecall
print:
    li a5, 80
    ecall
drawChar:
    li a5, 81
    ecall
createUser:
    li a5, 82
    ecall
deleteUser:
    li a5, 83
    ecall
setHighScore:
    li a5, 84
    ecall
setName:
    li a5, 85
    ecall
getName:
    li a5, 86
    ecall
getHighScore:
    li a5, 87
    ecall
setCurrentScore:
    li a5, 88
    ecall
getCurrentScore:
    li a5, 89
    ecall
getBackgroundControl:
    li a5, 90
    ecall
getBackgroundPalette:
    li a5, 91
    ecall
colorGridCell:
    li a5, 92
    ecall
deleteBackgroundSprite:
    li a5, 93
    ecall
clearBackground:
    li a5, 94
    ecall
removeBGPixels:
    li a5, 95
    ecall
removeBGPixelsByGrid:
    li a5, 96
    ecall
switchMode:
    li a5, 97
    ecall
clearAllText:
    li a5, 98
    ecall
randomNumber:
    li a5, 99
    ecall
setSeed:
    li a5, 100
    ecall
.end
