.section .text

.globl mix_test
mix_test:
    li a0, 0x1001000
    addi a1, tp, 0x3c0
    lw a2, (a0)
    lw a3, 4(a0)
    lw a4, 8(a0)
    lw a5, (a1)
    lw a5, 4(a1)
    sw a2, (a1)
    sw a3, 4(a1)
    sw a4, 8(a1)
    lw a5, (a1)
    lw a5, 4(a1)
    sb a2, (a1)
    lbu a2, 1(a1)
    sh a3, (a1)
    lhu a2, 2(a1)
    ret

.globl load_testb
load_testb:
    li a0, 0x1001000
    addi a1, tp, 0x3c0
.rept 100
    lbu a2, (a0)
    sw a2, (a1)
.endr
    ret

.globl load_testh
load_testh:
    li a0, 0x1001000
    addi a1, tp, 0x3c0
.rept 100
    lhu a2, (a0)
    sw a2, (a1)
.endr
    ret

.globl load_test
load_test:
    li a0, 0x1001000
    addi a1, tp, 0x3c0
.rept 100
    lw a2, (a0)
    sw a2, (a1)
.endr
    ret

.globl load_test2
load_test2:
    li a0, 0x1001000
    addi a1, tp, 0x3c0
.rept 100
    lw2 a2, (a0)
    sw2 a2, (a1)
.endr
    ret

.globl load_test4
load_test4:
    li a0, 0x1001000
    addi a1, tp, 0x3c0
.rept 100
    lw4 a2, (a0)
    sw4 a2, (a1)
.endr
    ret

.globl store_testb
store_testb:
    li a0, 0x1001000
    addi a1, tp, 0x3c0
.rept 100
    lw a2, (a1)
    sb a2, (a0)
.endr
    ret

.globl store_testh
store_testh:
    li a0, 0x1001000
    addi a1, tp, 0x3c0
.rept 100
    lw a2, (a1)
    sh a2, (a0)
.endr
    ret

.globl store_test
store_test:
    li a0, 0x1001000
    addi a1, tp, 0x3c0
.rept 100
    lw a2, (a1)
    sw a2, (a0)
.endr
    ret

.globl store_test2
store_test2:
    li a0, 0x1001000
    addi a1, tp, 0x3c0
.rept 100
    lw a2, (a1)
    sw2 a2, (a0)
.endr
    ret

.globl store_test4
store_test4:
    li a0, 0x1001000
    addi a1, tp, 0x3c0
.rept 100
    lw a2, (a1)
    sw4 a2, (a0)
.endr
    ret
