# Markov Text Generator

## Overview 

This project implements a Markov Model text generator, using both a basic and a more efficient hashmap approach. The algorithm is trained on a user-provided training text and outputs text sequences based on the patterns found in the training text. For example, if the traing text has "The dog sat on the rug. The dog slept on the rug" and the model begins with "dog," then the next model output will have a 50% chance of being either "sat" or "slept." 

## Detailed Overview

1) Reads a given text file and processes it into an array of words
2) Either BaseMarkov or HashMarkov is initilized based on which one the user selected 
3) The model is assigned a Markov order, which determines how many words to look at when predicting next word
4) A random starting sequence is selected

**Base Markov**
5) The models scans the array of words and returns a list of possible words to follow starting sequence
6) The new word replaces the oldest word in the sequence, forming a new WordGram (sequence of words)
7) The process repeats 

**Hashmap Markov** 
5) Create a Hashmap with word sequences of length Markov order as the key and the words that follow the particular phrase as the values 
6) Looks up current sequence in the hashmap and randomly selects one of the words from the associated array
7) The new word replaces the oldest word in the sequence, and the process repeats

**Both**
8) If there is no word that follows a sequence, a random sequence is choosen from the training text
9) The models finish when the output has reached the user defined output length

## Skills Used/Key Features 

- <big>**Dynamic Data Structures**</big> Uses arrays, array lists, and hash map 
- <big>**Object Oriented Programming**</big> Uses encapsulation (protected/private variables), inheritance (BaseMarkov and HashMarkov implement MarkovInterface), and polymorphism 
- <big>**Algorithmic Concepts**</big> Trains an algorithm, evaluates runtimes, selects random words, and provides outputs based on probabilities 
- <big>**Runtime/Performance Metrics**</big> Calculates and prints performance metrics, and calculates Big O runtime ( O(N) for base model lookup, O(1) for hash map lookup)
- <big>**File I/O**</big> Reads and manipulates data directly from user provided files 

## File Purposes

*Algorithm Files*
- <big>**BaseMarkov.java**</big> Implements brute force (O(N) lookup time) Markov method by searching the training text every time a word is to be generated using the getFollows() method. Stores words from training text in an array
- <big>**HashMarkov.java**</big> Uses a HashMap to increase efficiency (O(1) lookup time) by running through the training text once and setting up a HashMap where the keys are the word sequences and the values are the words following that particular sequence
- <big>**MarkovDriver.java**</big> Driver program for both Markov models. Loads and reads training text file, initilizes BaseMarkov or HashMarkov, and prints generated text and calculated performance metrics 
- <big>**MarkovInterface.java**</big> Defines methods every Markov Model must implement and ensures that all Markov Models can be run using the MarkovDriver
- <big>**WordGram.java**</big> Creates a WordGram, or a fixed sequence of words. Used to manage which sequence of words the program is basing the next predicted word on

*Other files*
- <big>**MarkovTest.java**</big> JUnit test suite used to check whether a Markov model is working correctly
- <big>**TextSource.java**</big> Reads an entire file into a single string while removing extra spaces and whitelines

## How to Run Program/Inputs

- Modify MarkovDriver by uncommenting whichever Markov model is to be used
- Change the training file used by changing the file path in line 22 in MarkovDriver
- Change the "private static int MODEL_ORDER" in MarkovDriver to adjust how many words are in a sequence
- Change the "private static int TEXT_SIZE" in MarkovDriver to dictate how many words the output text should have
- Change the "private static long RANDOM_SEED" to be System.currentTimeMillis(); if the user wants unique outputs each time the program is run (it is set to the static number 1234 to help debugging)

## Outputs 

- Generated text based on training data patterns
- Performance Metrics: time take to train and generate text, number of words in training text, and Markov model order used

## Next Steps

- Allow the user to type in inputs instead of directly changing the code
- Use a more efficient method, such as a TreeMap or a suffix tree
- Create a Java GUI to make the program user-friendly 