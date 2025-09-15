# Kavishka Bartlett Technical Portfolio

## Overview 

My name is Kavishka Bartlett, and I am an undergraduate student at Duke University pursuing a double major in Electrical/Computer Engineering (ECE) and Computer Science (CS) with a minor in Mathmatics. I am also conducting research with Dr. Dowell and Dr. Thomas as part of the Duke Aeroelasticity group. 

This repository showcases advanced projects that demonstrate my proficiency in C, MIPS Assembly, Java, and Logisim, focusing on low-level programming, hardware design, operating system logic, and multi-threaded parallel synchronization among others.* Additionally, it highlights my current research project, where I
am using parallelism, physic-informed neural networks, and deep learning surrogate models to dramatically accelerate fluid flow modeling. As my research is still ongoing, files found in the research directory may be unpolished and subject to change. All other directories, however, contain fully completed projects as well as detailed README files following this universal structure: 

- **Overview:** Brief project description 
- **Detailed Overview/architecture:** System design, implementation details, and structure
- **Skills Used and Key Features:** Important technical concepts, methodologies, and tools used in the project
- **File Purposes:** Briefly explains the purpose of all files in file dense projects. Algorithmic files are the most important files, and ordered by level of contribution, complexity and importance**
- **How to Run the Program:** Setup instructions
- **Input and Output data:** Required data from the user and the expected results
- **Next Steps:** Features to add that increase performance, runtime, and program ability

All code is extensively commented to ensure readability and optimal clarity. All Logisim projects contain an output folder containing pictures of the circuits found in the .circ files

## Organization
Each folder contains the following projects: 

- <big>**CFD Research: Deep Learning and Parallelism in Computational Fluid Dynamics**</big> CFD Solver Python, CFD Solver Fortran, CUDA Programming, Diffusion Modules, Gradient Descent Optimization,
    Kokkos Programming, Open MPI Programming, Operator Networks, Physics-Informed Neural Networks, Unstructured Grid Generation  
- <big>**C Programming**</big> Dynamic Data Handling and Recursive Computations, Multi-Level Queue Dispatcher, Parallel Synchonization in Multi-threaded Lab Simulation  
- <big>**Java Programming**</big> DNA Manipulation Simulator, Huffman Coding Compression and Decompression, Markov Model, US Map Route Planner, Word Completion Predictor  
- <big>**Logisim Circuit Design**</big> 16-Bit CPU Circuit, Digital Logic Circuits: Finite State Machines, Adders, and Boolean Algebra  
- <big>**MIPS Assembly Language Programming**</big> Recursion, Computations, and Dynamic Structures  
- <big>**Papers**</big> Philosophy, Quantum Mechanics, Politics    
## Research Summary

- <big>**Parallelization - Kokkos, CUDA, Open MPI**</big> Worked with Kokkos, Open MPI, and CUDA programming to parallelize expensive computations across CPU threads and cores, GPU threads and cores, and multiple CPU/GPU hybrid nodes in HPC clusters. 
- <big>**Physics-Informed Neural Networks**</big> Used activation functions, loss functions, overfitting and regularization deep learning techniques to train, validate, and test the expressivity of self-created physics-informed neural networks, replacing costly CFD simulations with pattern prediction
- <big>**Optimization**</big> Used Gradient-Descent and stochastic optimization, as well as Lagrange Multiplers and Quasi-Newton Methods to enhance my diffusion and surrogate neural networks  
- <big>**Grid Generation**</big> Used Delaunay Grid Generation to create unstructured grids suitable for a wide variety of object geometries 

## Project Summary
*Projects are listed by decreasing order of complexity*

- <big>**Logisim Engineering: 16-Bit CPU Circuit**</big> Built a fully functional 16-bit CPU with only basic logic structures (logic gates, mux, splitters) capable of executing MIPS Assembly programs
- <big>**C Programming: Parallel Synchronization in Multi-threaded Lab Simulation**</big> Worked with operating system logic using the pthread.h library in C. Used multiple mutexes and conditional variables to allow multiple threads to run at the exact same time while preventing data corruption
- <big>**MIPS Assembly Language Programming: Recursion, Computations, and Dynamic Structures**</big> Wrote 3 programs in MIPS Assembly implementing recursion, iterative, and memory management problem-solving techniques. Each program can be run on the 16-bit CPU I built
- <big>**C Programming: Multi-Level Queue Dispatcher**</big> Implemented the logic of a multi-level queue dispatcher using C programming. Calculates performance metrics, directly prevents starvation, and uses both first come first served and round robin algorithms
- <big>**Logisim Engineering: Digital Logic Circuits: Finite State Machines, Adders, and Boolean Algebra**</big> Built Logisim circuits implementing boolean algebra, ripple carry adder/subtractor logic, and finite state moore machine. Used output equations, state transition tables, and D Flip-Flop registers to build the circuits
- <big>**Java Programming: Word Completion Predictor**</big> Designed an autocomplete system to predict the word a user is attempting to type using binary search and HashMap algorithms. Wrote benchmark tests to compare efficiency
- <big>**C Programming: Dynamic Data Handling and Recursive Computation**</big> Coded recursion, iterative and memory mangement methods in C. Worked with file parsing and direct data allocation
- <big>**Java Programming: Markov Text Generator**</big> Developed and trained predictive text generator model using n-gram Markov chains. Implemented both a brute force and a more efficient HashMap algorithm. Used interfaces, class hierarchy, arraylists, file I/O, and Big O runtime calculations
- <big>**Java Programming: US Map Route Planning Algorithm**</big> Simulate US road infrastructure with a graph structre (vertices, edges, edge weights), calculates the shortest route between two user-provided cities using Djikstra's algorithm, draws the route over US map, and outputs execution time and total route distance
- <big>**Java Programming: Huffman Compression and Decompression**</big> Created a Huffman tree to encode and decode raw images and text. Used priority queues to ensure the highest occuring characters are represented with the shortest binary sequences 
- <big>**Java Programming: DNA Manipulation Simulator**</big> Simulated DNA manipulation with separate algorithms using Linked Lists, Strings, and StringBuilders. Had append, cut and slice, reverse, and nucleotideAt methods, as well as performance benchmarks to determine relationship between runtime efficiency and memory usage 

*Some of the projects were assigned by Duke University or the University of Sydney. Whenever the instructors contributed framework, coding segments, or testing files to my projects, they were tagged as an author at the top of their respective files.  
**All files were included in some projects - the README files in those projects distinguish between algorithmic files (ordered by importance) and other files (used for testing, GUI, etc.). If data files were used to test my programs, I included them in a separate Data folder 

## Papers

This portfolio also contains a selected collection of written work I created over the past few years. Some came from academic assignments, others from my personal fascination with Quantum Mechanics, politics, theoretical astrophysics, and philosophy. Included in the folder is a transcript of an AI speech I performed, a Supreme Court Op-Ed, a philosophical exhibition, Aerospace Financial/Management case study, and several theoretical physic research papers.       
