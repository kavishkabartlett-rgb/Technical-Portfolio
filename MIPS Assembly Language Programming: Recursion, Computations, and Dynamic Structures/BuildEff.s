.data

#buildingStructure: .word 17             #64 bytes for name plus 4 bytes for efficiency - 68 
buildingName: .asciiz "Building Name: "
squareFootage: .asciiz "Square Footage: "
KwH: .asciiz "KwH: "
baseCase: .asciiz "DONE"
str_space: .asciiz " "
str_newline: .asciiz "\n"

.text

main: 

    addi	$sp, $sp, -24			#Create space in the stack for the register and $s0 used in program
    sw		$ra, 0($sp)
    sw      $s0, 4($sp)
    sw      $s1, 8($sp)
    sw      $s2, 12($sp)
    sw      $s3, 16($sp)
    sw      $s4, 20($sp) 

    #Create Head and store address in $s0
    li      $v0, 9                  #Syscall code for sbrk (memory allocation)
    li      $a0, 72                 #Size of the struct - 64 bytes for string (null) plus 4 bytes for efficiency plus 4 bytes for next
    syscall                         #Allocate memory 
    move    $s0, $v0                #$s0 = making $s0 the head of the structure
    sw		$zero, 0($s0)		    #Load initial to be null so I can recognize when I am at end of list

    j _loop                        #going to create a loop to read and store inputs

_loop:  

    li $v0, 9                       #Syscall code for sbrk (memory allocation)
    li $a0, 72                      #Size of the struct - 64 bytes for string (null) plus 4 bytes for efficiency plus 4 bytes for next
    syscall                         #Allocate memory
    move $t0, $v0                   #$t0 = address of new structure

    #Prompt BuildingName: 
    li		$v0, 4		            #Syscall to print a string
    la		$a0, buildingName		#Address of what we want to print - "BuildingName: "
    syscall
    # Allocate memory for building name
    li      $v0, 9                   # Syscall code for sbrk (memory allocation)
    li      $a0, 64                  # Size of the building name
    syscall
    move    $a2, $v0                 # $a2 = address of allocated memory
    #Read BuildingName
    li		$v0, 8		            #Syscall for reading a string
    move	$a0, $a2		        #The read string will be stored at $a0, and 0($t0)
    li		$a1, 64		            #Maximum size of string to be read - 64
    syscall

    li      $t1, 0                  # $t1 is a counter
    move    $t2, $a0                # $t2 points to the beginning of the string

remove_newline:
    lb      $t3, 0($t2)             #Load byte from the string
    beqz    $t3, input_loop_end     #Checking for end of string
    beq     $t3, 10, found_newline  #Waiting until it finds newLine character
    addi    $t2, $t2, 1             #Keep moving
    addi    $t1, $t1, 1             
    j       remove_newline
found_newline:
    sb $zero, 0($t2)                #Get rid of newline by storing null character
input_loop_end:

    #Check to see if it is DONE
    move	$a1, $a0		        #$a1 will also have input address - act as counter going through input
    la		$a2, baseCase		    #$a2 will act as counter going through DONE
    li		$t3, 0		            #Going to count the number of times the _compareLoop runs
    li      $t4, 4                  #4 characters in DONE - loop should 4 times

_compareLoop:

    lb		$t1, ($a1)		        #Store what byte $a1 is pointing to in $t1 - using this to compare each character
    lb		$t2, ($a2)		        # 

    beq		$t3, $t4, _doneSorting	    #If this is equal to 4, then program has reached DONE and needs to be sorted
    bne		$t1, $t2, _storeInfo	#If $t1 != $t2 then the program will keep updating stack in _storeInfo section



    addi	$a1, $a1, 1			    #Go to the next character if they are equal
    addi	$a2, $a2, 1
    addi	$t3, $t3, 1			    

    j _compareLoop
    
_storeInfo:
    #Store BuildingName
    sw		$a0, 0($t0)		        #Store the buildingName - Need 64 bytes of space

    #Prompt and reading squareFootage: 
    li		$v0, 4		            #Syscall to print a string
    la		$a0, squareFootage		#Address of what we want to print - "BuildingName: "
    syscall
    li		$v0, 6		            #Syscall for reading a float
    syscall
    addi	$sp, $sp, -4			#Add space for float
    swc1    $f0, 0($sp)

    #Prompt and reading KwH
    li		$v0, 4		            #Syscall to print a string
    la		$a0, KwH		        #Address of what we want to print - "KwH: "
    syscall
    li		$v0, 6		            #Syscall for reading a float
    syscall                         #KwH = $f0


    #Calculate and store effciency
    lwc1    $f4, 0($sp)
    addi	$sp, $sp, 4			    #Remove the space used for float
    div.s   $f0, $f0, $f4           #calculate the KwH and add it into $f0
    swc1	$f0, 64($t0)		    #Store the KwH in current structure     #96-100 works - tested and it prints correct value for 64($t0)

    sw      $s0, 68($t0)            #Store "head" as next - in first case, next will be null
    la		$t1, 0($t0)		        #Stores correct word into $t1
    lwc1	$f1, 64($t0)		    #Stores correct float into $f1
    lw		$t3, 68($t0)		    #Succesfully has the address of $s0
    la		$s0, 0($t1)		        #Succesfully stores the building name
    swc1	$f1, 64($s0)		    #Stores float into $s0
    sw		$t3, 68($s0)		    #Stores next address (this current one just created) into the next section (68 offset)
    ###Tested exntensively - tried print each item from ($t0) and corresponding ($s0) - they are identical

    j _loop                         #jump back up to loop

_outerLoopSort:
    li    $t7, 0           # tracks whether or not a swap has occured to know when to end outer loop
    li    $s4, 0           # sets previous pointer to null - need to keep track of previous to swap
    move  $s1, $s0         # need head to transverse the list

_innerLoopSort:
    lw    $s2, 68($s1)     # change pointer to next node
    beq   $s2, $zero, _afterInnerLoop  # if there is not a next node (iteration is done)
    lwc1  $f0, 64($s1)     
    lwc1  $f1, 64($s2)     
    c.lt.s $f0, $f1       
    bc1t _doSwap          # if current node efficiency < next node efficiency, swap nodes
    
    # if equal, check the names
    c.eq.s $f0, $f1       
    bc1f _nextNode        # if efficiencies are not equal and current node is not less, skip swap

    #Compare the names to determine if swap is necessary
    lw    $a0, 0($s1)      # pointer to first node
    lw    $a1, 0($s2)      # pointer to next node
    jal   compareString    # a custom written method to compare the strings
    bgt   $v0, $zero, _doSwap  # if current name > next name, dont move to next node
    j     _nextNode        # Only executed if strings are equal or current name < next name

_doSwap:
    lw    $s3, 68($s2)     # temp node
    sw    $s3, 68($s1)     # sets next to temp
    sw    $s1, 68($s2)     # next node points to current
    beq   $s4, $zero, _updateHead   #if the swap is at the head, update the head pointer
    sw    $s2, 68($s4)     # update previous node pointer
    j     _swapFinished         

_updateHead:
    move  $s0, $s2        

_swapFinished:
    li    $t7, 1          # note swap occured
    move  $s4, $s2        # update previous pointer to the new first node
    move  $s1, $s2        # reset current pointer to swapped node

_nextNode:
    move  $s4, $s1
    lw    $s1, 68($s1)     # move current to next node
    bnez  $s1, _innerLoopSort #if current isnt NULL, branch back 

_afterInnerLoop:
    bnez  $t7,  _outerLoopSort  # if a swap occurred, start another outer loop
    j     _doneSorting

# Compare Strings
compareString:
compareString_loop:
    lb    $t0, 0($a0)
    lb    $t1, 0($a1)
    beq   $t0, $t1, _inc_chars
    sub   $v0, $t0, $t1
    jr    $ra
_inc_chars:
    beqz  $t0, _compareString_done
    addi  $a0, $a0, 1
    addi  $a1, $a1, 1
    j     compareString_loop
_compareString_done:
    li    $v0, 0
    jr    $ra

#_sorting:
    #$s0 = head
    #$s1 = current
    #$s2 = temp (current.next)
    #$s3 = temp.next
    #$s4 = previous
    #64($s0) = float for efficiency
    #68($s0) = pointer to the next element in linked list
 
#    lw      $s1, 0($s0)             #Stores the head address into current $s1
#    lw      $s2, 68($s0)            #Stores the head.next address into temp
#    li      $s4, 0                  #set previous to be null at first   

#_sortingLoop:                       #Assuming $s1 is the temporary head of the linked list, and the first byte of the last elment is 0. 

#    lw      $a0, 0($s2)             #Stores the first byte of temp - if this is zero then we have reached end of the list
#    beqz    $a0, _doneSorting       

    
#    lwc1      $f0, 64($s1)            #$f0 = efficiecny of first element
#    lwc1      $f1, 64($s2)            #$f1 = efficiency of second element

    ###### IDEA - DETERMINE IF THEY NEED TO BE SWAPPED ALPHABETICALLY OR NOT HERE, IF THEY DO, THEY RUN _needToBeSwapped
#    c.lt.s  $f0, $f1                  #Checks to see if they need to be swapped
#    bc1f    _needToBeSwapped          #If second element is bigger, than need to be swapped, hence branch
#    move    $a0, $s1                  #address we can manipulate in sorting alphabetically
#    move    $a1, $s2
#    c.eq.s  $f0, $f1                  #Checks to see if they are equal 
#    bc1t    _sortAlpabetically        #If both efficiences are equal, check to see if they need to be sorted alphabetically

 #   j       _goToNextElementToBeSorted 

#_needToBeSwapped: 

    #current.next = temp.next
 #   lw      $s3, 68($s2)            # 68($s2) is temp.next
   # sw      $s3, 68($s1)            # 68($s1) is current.next - store temp.next ($s3) in 68($s1) 
    #temp.next = current
   # lw      $s3, 0($s1)             # Store current 0($sp1) in $s3
  #  sw		$s3, 68($s2)		    # Set temp.next 68($s2) equal to current

  #  beqz    $s4, _firstTime         #If previous ($s4) equals null, then it is first time and you need to set previous.next equal to temp
    #previous.next = temp
  #  lw      $s3, 0($s2)             # Store temp 0($s2) in $s3
  #  sw      $s3, 68($s4)            # Set previous.next 68($s4) equal to temp
  #  j		_madePrevious		    # Don't want to run the notFirstTime code, so it jumps

#_firstTime:

    #head = temp - check this one
 #   lw      $s3, 0($s2)             # Store temp 0($sp2) in $s3
  #  sw		$s3, 0($s0)             # Set head 0($s1) equal to temp

#_madePrevious:
    #current = temp
 #   lw      $s3, 0($s2)             # Store temp 0($s2) in $s3
  #  sw		$s3, 0($s1)		        # Set current 0($s1) equal to temp

    #temp = current.next
   # lw		$s3, 68($s1)		    # Store current.next 68($s1) in $s3
    #sw      $s3, 0($s2)             # Set temp 0($s2) equal to current.next
    
#_goToNextElementToBeSorted:    
    #Once finished swapping alphabet and number
    #previous = current
   # lw      $s3, 0($s1)             # Store current 0($s1) in $s3
    #sw      $s3, 0($s4)             # Set 0($s4) equal to current
    
    #current = temp
    #lw      $s3, 0($s2)             #Store temp in $s3
    #sw      $s3, 0($s1)             # Set current equal to temp

#j _sortingLoop                      #Finished swapping, so return back to sorting function


#_sortAlpabetically:

 #   lb      $t0, ($a0)              #Load a character from the first string
  #  lb      $t1, ($a1)              #Load a character from the second string
   # beqz    $t0, _sortingLoop       #If first string is shorter, then it is already sorted - go back up
 #   beqz    $t1, _needToBeSwapped  #If we reached the end of the second string, that means second string comes first
 #   bgt     $t0, $t1, _needToBeSwapped  #If $t1 is greater than $t0, ex, if $t1 comes earlier in the alphabet, then need to be swapped

  #  addi    $a0, $a0, 1
  #  addi    $a1, $a1, 1 
  #  j       _sortAlpabetically

_doneSorting:                        #Code for when the program has reached DONE

    lb      $a0, 0($s0)
    beqz    $a0, _doneWithPrinting

    lw		$a0, 0($s0)		        #Load the BuildingName string in $a0 to be printed
    lwc1	$f12, 64($s0)		    #Load the efficiency value into $f12 to be printed 

    #Print everything
    li		$v0, 4		            #Print BuildingName
    syscall

    li		$v0, 4		            #Print a space
    la		$a0, str_space		    #
    syscall

    li $v0, 2                       #Print the float (effciency)    
    syscall

    li		$v0, 4		            #Print a new line
    la		$a0, str_newline
    syscall

    lw      $s0, 68($s0)
    
    j _doneSorting

_doneWithPrinting:

_firstLineDone:

    lw	    $ra, 0($sp)	            #Restore valus that was orginally in $ra    
    lw		$s0, 4($sp)		        #Restore the original $s0 value
    lw		$s1, 8($sp)
    lw		$s2, 12($sp)
    lw		$s3, 16($sp)
    lw      $s4, 20($sp)
    addi	$sp, $sp, 8	            #Empties the stack
    jr      $ra                     #jump back to whatever function called main
