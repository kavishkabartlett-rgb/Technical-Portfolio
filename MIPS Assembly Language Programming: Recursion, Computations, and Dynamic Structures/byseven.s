.data 
str_newline: .asciiz "\n"
str_space: .asciiz " "

.text                           # Code segment
.align 2                        # ensure data segment items align to 2^2 = 4 bytes

.globl main                     # indicate that 'main' label is a function and not just an private inner label
main: 
    addi $sp, $sp, -4
    sw	 $ra, 0($sp)		#Saves whatever value is in ra before this function is called so that the value will be preserved

    li $v0, 5
    syscall
    move 	$t1, $v0		#$t1 = integer N

    li $t0, 1               #$t0 is the counter for the loop (i)
_loop:
    bgt		$t0, $t1, _endloop	# if $t0 > $t1 then goto target
    mul     $t2, $t0, 7         # multiply the values in t0 by 7 and put it into t2
            
    li      $v0, 1              #put 1 (code for int) into v0
    move    $a0, $t2            # load value t2 into a0
    syscall

    li      $v0, 4              #syscall to print a string
    la      $a0, str_newline
    syscall

    addi	$t0, $t0, 1			# Increment $t0 by 1 every single time this code runs - counter
    b		_loop			    # branch to _loop
    
_endloop:
    
    lw		$ra, 0($sp)		#Restores value that was orginally in ra
    addi	$sp, $sp, 4	    #Empties the stack
    jr $ra