# Multi-Level Queue Dispatcher Program

##                                   Overview
This project implements a multi-level queue dispatcher using job scheduling, priorty-based queues, and dynamic process management. The dispatcher uses three priority queues, with each utilizing different scheduling policies, to prevent starvation while ensuring an efficient process execution process. Level-0 Queue uses First-Come-First-Served (FCFS), Level-1 Queue uses FCFS, Level-2 uses Round Robin. Each queue has a specific time limit, and processes are downgraded to a lower queue (ex 0 to 1) if they do not finish within the allocated time frame. To prevent starvation, jobs that have waited a certain time W are promoted. 

## Detailed Process Overview
1) Jobs are loaded from an input file (jobs.txt) and stored in queue 
2) Jobs enter priority queue based on arrival and initial priority
3) Level-0 Jobs: Execute first using FCFS, given time quantum of t0. If unfinished, process downgraded to Level-1 at the end of the queue
4) Level-1 Jobs: Executed only if Level-0 empty, given time quantum of t1, and will stop current running Level-1 job instantly if new Level-0 job arrives since jobs can arrive randomly and Level-0 always has highest priority. If process unfinished, downgraded to Level-2
5) Level-2: Executed only if Level-0 and Level-1 empty, given time quantim t2, uses Round Robin (each process given fixed time quantum and then moved to back of queue if unfinished)
6) If Level-1 job has waited W units, all Level-1 moved to Level-0
7) If Level-2 job has waited W units, all Level-2 jobs moved to Level-0
8) All jobs maintain original order when promoted
9) All performance metrics are computed - average tunaround time, average waiting time, and average response time

##                           Skills Used/Important Features
- <big>**Multi-Level Queue System**</big> implements 3 distinct priority queues 
- <big>**Preemptive Scheduling**</big> Higher-priority jobs always execute first even if process in a lower level has not completed the time quantum 
- <big>**FCFS and Round Robin Execution**</big> Different executions used based on the level queue 
- <big>**Starvation Prevention**</big> Jobs that have waited W seconds are promoted
- <big>**Queue-Based Execution**</big> Queues ensure FIFO structure and efficiency
- <big>**Performance Tracking**</big> Key efficiency metrics are calculated and presented as an output, allowing for users to fine tune the time quantums for each level to achieve different results based on which metric is the priority

## File Organization
- **main.c:** Main program logic for the dispatcher. Assigns jobs to appropriate level, implements time quantum rules, manages job execution/suspension/promotion/demotion, and computes performance metrics. Asks for t0, t1, t2, and W inputs  
- **main.h:** Header for main    
- **pcb.c:** - Process Control Block Management. Defines functions to create, manage, and manipulate processes in the queue  
- **pcb.h:** - Header for pcb  
- **random.c:** - Randomly generates jobs. Asks user for number of jobs to generate, poisson distribution mean, and exponential distribution mean as the inputs, and uses these values to generate random arrival and execution times, as well as random priorities. Outputs them in txt file in [arrival time] [execution time] [priority]  

## Inputs (enter ints one after the other with space in between)
- Time for Level-0 queue (lower number ex: 6) 
- Time for Level-1 queue (middle number ex: 8)
- Time for Level-2 queue (longest number ex: 15)
- Maximize waiting time (ex 20)
- Ex: "6 8 15 20" sets t0=6, t1=8, t2=15, and maximum waiting time=20

## Outputs 
- Average Turnaround Time
- Average Waiting Time
- Average Response Time  
*Note: Adjust t0, t1, t2, and W to achieve different output results. If minimzing waiting time is priority, then users could decrease t2, increase t0 and t1, and decrease W*

##                                   Next Steps
- Instead of moving all jobs in a queue level when a single job has waited W time, only move jobs that had actually waited W time
- Increase the number of queues
- Use adaptive time quantum numbers based on the type of job being executed instead of fixed ones 
- Train model to predict how long specific jobs within a certain time will take to optimize scheduling 
- Use weighted scheduling based on total wait time in the Round Robin queue so that long processess do not have exceptionally long wait times 