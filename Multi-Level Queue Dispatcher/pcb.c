#include <stdio.h> 
#include <stdlib.h> 
#include "pcb.h"

/**************************************************************
Creates and initlizes null PCB block
returns pointer to new block
***************************************************************/

PcbPtr createnullPcb(void)
{

    PcbPtr new_process_Ptr = (PcbPtr)malloc(sizeof(Pcb));
    if (new_process_Ptr == NULL)
    {
        fprintf(stderr, "ERROR: Could not create new process control block\n");
        return NULL;
    }
    new_process_Ptr->pid = 0;
    new_process_Ptr->args[0] = "./process";
    new_process_Ptr->args[1] = NULL;
    new_process_Ptr->arrival_time = 0;
    new_process_Ptr->service_time = 0;
    new_process_Ptr->start_time = 0;
    new_process_Ptr->remaining_cpu_time = 0;
    new_process_Ptr->time_spent_in_a_queue = 0;
    new_process_Ptr->starvation_check = 0; 
    new_process_Ptr->status = PCB_UNINITIALIZED;
    new_process_Ptr->next = NULL;
    new_process_Ptr->priority = -1;
    return new_process_Ptr;
}

/**************************************************************
Add node to end of linked list
returns pointer to old PCB
***************************************************************/

PcbPtr enqPcb(PcbPtr q, PcbPtr p)
{
    PcbPtr h = q; // q is the original head of the linked list
    p->next = NULL;
    if (q)
    {
        while (q->next) // Loops until q->next == NULL (end of the list)
            q = q->next;
        q->next = p; // Put p as the last node
        return h;
    }
    return p;
}

/**************************************************************
Removes the current head
Returns pointer to the old head
***************************************************************/

PcbPtr deqPcb(PcbPtr *hPtr)
{
    PcbPtr p;
    if (hPtr && (p = *hPtr))
    { // Ensures queue exists and the queue is not empty (*hPtr not null)
        *hPtr = p->next;            //This ensures the original pointer is pointing to the next node 
        return p;
    }
    return NULL;
}

/**************************************************************
Forks the parent process into parent and child process, the child process is then assigned the program specified in p and begins running
If process already exists, then the child is not created and the processes is just resumed
returns pointer to the PCB passed in
***************************************************************/

PcbPtr startPcb(PcbPtr p)
{
    if (p->pid == 0)
    { // not yet started
        switch (p->pid = fork())
        { // putting = in the condition check just saves the step of assigning p->pid to fork(). Condition is still checking fork() return value
        case -1:
            fprintf(stderr, "FATAL: Could not fork process!\n");
            exit(EXIT_FAILURE);
        case 0:                         // if the return value is 0 (returned by fork), that means this process is the child process
            p->pid = getpid();          // getpid() return the pid of the current running process
            p->status = PCB_RUNNING;
            printPcbHdr();              // prints header for formatting
            printPcb(p);
            fflush(stdout);             // ensures all printed statements are finished
            execv(p->args[0], p->args); // replaced current process with another process - does not create new process just replaces
            fprintf(stderr, "ALERT: You should never see me!\n");
            exit(EXIT_FAILURE);
        }
    }
    else // already started, so continue
        kill(p->pid, SIGCONT);
    p->status = PCB_RUNNING; // either way the program is now running
    return p;
}

/**************************************************************
Terminates the process passed in
Returns pointer to the terminated process
***************************************************************/

PcbPtr terminatePcb(PcbPtr p)
{
    int status;
    if (!p)
    { // Check if process exists
        fprintf(stderr, "ERROR: Can not terminate a NULL process\n");
        return NULL;
    }
    else
    {
        kill(p->pid, SIGINT);
        waitpid(p->pid, &status, WUNTRACED);
        p->status = PCB_TERMINATED;
        return p;
    }
}

/**************************************************************
Suspends the current PCB and returns pointer to the suspended block
***************************************************************/
PcbPtr suspendPcb(PcbPtr p) {               //Check if process exists and is running
    if (!p || p->status != PCB_RUNNING) {
        fprintf(stderr, "ERROR: Can not suspend a process that is not running or is NULL\n");
        return NULL;
    }

    kill(p->pid, SIGTSTP);                  //Signal to stop process
    waitpid(p->pid, NULL, WUNTRACED);       //Wait until the process has stopped, use NULL since we dont need exit status information
    p->status = PCB_SUSPENDED;              
    return p;
}

/**************************************************************
Resumes the PCB and returns pointer to the resumed PCB
***************************************************************/
PcbPtr resumePcb(PcbPtr p) {
    if (!p || p->status != PCB_SUSPENDED) {
        fprintf(stderr, "ERROR: Can not resume a process that is NULL or not currently suspended\n");
        return NULL;
    }

    kill(p->pid, SIGCONT);
    p->status = PCB_RUNNING;
    return p;
}

PcbPtr insertatfrontPcb(PcbPtr *q, PcbPtr p) {
    if (!p) {
        fprintf(stderr, "ERROR: Can not insert NULL node in the front");
        return NULL;
    }

    p->next = *q;
    *q = p;
    return p; 
}

/**************************************************************
Takes original queue as 1st parameter, promotion queue as second 
Increments counter whenever the while loop in main runs
Promotes the queue if the wait time is long enough. Returns target array head
***************************************************************/

PcbPtr incrementqueuewaittimePcb(PcbPtr *q, PcbPtr p, int W) {            //Want to check the first element of the queue, so pass in head
    PcbPtr current = *q; 

    while (current) {                           //Use a while loop to increment the counter for every value in the queue
        current->starvation_check++;
        if (current->starvation_check >= W)  { 
            current->priority--;
            current->starvation_check = 0;                   
            if (!p) 
                p = enqPcb(p,current);              //if p is null, this will set it to be the new element just added
            else 
                enqPcb(p, current);                 //Adds current to the end of queue p. Dont change p,however, because p needs to be the head
            deqPcb(q); //Removes the head from queue q and sets the new head to the original pointer. Can remove head because queue is FCFS, so head will always have been there the longest
            current = *q; 
        }
        else 
            current = current->next;            //If nothing is added, keep going through the queue to increment the counter
    }
    return p;
}

/**************************************************************
Prints information about PCB block
***************************************************************/

PcbPtr printPcb(PcbPtr p)
{
    printf("%7d%7d%9d%7d ", (int)p->pid, p->arrival_time, p->service_time, p->remaining_cpu_time);
    switch (p->status)
    {
    case PCB_UNINITIALIZED:
        printf("UNINITIALIZED");
        break;
    case PCB_INITIALIZED:
        printf("INITIALIZED");
        break;
    case PCB_READY:
        printf("READY");
        break;
    case PCB_RUNNING:
        printf("RUNNING");
        break;
    case PCB_SUSPENDED:
        printf("SUSPENDED");
        break;
    case PCB_TERMINATED:
        printf("PCB_TERMINATED");
        break;
    default:
        printf("UNKNOWN");
    }
    printf("\n");
    return p;
}

void printPcbHdr()
{
    printf(" pid arrive service cpu status\n");
}