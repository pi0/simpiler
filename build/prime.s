
## application main entry point
.text
main:

## While Start
WhileStart_xpye:

## Load variables
lw $t0,var_a
lw $t1,var_xakg

## slt
slt $t2,$t0,$t1

## Check while condition
beqz $t2,WhileEnd_xpye

## Load variables
lw $t0,var_xkgi
move $t1,$0

## add
add $t2,$t0,$t1
sw $t2,var_i

## Load variables
lw $t0,var_xafkb
move $t1,$0

## add
add $t2,$t0,$t1
sw $t2,var_is_prime

## While Start
WhileStart_xpolb:

## Load variables
lw $t0,var_i
lw $t1,var_a

## slt
slt $t2,$t0,$t1

## Check while condition
beqz $t2,WhileEnd_xpolb

## Load variables
lw $t0,var_a
lw $t1,var_i

## rem
rem $t2,$t0,$t1
sw $t2,var_mod

## Load variables
lw $t0,var_mod
lw $t1,var_xapqb

## seq
seq $t2,$t0,$t1

## start of if
beqz $t2,IFElse_xpepb

## Load variables
lw $t0,var_xpjsb
move $t1,$0

## add
add $t2,$t0,$t1
sw $t2,var_is_prime
j IFEnd_xpepb

## IF Else part
IFElse_xpepb:

## End of if
IFEnd_xpepb:

## Load variables
lw $t0,var_i
lw $t1,var_xfxtb

## add
add $t2,$t0,$t1
sw $t2,var_i
j WhileStart_xpolb

## End of while
WhileEnd_xpolb:

## Load variables
lw $t0,var_is_prime
lw $t1,var_xkbwc

## seq
seq $t2,$t0,$t1

## start of if
beqz $t2,IFElse_xakvc

## print integer
li $v0,1
lw $a0,var_a
syscall

## print string
li $v0,4
la $a0,var_xfiac
syscall
j IFEnd_xakvc

## IF Else part
IFElse_xakvc:

## End of if
IFEnd_xakvc:

## Load variables
lw $t0,var_a
lw $t1,var_xfidc

## add
add $t2,$t0,$t1
sw $t2,var_a
j WhileStart_xpye

## End of while
WhileEnd_xpye:

## tell system to terminate application
exit:
li $v0,10
syscall

## end of application main entry point

## Allocate and label memory for variables
.data
var_xfse:	.word 2
var_xakg:	.word 50
var_xkgi:	.word 2
var_xafkb:	.word 1
var_xapqb:	.word 0
var_xpjsb:	.word 0
var_xfxtb:	.word 1
var_xkbwc:	.word 1
var_xfiac:	.asciiz "\n"
var_xfidc:	.word 1
var_a:	.word 0
var_i:	.word 0
var_is_prime:	.word 0
var_mod:	.word 0
