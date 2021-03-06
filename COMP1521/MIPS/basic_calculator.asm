#A MIPS program that implements a basic calculator

    .data
msg1: .asciiz "Enter the first number \n"
msgOp: .asciiz "Enter an operation 1 => + , 2 => - , 3 => *, 4=> / \n"
userOp: .space 3
msg2: .asciiz "Enter the second number \n"
msg3: .asciiz "The result is: \n"

plus: .word 1
minus: .word 2
times: .word 3
divide: .word 4


    .text
    .globl main

main:
    #print the first message
    li $v0, 4
    la $a0, msg1
    syscall

    #Scan the first number and store in t0
    li $v0, 5
    syscall
    move $t0, $v0

    #Ask for the operation
    li $v0, 4
    la $a0, msgOp
    syscall

    #Store the operation
    li $v0, 5
    syscall
    move $t2, $v0

    #Ask for the second number
    li $v0, 4
    la $a0, msg2
    syscall

    #Scan the second number and store in t1
    li $v0, 5
    syscall
    move $t1, $v0

    #Check if '+' is in t2... YES!
    #li $v0, 4
    #move $a0, $t2
    #syscall

    li $v0, 4
    la $a0, msg3
    syscall

    #If statements are here...aka branching
    move $a0, $t2

    lw $a1, plus
    beq $a1, $a0, Add

    lw $a1, minus
    beq $a1, $a0, Subtract

    lw $a1, times
    beq $a1, $a0, Multiply

    lw $a1, divide
    beq $a1, $a0, Divide
    #If there are no valid operations, terminate
    bne $a1, $a0, Exit


Add:
    add $t7, $t1, $t0
    beq $a0, $a0, Exit

Subtract:
    sub $t7, $t0, $t1
    beq $a0, $a0, Exit

Multiply:
    mul $t7, $t1, $t0
    beq $a0, $a0, Exit
Divide:
    div $t0, $t1
    mfhi $t4
    mflo $t3
    #display quotient
    li $v0, 1
    move $a0, $t3
    syscall
    #display remainder
    li $v0, 1
    move $a0, $t4
    syscall

    li $v0, 10
    syscall

    #display the sum

Exit:
    li $v0, 1
    move $a0, $t7
    syscall

    li $v0, 10
    syscall
