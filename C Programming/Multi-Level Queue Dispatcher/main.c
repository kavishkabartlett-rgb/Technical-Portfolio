
/*
FCFS Dispatcher
*/

#include "fcfs.h"

int main(int argc, char *argv[])
{
    /*** Main function variable declarations ***/
    FILE *input_list_stream = NULL;
    PcbPtr fcfs_queue = NULL;
    PcbPtr level0_queue = NULL;     //Priority queues
    PcbPtr level1_queue = NULL;
    PcbPtr level2_queue = NULL;
    PcbPtr current_process = NULL;
    PcbPtr process = NULL;
    int timer = 0;
    int total_turnaround_time = 0;
    int total_waiting_time = 0;
    int total_response_time = 0;
    int process_count = 0;
    int t0, t1, t2, W = 0;

    // 1. Populate the FCFS queue
    if (argc <= 0)
    {
        fprintf(stderr, "FATAL: Bad arguments array\n");
        exit(EXIT_FAILURE);
    }
    else if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <TESTFILE>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if (!(input_list_stream = fopen(argv[1], "r")))
    {
        fprintf(stderr, "ERROR: Could not open \"%s\"\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    while (!feof(input_list_stream))
    { // put processes into fcfs_queue
        process = createnullPcb();
        if (fscanf(input_list_stream, "%d, %d, %d", &(process->arrival_time), &(process->service_time), &(process->priority)) != 3)
        {
            free(process);
            continue;
        }
        process->remaining_cpu_time = process->service_time;
        process->status = PCB_INITIALIZED;
        fcfs_queue = enqPcb(fcfs_queue, process);
        process_count++;
    }
    process = createnullPcb();

    //Request t0,t1,t2, W info from user
    printf("Enter values for t0, t1, t2, and W: ");
    while (scanf("%d %d %d %d", &t0, &t1, &t2, &W) != 4)             //if the user does not input the data correctly, the program will keep requesting until they do
        printf("Error reading input. Please re-enter the 4 values and ensure there is only a space in between each number: ");

    // 2. Whenever there is a running process or the queues are not empty:
    while (current_process || fcfs_queue || level0_queue || level1_queue || level2_queue)
    {
        //If the target level is empty, then leveln_queue =... will ensure that the pointers now point to something
        //No need to update the original queue because I'm passing in pointer of pointer - the actual inherent value will be changed and the original pointer will be accurate
        level0_queue = incrementqueuewaittimePcb(&level1_queue, level0_queue, W);      //First parameter is the queue checking for upgrades, second is queue to be promoted to, W is W
        level1_queue = incrementqueuewaittimePcb(&level2_queue, level1_queue, W);      //Increment every single item in the array each time the main loop runs since every element is waiting whenever main runs
                                                                        //No need to do anything for queue0 since it has highest priority

        while (fcfs_queue && fcfs_queue->arrival_time <= timer) {       //if queue has elements and jobs have arrived
            process = deqPcb(&fcfs_queue);                              //Removes top node from the list of jobs and assign it to process

            switch (process->priority) {                                //Checks each priority and assigns to appropriate queue
                case 0: 
                    level0_queue = enqPcb(level0_queue, process);
                    break;
                case 1: 
                    level1_queue = enqPcb(level1_queue, process);
                    break;
                case 2: 
                    level2_queue = enqPcb(level2_queue, process);
                    break;
                default:                                                //if queue number is not 0,1,2 then its invalid 
                    fprintf(stderr, "ERROR: Invalid priority number %d\n", process->priority);
                    free(process);
                    break;       
            }
        }

        // i. If there is a currently running process
        if (current_process)
        {
    
            current_process->remaining_cpu_time--;                      // a. Decrement the process's remaining_cpu_time variable. This is effectively counting for the last iteration (not the current one)
            current_process->time_spent_in_a_queue++;                   //increase the counter keeping track of how long a specific process has spent in  given queue
            
            //Checks each priority to the associated time to see whether the process has used all of their time. service time - remaining cpu will give how                            
            switch(current_process->priority) {                             //Check whether a process has run for the allocated time for their associated queue
                case 0:
                    if (current_process->time_spent_in_a_queue >= t0 && current_process->remaining_cpu_time > 0) {        //If the process's allocated time has expired for queue 0 but they still have cpu time remaining 
                        suspendPcb(current_process);
                        current_process->priority = 1;                                 //Set it to now be in the new queue
                        current_process->time_spent_in_a_queue = 0;
                        level1_queue = enqPcb(level1_queue, current_process);  //If it has gone through t0 time in queue 0, then add it to queue 1
                        current_process = NULL;
                    }
                    break;
                case 1:
                    if (current_process->time_spent_in_a_queue >= t1 && current_process->remaining_cpu_time > 0) {        //If the process's allocated time has expired for queue 1:
                        suspendPcb(current_process);
                        current_process->priority = 2;                                 //Set it to now be in the new queue
                        current_process->time_spent_in_a_queue = 0;
                        level2_queue = enqPcb(level2_queue, current_process);  //If it has gone through t1 time in queue 1, then add it to queue 2
                        current_process = NULL;
                    }
                    break;
                case 2: 
                    if (current_process->time_spent_in_a_queue >= t2 && current_process->remaining_cpu_time > 0) {      //If the process's allocated time has expired for queue 2:
                        suspendPcb(current_process);
                        current_process->time_spent_in_a_queue = 0;
                        level2_queue = enqPcb(level2_queue, current_process);  //If it has gone through t2 time in queue 2, then add it to queue 2
                        current_process = NULL;
                    }
                    break;
            }

            if ( (current_process) && (current_process->remaining_cpu_time <= 0) ) {     //Logic if the process is completely finished
                /*total time for the entire process - the time where the job was not ready - how
                long the process should have taken if there was no waiting time*/
                total_waiting_time += (timer - current_process->arrival_time) - current_process->service_time;  
                total_turnaround_time += timer - current_process->arrival_time;     //Only adjust turnaround time if process has completely finished
                terminatePcb(current_process);              // A. Terminate the process;
                free(current_process);                      // B. Deallocate the PCB (process control block)'s memory
                current_process = NULL;                     //Prevent other lines from executing                          
            }   
        }


        //Check to see if a new item has been added to a higher queue
        if ( (current_process) && (level0_queue) && (current_process->priority > 0) ) { //If there is a current process, level0queue has an item, and the current priority is higher than 0, the process needs to be suspended
            suspendPcb(current_process);
            switch (current_process->priority) {
                case 1:
                    insertatfrontPcb(&level1_queue, current_process);   //By using pointer of a pointer, the method insert changes the level1_queue to point to the front of the list, which is now the current_process PCB
                    break;
                case 2:
                    insertatfrontPcb(&level2_queue, current_process);
                    break;
            }
            current_process = NULL;                                     //Set current_process to NULL so that the code when no process is running can execute and assign from the queue
        }
        else if (current_process && level1_queue && (current_process->priority > 1)) {       //If current process, level1 has item, current priority is 2, then pause process and switch
            suspendPcb(current_process);
            insertatfrontPcb(&level2_queue, current_process);       //Inserting at the front of the linked list
            current_process = NULL;
        }



        // ii. If there is no running process and there is a process ready to run:
        if (!current_process && (level0_queue || level1_queue || level2_queue)) { //Dont need to check if jobs have arrived since they would not have been added to the queues if they didnt arrive yet
            if (level0_queue)                                                     //Start process by going from 0 queue to 1 and lastly to 2. Using continue if means upper queues must be empty for bottom queues to run
                current_process = deqPcb(&level0_queue);
            else if (level1_queue)
                current_process = deqPcb(&level1_queue);
            else if (level2_queue)
                current_process = deqPcb(&level2_queue);

            //The process has not run before
            if (current_process->pid == 0) {                     //pid will only be 0 if the process has never run before
                current_process->start_time = timer;
                total_response_time += timer - current_process->arrival_time;         //Current time minus when the process arrived
            }
            startPcb(current_process);
        }

        // iii. Let the dispatcher sleep for one second;
        sleep(1);
        // iv. Increment the dispatcher's timer;
        timer++;
        // v. Go back to 2.
    }

    printf("Average turnaround time is: %.3f\n", total_turnaround_time / (float)process_count);
    printf("Average waiting time is: %.3f\n", total_waiting_time / (float)process_count);
    printf("Average response time is: %.3f\n", total_response_time / (float)process_count);
    // 3. Terminate the FCFS dispatcher
    exit(EXIT_SUCCESS);
}
