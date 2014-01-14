
## application main entry point
.text
main:

## Load variables
lw $t0,var_xfdc
move $t1,$0

## add
add $t2,$t0,$t1
sw $t2,var_c

## Load variables
lw $t0,var_xfid
move $t1,$0

## add
add $t2,$t0,$t1
sw $t2,var_a

## Load variables
lw $t0,var_xfnd
move $t1,$0

## add
add $t2,$t0,$t1
sw $t2,var_b

## While Start
WhileStart_xure:

## Load variables
lw $t0,var_c
lw $t1,var_xaaf

## slt
slt $t2,$t0,$t1

## Check while condition
beqz $t2,WhileEnd_xure

## Load variables
lw $t0,var_c
lw $t1,var_xakg

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
la $a0,var_xuhkb
syscall
j WhileStart_xure

## End of while
WhileEnd_xure:

## tell system to terminate application
exit:
li $v0,10
syscall

## end of application main entry point

## Allocate and label memory for variables
.data
var_xfdc:	.word 0
var_xfid:	.word 1
var_xfnd:	.word 1
var_xaaf:	.word 20
var_xakg:	.word 1
var_xuhkb:	.asciiz ","
var_a:	.word 0
var_b:	.word 0
var_c:	.word 0
var_t:	.word 0
