# Multithreaded Synchronization for Lab Scheduling Project

##                     General Skills Used
Multithreading and Parallel Processing  
Synchronization using Mutexes and Condition Variables  
FIFO-based Scheduling and Resource Allocation  
Low-level C Memory and Data Management  

##                          Overviews
This project implements an algorithm using concurrency and multithreading to simulate managing student lab exercises. There are teacher, student, and tutor/lab room threads, with the teacher placing students into lab groups, assigning them to a tutor/room, dismissing them once they complete their activity, assigning a new group into the room/tutor, and then dismissing the tutor once there are no more students left. There is one tutor thread per lab room, and the tutor thread is responsible for having the students complete the exercise, dismissing them once they finish, and notifying the teacher of when the lab room is avaiable for students. 

## Detailed Process Overview (in Order of Occurence)
1) N students divided in M groups, with each group having a group id 
2) Groups are of size N/M, with the first N%M groups having N/M+1 if N is not divisible by N 
3) Teacher thread randomly assigns students to groups after all students have arrived
4) Teacher then assigns each group to lab rooms 
5) There are K lab rooms with each lab room having a tutor (tutor id = lab room id)
6) Each room can only hold one group at a time 
7) Students enter a lab only when their group id is called by the teacher, which occurs when there is an empty room and a waiting student group
8) Groups are always called in natural order according to their group id (0 first, 1 secon...)-After entire group enters a lab room, tutors starts activity for a time between T/2 - T
9) After activity finishes, tutor dismisses all the students
10) Once all students leave, tutor reports to teacher and gets assigned a new group or dismissed by the teacher if no new groups
11) If more than one empty room, teacher assigns based on first-in-first-out order
13) After all tutors have left and no groups left, teacher leaves
14) After teacher leaves, main method free all data and terminates


##                         Skills/Key Features Used
- <big>**Dynamic Group Assignments**</big> - students randomly assigned into groups  
- <big>**Multiple threads**</big> - 1 main, 1 teacher, N student, and K tutor threads  
- <big>**Parallelism**</big> - student, tutor, and teacher threads can run at the same time. Ex: the teacher assigning a group to a tutor, a tutor running lab activities, another tutor signaling their lab room is empty, and students leaving the class could all be happening at the exact same time   
- <big>**Concurrency**</big> - within threads of the same type - ie. student threads can run at the same time but sometimes take turns accessing shared data   
- <big>**Multiple Mutexes**</big> - used in critical sections to protect shared data and prevent race conditions, and used multiple mutexes to allow true concurrency  
- <big>**Synchronization Mechanisms**</big> - semaphores, condition variables, FIFO Queues, and arrays used to ensure proper order of execution   
- <big>**Methods Used**</big> - used methods in pthread.h library (pthread cond broadcast/signal(), sem init(), pthread create, etc.)  
- <big>**Print statements**</big> - to ensure multi-threading, threads print lines after completing activity       

## Inputs 
***Enter values one after the other with a space in between each***   
Number of students  
Number of groups   
Number of tutors/lab rooms  
Maximum time limit for each group of students to complete activity  

##           Outputs (print statements for all thread actions)

### When the teacher is waiting for all students to arrive:
- “Teacher: I’m waiting for all students to arrive.”
- “Student (id): I have arrived and wait for being assigned to a group."

### When the teacher assigns group ids to students:
- “Teacher: All students have arrived. I start to assign group ids to students.”
- “Teacher: student (id) is in group (id).”
- “Student (id): OK, I’m in group (id) and waiting for my turn to enter a lab room.”

### When assigning lab rooms to groups after students are assigned to groups:
- “Teacher: I’m waiting for lab rooms to become available.”
- “Tutor (id): The lab room (id) is vacated and ready for one group.”
- “Teacher: The lab (id) is now available. Students in group (id) can enter the
room and start your lab exercise.”
- “Student (id) in group (id): My group is called. I will enter the lab room (id)
now.”
- “Tutor (id): All students in group (id) have entered the room (id). You can start
your exercise now.” (after all students in the group have entered the room)
- “Tutor (id): Students in group(id) have completed the lab exercise in (t) units of
time. You may leave this room now.”
- “Student (id) in group (id): Thanks Tutor (id). Bye!”
- “Teacher: There are no students waiting. Tutor (id), you can go home now.”
- “Tutor (id): Thanks Teacher. Bye!”
- “Teacher: All students and tutors are left. I can now go home.”

### When all students have completed lab exercises and tutors have left, the main thread prints the following status message and then terminate:
- “Main thread: All students have completed their lab exercises. This is the end of simulation.”

**IMPORTANT: Messages coming out of order proves parallelism and concurrency**

##                   Next Steps
- Use read/write locks instead of general locks to allow multiple readings of protected data at the same time
- Multiple teacher threads to increase efficiency 
- Instead of creating new threads for every instance, allow for threads to be reused
- Have students leave at their own random time instead of with the group simulates students working at different speeds
- Have K be a maximum value and instead increase and decrease total amount of rooms in circulation based on demand
- Add checkpoints to save system state in case of program fail
