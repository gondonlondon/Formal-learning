main:
    la   $a0, string0   # printf("calling function f\n");
    li   $v0, 4
    syscall

    addi $sp, $sp, -4
    sw $ra, ($sp)

    jal  f              # set $ra to following address
    jal  f              # set $ra to following address
    jal  f              # set $ra to following address
    jal  f              # set $ra to following address

    la   $a0, string1   # printf("back from function f\n");
    li   $v0, 4
    syscall

    lw $ra, ($sp)
    addi $sp, $sp, -4

    li   $v0, 10         # fails because $ra changes since main called
    jr   $ra            # return from function main


f:
    la   $a0, string2   # printf("in function f\n");
    li   $v0, 4
    syscall
    jr   $ra            # return from function f


    .data
string0:
    .asciiz "calling function f\n"
string1:
    .asciiz "back from function f\n"
string2:
    .asciiz "in function f\n"
