# Huffman Coding Compression and Decompression

## Overview

This project implements Huffman encoding and decoding to compress and decompress files. The program parses the file to find all possible characters and their frequency within a file, and then places them into a priority queue ordered from least freqeuent to most freqeuent. Using the priority queue, a binary tree is created. Each character is assigned a leaf node and a binary code based on the leaf's location (0 = left, 1 = right). For decompression, HuffMainDecompress starts at the root of the binary tree used in the compression process and reads one the file one bit at a time until a leaf node is reached. Once reached, the algorithm outputs the associated character, returns to the root, and repeats.

## Detailed Overview

*Compression*
1) The program read a user-provided input file and records character frequency 
2) Characters and their associated frequency are placed in a priority queue 
3) The first two characters are popped from the queue and form the bottom of the tree
4) Their frequency values are added together to form a parent node
5) Another element is popped from the queue and added as a leaf next to the parent node
6) The new leaf's frequency is added to the first parent node's frequency and a new parent node with the leaf and the original parent node as its children
6) The process repeats until the queue is empty
7) Starting at the root, a binary code is assigned to each leaf and their associated character - moving left in the tree means 1, moving right means 0 
8) The original file is compressed and the Huffman tree structure included

*Decompression*
1) The program reads the Huffman tree structure and recreates it
2) Starting at the root of the reconstructed binary tree, the program reads the compressed binary data and follows the instructions until it reaches a leaf
3) Once a leaf is reached, the character is printed and the program returns to the tree root to repeat the process
4) The original decompressed file is returned 

## Skills Used/Key Features

- <big>**Tree Data Structure**</big> A full, greedy algorithm binary tree is used to encode and compress the file. Recursive methods are used to reconstruct tree in decompression and to generate the Huffman encodings during compression
- <big>**Direct Bit Manipulation**</big> Uses classes such as BitInputStream and BitOutputStream to read and write bits to external sources 
- <big>**Java I/O and NIO**</big> Uses standard Java input/output API where read and write operations are handled one at a time, as well as NIO API where multiple bytes can be read and written at the same time
- <big>**Custom Exceptions and Error Handling**</big> HuffException extends RuntimException, allowing for more efficient handling of specific Huffman errors and exceptions and ensuring bit-level operations are enforced and executed correctly

## File Purposes 

*Algorithm Files*
- <big>**HuffProcessor.java**</big> Parent file that calls the methods in compress, decompress, BitInputStream, and BitOutputStream and processes all of the information
- <big>**HuffMainCompress.java**</big> Reads input file, compresses the file, and writes the output
- <big>**HuffMainDecompress.java**</big> Reads a compressed file, reconstructs Huffman tree, and restores the original file
- <big>**BitInputStream.java**</big> Reads file bit by bit using Java NIO
- <big>**BitOutputStream.java**</big> Writes file bit by bit using Java NIO

*Other Files*
- <big>**HuffException.java**</big> Custom exception and error handling
- <big>**FileSelector.java**</big> Provides the GUI for file selection

## How to Run Program/Inputs

*Compress* 
- Compile and run HuffMainCompress 
- Select file using the GUI file picker

*Decompress*
- Compile and run HuffMainDecompress
- Select .hf file to decompress 

## Outputs 

*Compress*
- Outputs .hf compressed file, bits read, bits written, and time taken

*Decompress* 
- Outputs original file as a .uhf, bits read, bits written, and time taken 

## Next Steps

- Speed up execution time by using multi-threading techniques taken from my Synchonization project. Could have compression/decompression of different sections of a file running at the same time
- Extend file support (images, audio, etc) 
- Implement different encoding algorithms (RLE, arithmetic coding, Lossy Encoding)