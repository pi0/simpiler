
## application main entry point
.text
main:

## Load variables
lw $t0,var_xptb
lw $t1,var_xuwc

## rem
rem $t2,$t0,$t1
sw $t2,var_a

## print integer
li $v0,1
lw $a0,var_a
syscall

## tell system to terminate application
exit:
li $v0,10
syscall

## end of application main entry point

## Allocate and label memory for variables
.data
var_xptb:	.word 10
var_xuwc:	.word 4
var_a:	.word 0
