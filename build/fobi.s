
## application main entry point
.text
main:

## Load variables
lw $t0,var_xaac
move $t1,$0

## add
add $t2,$t0,$t1
sw $t2,var_a

## Load variables
lw $t0,var_xafd
move $t1,$0

## add
add $t2,$t0,$t1
sw $t2,var_b

## While Start
WhileStart_xpjd:

## Load variables
lw $t0,var_a
lw $t1,var_xure

## slt
slt $t2,$t0,$t1

## Check while condition
beqz $t2,WhileEnd_xpjd

## Load variables
lw $t0,var_a
move $t1,$0

## add
add $t2,$t0,$t1
sw $t2,var_t

## Load variables
lw $t0,var_b
move $t1,$0

## add
add $t2,$t0,$t1
sw $t2,var_t

## Load variables
lw $t0,var_a
lw $t1,var_b

## add
add $t2,$t0,$t1
sw $t2,var_b

## Load variables
lw $t0,var_t
move $t1,$0

## add
add $t2,$t0,$t1
sw $t2,var_a

## print integer
li $v0,1
lw $a0,var_a
syscall

## print string
li $v0,4
la $a0,var_xfsj
syscall
j WhileStart_xpjd

## End of while
WhileEnd_xpjd:

## tell system to terminate application
exit:
li $v0,10
syscall

## end of application main entry point

## Allocate and label memory for variables
.data
var_xaac:	.word 1
var_xafd:	.word 1
var_xure:	.word 100
var_xfsj:	.asciiz " ,"
var_a:	.word 0
var_b:	.word 0
var_t:	.word 0
