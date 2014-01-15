
## application main entry point
.text
main:

## Load variables
lw $t0,var_xkld
move $t1,$0

## add
add $t2,$t0,$t1
sw $t2,var_c

## Load variables
lw $t0,var_xaue
move $t1,$0

## add
add $t2,$t0,$t1
sw $t2,var_a

## Load variables
lw $t0,var_xpef
move $t1,$0

## add
add $t2,$t0,$t1
sw $t2,var_b

## While Start
WhileStart_xakg:

## Load variables
lw $t0,var_c
lw $t1,var_xkvh

## slt
slt $t2,$t0,$t1

## Check while condition
beqz $t2,WhileEnd_xakg

## Load variables
lw $t0,var_c
lw $t1,var_xauj

## add
add $t2,$t0,$t1
sw $t2,var_c

## print integer
li $v0,1
lw $a0,var_a
syscall

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

## print string
li $v0,4
la $a0,var_xaptb
syscall
j WhileStart_xakg

## End of while
WhileEnd_xakg:

## tell system to terminate application
exit:
li $v0,10
syscall

## end of application main entry point

## Allocate and label memory for variables
.data
var_xkld:	.word 0
var_xaue:	.word 1
var_xpef:	.word 1
var_xkvh:	.word 20
var_xauj:	.word 1
var_xaptb:	.asciiz ","
var_a:	.word 0
var_b:	.word 0
var_c:	.word 0
var_t:	.word 0
