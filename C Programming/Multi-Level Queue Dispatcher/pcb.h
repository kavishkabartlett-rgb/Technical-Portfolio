#ifndef FCFS_PCB // Header guard
#define FCFS_PCB // Acts as a flag to ensure header file is only used once during compilation

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

/*Checks to make sure FALSE and TRUE area defined*/
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

/*Process Management Status*/
#define PCB_UNINITIALIZED 0
#define PCB_INITIALIZED 1
#define PCB_READY 2
#define PCB_RUNNING 3
#define PCB_SUSPENDED 4
#define PCB_TERMINATED 5

struct pcb
{
    pid_t pid;
    char *args[3];
    int arrival_time;
    int service_time;
    int remaining_cpu_time;
    int time_spent_in_a_queue;
    int starvation_check;
    int start_time;
    int status;
    int priority;
    struct pcb *next;
};
typedef struct pcb Pcb;
typedef Pcb *PcbPtr;

/* Function Prototypes */
PcbPtr startPcb(PcbPtr);
PcbPtr terminatePcb(PcbPtr);
PcbPtr printPcb(PcbPtr);
void printPcbHdr(void);
PcbPtr createnullPcb();
PcbPtr enqPcb(PcbPtr, PcbPtr);
PcbPtr deqPcb(PcbPtr *);
PcbPtr suspendPcb(PcbPtr);
PcbPtr resumePcb(PcbPtr);
PcbPtr insertatfrontPcb(PcbPtr*, PcbPtr);
PcbPtr incrementqueuewaittimePcb(PcbPtr*, PcbPtr, int);
#endif
