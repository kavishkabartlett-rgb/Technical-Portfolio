#include <stdio.h>
#include <stdlib.h>

#int recurse(int N) {
#    if (N==0)
#        return 2;
#    else return 3*(N-1)+recurse(N-1)+1;
#}
#int main(int argc, char *argv[]) {
#    if (argc != 2) {
#        printf("Only type one input \n");
#        return EXIT_FAILURE;
#    }
#
#    int N;
#    if (sscanf(argv[1], "%d", &N) != 1 || N < 0) {
#        printf("Invalid input. Please enter an integer greater than zero.\n");
#        return EXIT_FAILURE;
#    }

#    int result = recurse(N);
#    printf("%d\n", result);
#    return EXIT_SUCCESS;
#}
.text

main: 

    addi    $sp, $sp, -4    #create space in the stack
    sw      $ra, 0($sp)     #store the r0, s0 so its saved after main

    li      $v0, 5          # syscall for read integer
    syscall     
    move 	$a0, $v0        # move the value N into register since we need v0 to return (for later)

    jal foo                 #Creates $ra so that when foo is done running it will return to this line

    move 	$a0, $v0		#Move the value calculated by recurse ($v0) into $a0
    li		$v0, 1		    #Syscall to print an integer
    syscall
    

    lw	    $ra, 0($sp)	    #Restore valus that was orginally in ra, s0         
    addi	$sp, $sp, 4	    #Empties the stack
    jr      $ra             #jump back to whatever function called main

foo:                        #going to be the function where the actual computation occurs

    addi	$sp, $sp, -8	# create stack for variables 
    sw      $ra, 0($sp)     #Essentially saving the address of jal foo (line 65) so that it will loop, except for first time
                            #where it is saving the address of jal foo in main (line 37)

    bnez    $a0, _endif     #Branch if the value is not equal to zero - checking to make sure its not the base case

    li		$t0, 2		    #This will only run when $a0 = 0 (base case), thus the value should be 2
    move 	$v0, $t0		#Moves the t0 into v0 so we can sum it later
    
    j _return

_endif:

    addi	$a0, $a0, -1		#Turns a into n-1

    mul     $t0, $a0, 3         #Accounts for the 3*(N-1)
    addi	$t0, $t0, 1			#Adds the plus 1
    
    sw		$t0, 4($sp)		    #Stores the value of t0 (the value calculated by one recurse method)
    jal		foo				    #jump to foo and save position to $ra
    lw		$t0, 4($sp)		    #Stores the value 4 past $sp - the most recent $t0 calculted by lines 61-62
    
    add		$v0, $v0, $t0		#Gets the value most recent $t0 and adds it to $v0, which is keeping running sum    

_return:

    lw		$ra, 0($sp)		    #This will load the most recent $ra (address of jal foo in line 65) so the program can jump
    addi	$sp, $sp, 8			#Effectively clears the stack - moves to the next value that was calculated and added to stack
    
    jr		$ra					#Jump to $ra (line 68)