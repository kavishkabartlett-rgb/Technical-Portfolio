# MIPS Assembly Language Programming: Recursion, Computations, and Dynamic Structures

## Overview
This project uses MIPS Assembly to tackle 3 unique problem-solving approaches: loop-based iteration (byseven.s), recursive function calls (recurse.s), and dynamic memory allocation (BuildEff.s). Byseven.s prints the first N numbers that are divisible by 7 by storing N in register $t1 and incrementing a counter in register $t0 until $t0 > $t1. In each loop iteration, the calculated value is placed in $t2 and printed using the proper syscall command. In recurse.s, it computes the funtion f(N) = 3(N-1) + f(N-1) + 1 with a base case of f(0) = 2 through instructions such as jal foo, jr $ra, and j _return and by avoiding overwriting data through careful stack implementation. BuildEff.s uses an iteration method to read a list building and their power usage, calculate energy efficiency, store data in linked list, and sorts the list in descending order of efficiency using a bubble sort method.

## Detailed Process Overview (BuildEff)

1) Using $sp, manually creates space in the stack for registers $ra and $s0-$s4.
2) Create logic of a linked list by using a syscall to request  72 bytes of memory for the first node (64 bytes for name, 4 bytes for efficiency, 4 bytes for a pointer)
3) Prompts user for building name, square footage, and energy usage while manually allocating bytes to store the information 
4) Reads input and iterates through the string to detect ASCII and replace it with NULL
5) Goes character by character checking for DONE, if matched, sorting being; if not, input data is stored instead
6) Stores information, calculates efficiency using flatoing-point division, and links a new entry into the linked list by manually moving $s0 68 bytes to a new address
7) Initialize sorting variables, move address to end of linked list, and compare efficiency. If efficiency of node before is less than current, swap. If not, check alphabetical order
8) Sort alphabetically by manually comparing every character, if first name is greater, swap them 
9) Repeat until the entire list is sorted (bubble sort) 
10) Print the sorted list

## Skills Used/Key Features

- <big>**Direct Hard Interaction**</big> I decomposed MIPS instructions into direct stream of 0 and 1s, meaning I could run this entire program on the CPU I built in Logisim and it would work exactly as intended
- <big>**Dynamic Memory Allocation**</big> Used sbrk syscall to customize and directly create/manage memory 
- <big>**Linked List**</big> Manually create a linked list by directly managing memory addresses 
- <big>**User Input Parsing**</big> Processes user inputs and ensures they are valid
- <big>**Bubble Sort**</big> Implemented the logic of a bubble sort 

## How to Run Program/Inputs

- Download MIPS simulator (like QtSpim)
- byseven.s, enter integer N when prompted 
- recurse.s, enter integer N when prompted 
- BuildEff.s, enter building name, square footage, and KwH as separate inputs when prompted. Type DONE when finished with the list

## Outputs

- byseven.s, outputs multiples of 7 from 7 to N*7
- recurse.s, outputs 3(N-1) + f(N-1) + 1 
- BuildEff.s, outputs list of building names and their efficiency, ordered by efficiency or alphabetically if efficiencies equal 

## Next Steps 

- Use Merge Sort to achieve O(N*log(N)) runtime instead of O(N^2)
- Take input from .txt file instead of user generating everything 
- Include method to generate random buildings for a .txt file