# Dynamic Data Handling and Recursive Computation in C Programming 

## Overview
This project contains three distinct C programs that each utilize a different fundamental technique to solve coding problems. byseven.c computes the first N positive numbers divisible by 7 through an iterative looping process. recurse.c solve a mathmatical formula using recursion and a base case. BuildEff.c read building data from a file, computes energy efficiency, and sorts the buildings based on efficiency or name (alphabetically) using dynamic memory management techniques. Goal was to learn the differences between a higher level programming language (Java, Python) and a low-level language.  

## Detailed Overview 

1) Initializes values (a swapping tracker, ints to store data values), creates a Building struct with defined values
2) An outer loop starts repeating based on is_sorted, which tracks whether any swaps occured
3) The inner loop compares and swaps adjacent nodes based on whether the current efficiency is less than the next nodes efficiency
4) If same efficiency, the nodes are swapped to establish alphabetical order
5) Prints the buildings and their efficiency 
6) Closes files, frees memory, and terminates

## Skills Used/Key Features

- <big>**Dynamic Memory Allocation**</big> Uses stack management (malloc, free) to directly manage memory
- <big>**Command-Line Argument Implementation**</big> Uses argv[] to handle user inputs and file methods to read text from files
- <big>**Looping Techniques**</big> Implements both recursive and iterative alogrithms
- <big>**Bubble Sort**</big> Implements bubble sort algorithm

## How to Run Program/Inputs

- Use gcc -o byseven byseven.c to compile program 
- Directly enter N when running recurse or byseven (ex: ./recurse 4 if N=4) 
- Run BuildEff by passing a file as an argument (ex: ./BuildEff buildinginfo.txt), where the file has building name on one line, the square footage on the next, and the KwH on the next. File should end with DONE when no more buildings are to be added

## Outputs

- byseven.c, outputs multiples of 7 from 7 to N*7
- recursive.c, outputs answer to f(N) = 3(N-1) + f(N-1) + 1 with base case f(0) = 2
- BuildEff.c, outputs list with building names and their efficiency, ordered by decreasing efficiency or alphabetically if similar efficiencies