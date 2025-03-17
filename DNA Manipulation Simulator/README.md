# DNA Splicing Simulator 

## Overview 

This program simluates an enzyme splicing DNA strands using various data structures. Uses LinkedList in LinkStrand and StringBuilder in IDnaStrand to represent DNA strands. Each implementation can splice, reverse, cut, and append DNA strands efficiently. DNABenchmark calculates important perfomance metrics to determine the tradeoffs between the two approaches: String has faster character access time but slower concatenation, while linked list has slow charater access time but faster cutting, splicing, and appending times. I also implemented a basic String method, however, it has the worst performance and is mainly used as a relative performance benchmark.

## Detailed Overview 

1) The user modifies DNABenchmark to set DNA sequence, enzyme, and splice characteristics
2) The program then loads the DNA sequence from a user provided file, and creates either a LinkedList, StringBuilder, or String based on which method the user chooses
3) The user selects an opeation (cut, splice, reverse, or append)
4) DNA modifications are executed, and performance metrics are calculated
5) Results are printed (metrics + modified DNA)

## Skills Used/Key Features

- <big>**Interfaces**</big> Used inheritance (from IDnaStrand) and thus a common structure shared between all DNA strands to allow for easy performance comparison
- <big>**Linked List**</big> Used a linked list to represent DNA strands and improve appending, splicing, and cutting operations
- <big>**Iterators**</big> Created CharDnaIterator to efficiently iterate character-by-character through DNA strands
- <big>**Performance Benchmarks**</big> Execution speeds and Big O runtime are calculated for each methods

## File Purposes 
*N = length of DNA strand, M = length of DNA segment being inserted, K = number of enzyme to be cut occurences*  
  
- <big>**IDnaStrand.java**</big> The parent class for all DNA strand classes. Defines methods such as initialize, append, cutAndSplice, charAt, etc.  
- <big>**LinkStrand.java**</big> Linked List implementation, has O(1) append, O(K) cut and splice operations, but O(N) charAt
- <big>**StringStrand.java**</big> Basic String implementation, has O(1) charAt, but O(N) append and O(N+M) cut and splice
O(N+M) cut and splice,  
- <big>**StringBuilderStrand.java**</big> StringBuilder implementation, has O(1) amortized append, and O(1) charAt, but O(N+M) cut and splice
- <big>**CharDnaIterator.java**</big> Defines iteration process over DNA strands
- <big>**DNABenchmark.java**</big> Measures efficiency of different DNA strand implementations. Uses System.nanoTime() to determine execution time
- <big>**TestStrand.java**</big> Contains automated tests to determine accuracy of DNA strand operations

## How to Run Program/Inputs

- Uncomment which implementation is being used in DNABenchmark.java 

**Automatic Run**
- User adjusts "private static final String ENZYME" to be whatever string of nucleotides they want. This determines which sequence of nucleotides will be cut/replaced
- Change filename in line 35 to change the original data sequence 
- Change "private static final int DEFAULT_SPLICEE" in DNABenchmark to a number, L, which means the program will take the first nucleotides of the DNA sequence up until L and use them as the replacement sequence for cut and splice
- Program will run spliceeBenchmark and sourceBenchmark() to test performance for different splicee and DNA sequence sizes

**Manual Run**
- To run DNA operations manually, comment out sourceBenchmark and spliceeBenchmark 
- Uncomment the specific sections for append, reverse, and cut and splice to have those operations performed 
- Comment out spliceeBenchmark and sourceBenchmark
- The program will instead run those operations and output the updated DNA strand

## Outputs 

- For default test, it prints DNA storage method, original DNA sequence length, length of replacement (splice) DNA inserted, final length, execution time, and number of times the replacement occured 
- For manual test, the original entire DNA sequence, the operations, and the finalized DNA strand will all be printed 

## Future Improvements 

- Improve the user interface so the user does not have to directly comment/uncomment lines of code
- Add support for multiple enzyme cuts at the same time
- Randomly replace bases with new nucleotides to represent mutations 
- Combine with Markov model to predict which bases could come next in a random DNA strand based on training data 