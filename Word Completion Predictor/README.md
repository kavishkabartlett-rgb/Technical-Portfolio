# Word Completion System

## Overview 

This project suggests the next word based on a given prefix, similar to how text prediction works in mobile keyboards. The system is trained with a word dataset, where each word is assigned a weight value representing its frequency of use. When a user types in a certain prefix, the program scans its dataset to find the most likely match. To search, several methods are implemented - BruteAutocomplete scans every word in the dataset (O(N)) to find matches, BinarySearchAutocomplete uses Binary search to find words (O(log N)), and HashMap implements a hashmap with various prefixes as the keys and corresponding words as the values (O(1)). The program uses benchmarking tools to compare different autocomplete algorithms and their various execution times and memory usage.

## Detailed Overview

1) The program intializes key parameters based on whether "final static String Autocompletor_Class_Name" equals binary search, brute force, or hashmap
2) A file selection window is opened, and the user selects a dataset in .txt format
3) A new GUI window is launched with a search bar, where the user enters a prefix and the program continuously suggests matching words based on what the user has entered 
4) The program finds the matching words using either brute force, binary search, or HashMap algorithms 
5) Once the user presses enter or clicks a suggestion, a browser opens a google serach with the selected/entered word
6) If the user runs BenchmarkForAutocomplete, then alexa.txt dataset is loaded, all 3 autcomplete are initialized, execution time and memory usage are measured, and a comparision table is printed 

## Skills Used/Key Features

- <big>**Search Algorithms**</big> One of the methods implements a binary search, where all of the words in a dataset array are sorted alphabetically to allow for O(log N) searching complexity 
- <big>**Search Engine Auto-Suggestions**</big> Autocomplete algorithms similar to ones found in this project are used in search engines, smart text predictions, voice assistants, and more
- <big>**Priority Queues**</big> Implemented a fully functioning priority queue to ensure the lowest weighted word is always at the front and can be easily removed when the queue has reached maximum size
- <big>**HashMaps**</big> Used HashMaps with prefixes as the keys and an array of corresponding words (organized by decreasing occurence) as the values to allow faster runtime but at the cost of greater memory usage 
- <big>**Algorithmic Efficiency**</big> Used benchmark tests and Big O analysis to understand the inverse relationship between execution time and memory usage - faster execution time often resulted in greater memory usage

## File Purposes
*N = Total number of words in dataset, M = Number of words matching a given prefix, K = Number of suggestions the user wants the program to return*

***Algorithm Files***
- <big>**Autocompletor.java**</big> Defines interface for the various autocomplete algorithms, ensures consistent structure
- <big>**BruteAutocomplete.java**</big> Implements brute force method by manually searching through the dataset for every single prefix, O(N log K) runtime, low memory usage
- <big>**BinarySearchAutocomplete.java**</big> Implements binary search algorithm by sorting the dataset and using a binary search to find corresponding words, O(log(N) + Mlog(K)), low memory usage
- <big>**HashListAutocomplete.java**</big> Uses a Hashmap with prefixes as the keys and corresponding words as the values (organized by weight), allows O(1) runtime, high memory usage

*GUI Files*
- <big>**AutocompleteMain.java**</big> Main entry point of the program, allows user to choose to run benchmark tests or the autocomplete GUI interactive window
- <big>**AutocompleteGUI.java**</big> Controls the GUI for displaying text box, suggesting completions, and selecting the correct suggestion 
- <big>**Term.java**</big> Handles file input selection for the GUI 

*Utility/Benchmark Files*
- <big>**BinarySearchLibrary.java**</big> Provides methods for finding the start and end positions of matching prefixes within a sorted list of Term objects, used in binary search
- <big>**Term.java**</big> Represents individual term, each term contains word + weight, defines methods for manipulating order
- <big>**PrefixComparator.java**</big> Comparator used to sort words based on prefix, used for the binary search method 
- <big>**BenchmarkForAutocomplete.java**</big> Compares execution speed and memory usage for each algorithm on different datasets
- <big>**TestBruteAutocomplete.java**</big> JUnit test suite for brute force
- <big>**TestBinarySearchAutocomplete.java**</big> JUnit test suite for binary search 
- <big>**TestHashListAutocomplete.java**</big> JUnit test suite for HashMap autocomplete
- <big>**TestTerm.java**</big> Verifies Term objects are properly compared and sorted 
- <big>**CountedComparator.java**</big> Counts how many compare() are called, used for measuring performance of the different algorithms

## How to Run Program/Inputs

*GUI Autcomplete*
- After compiling and running AutocompleteMain, user selects a dataset in the file selection window that appears
- As the user begins typing a word in the search bar, the top K matching words appear and update dynamically 
- Once the user clicks the correct suggestion or finishes typing the word and presses enter, a Google search opens for the word

*Benchmark*
- Compile and run BenchmarkForAutocomplete with the command 

## Outputs 

*GUI Autocomplete* 
- Suggestions and the eventual Google search 

*Benchmark* 
- Initialization time for each of the 3 algorithms, sample prefixes, N, K, execution time and total memory usage for all 3 algorithms

## Next Steps
- Use a cache to keep track of frequently selected items, and update the weight of words accordingly 
- Combine autocomplete with a Markov model to not only complete word, but suggest how the entire sentence might be written
- Add support for mispellings - if the user types "akl," suggest all as an option 