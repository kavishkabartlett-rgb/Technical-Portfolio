#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
typedef struct {
    int *array;
    int start;
    int end;
    int size;
} Queue;

//Defining Global Variables
Queue queue_tutor_release, queue_tutor_rooms;                                                      //Use queues to ensure elements get performed in order
int N, M, K, T, num_students_arrived=0, num_student_to_answer_teacher=0, num_available_tutor_id=0, num_extra_tutors_to_be_dissmissed_tutor=0, num_extra_tutors_to_be_dissmissed_teacher=0,
num_next_group_up=0, num_groups_left_teacher=0, num_groups_left_tutor=0, 
num_lab_number_for_tutor_release= -1, num_tutors_left=0;
pthread_t *array_student_system_ids, *array_tutor_system_ids;                                            //Arrays allow tutors threads to work independently and at the same time - instead of changing a number,
int *array_student_custom_ids, *array_group_assignments, *array_tutor_custom_ids, *array_group_numbers,  //the tutor threads change an index in an array. Since we know no other tutor thread will ever be assigned the same group,
*array_lab_assignments, *array_lab_assignments_backwards, *array_student_arrived, *array_lab_time, *array_tutor_says_leave,     //this means we do not need to worry about threads changing the same data at the same as an another tutor
*array_all_students_left, *array_tutor_message;                                                                               

//Mutexes and condition variables
pthread_mutex_t mutex, mutex_lab;                                                                         //Mutexes to lock access
pthread_cond_t cond_students_arrived, cond_teacher_assigned_groups, cond_student_read_ids,                //Condition Variable to determine when all students have arrived, when students have been assigned
cond_teacher_waiting_room, cond_tutor_room_available_for_teacher, cond_teacher_assigned_room, cond_all_students_arrived,
cond_tutor_says_leave, cond_students_left_room, cond_tutor_release, cond_tutor_message;
bool bool_teacher_assigned_group = false, bool_student_read_ids = false, bool_teacher_waiting_for_room = false;        //booleans to pair along with conditional variables

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------QUEUE LOGIC-----------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void queue_initialize(Queue *q, int total_size) {       //Initialize Queue
    q->start = 0;                                       //Start keeps track of first location of array
    q->end = -1;                                        //End keeps track of where to add an element
    q->size = 0;                                        //Size of queue
    q->array = malloc(total_size*sizeof(int));          //Malloc data for the queue
}       


void place_in_queue(Queue *q, int tutor_id) {           //When placing in the queue, update end and size                                            
    q->end++;
    q->array[q->end] = tutor_id;
    q->size++;
}

int remove_from_queue(Queue *q) {                       //Return the value in the array, decrease size, and add one to size to 'pop' the element
    int tutor_id = q->array[q->start];
    q->start++;
    q->size--;
    return tutor_id;
}

int return_size(Queue *q) {                             //Returns array size without popping
    int current_size = q->size;
    return current_size;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------STUDENT ROUTINE---------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void* student_routine(void* arg) {
    int student_id = *(int*)arg;                                                    //Casts the void* to an int* and dereferences the pointer to get the actual value
    int group_assignment = 0;                                                       //Group number assigned
    int lab_number = 0;                                                             //Lab number assigned 
    int group_number = 0;                                                           //How many group members in students group
    free(arg);                                                                      //Free malloc for arg

    //--------------------------------------------------------------------------STUDENTS ARRIVING--------------------------------------------------------------------------//

    //Lock mutex, print statement, and increment counter
    pthread_mutex_lock(&mutex);                                                     //Lock the mutex because we are accessing students arrived
    printf("Student %d: I have arrived and wait for being assigned to a group \n", student_id);     
    num_students_arrived++;                                                         //increment counter for number of students

    //Adjust conditional variable
    if (num_students_arrived == N) 
        pthread_cond_signal(&cond_students_arrived);                                //Signal that all students have arrived

    //--------------------------------------------------------------STUDENTS RESPOND TO BEING ASSIGNED GROUP---------------------------------------------------------------//
    
    while (num_student_to_answer_teacher != student_id || bool_teacher_assigned_group == false)    //Since teacher does a broadcast, must check to make sure its the correct student
        pthread_cond_wait(&cond_teacher_assigned_groups, &mutex);
    printf("Student %d: OK, I'm in group %d and waiting for my turn to enter a lab room\n", student_id, array_group_assignments[student_id]);                                                                                             
    group_assignment = array_group_assignments[student_id];
    bool_student_read_ids = true;                                                       //Says that student has printed their group statement
    bool_teacher_assigned_group = false;                                                //Resets the condition to allow the teacher to continue assigning groups
    pthread_cond_signal(&cond_student_read_ids);

    //--------------------------------------------------------------STUDENTS RESPOND TO BEING ASSIGNED LAB-----------------------------------------------------------------//
    while (array_lab_assignments[group_assignment] == M)                                //Since lab assignments was initilized to M, if it does not equal M then the group was not assigned
        pthread_cond_wait(&cond_teacher_assigned_room, &mutex);     
    lab_number = array_lab_assignments[group_assignment];                               //Lab was assigned, so array_lab_assignment will have the correct number
    group_number = array_group_numbers[group_assignment];
    printf("Student %d in group %d: My group is called. I will enter the lab room %d now.\n", student_id, group_assignment, lab_number);
    array_student_arrived[group_assignment]++;                                          //Keeps track of how many students have arrived, where index is the group number and value is how many have arrived
    if (array_student_arrived[group_assignment] == array_group_numbers[group_assignment]) {    //Check to see if every student for that group has arrived                
        pthread_cond_broadcast(&cond_all_students_arrived);                                    //Tell tutor that everyone has arrived                            
    }
    pthread_mutex_unlock(&mutex);

//--------------------------------------------------------------------------LAB ACTIVITIES BEGIN--------------------------------------------------------------------------//
    pthread_mutex_lock(&mutex_lab);                                                     //Use different mutex because students assigning and students performing lab activities do not and should not depend on each other
    while(array_tutor_says_leave[group_assignment] != 1)                                //Wait until the value for their group is 1 and they are allowed to leave
        pthread_cond_wait(&cond_tutor_says_leave, &mutex_lab);
    printf("Student %d in group %d: Thanks Tutor %d. Bye!\n", student_id, group_assignment, lab_number);
    array_all_students_left[group_assignment]++;                                        //Keeps track of how many students have left the room
    if (array_all_students_left[group_assignment] == array_group_numbers[group_assignment])
        pthread_cond_broadcast(&cond_students_left_room);
    pthread_mutex_unlock(&mutex_lab); 
    return NULL;                                                                        //After student has left the room, they are complete
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------TEACHER ROUTINE---------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void* teacher_routine(void* arg) {                                  
    printf("Teacher: I'm waiting for all students to arrive.\n");   //Teacher waits for students to arrive
    
    //---------------------------------------------------------------------TEACHER ASSIGNING GROUPS----------------------------------------------------------------------//
    //Teacher needs to wait for all students to arrive before assigning
    pthread_mutex_lock(&mutex);                                               //Ensure data isn't changed while deciding whether to wait or progress  
    while (num_students_arrived != (N)) {                                     //The while loop ensures that if the cond signal was already sent, the wait line wont execute and the teacher will continue
        pthread_cond_wait(&cond_students_arrived, &mutex);
    }
    printf("Teacher: All students have arrived. I start to assign groups ids to students\n");

    //The teacher needs to randomly assign students, so randomly reorganize the array of student IDs
    srand(time(NULL));
    for (int i=N-1; i>0; i--) {                                               //Goes backwards so that when choosing a random number, it will start from a high index 
        int random = rand() % (i+1);                                          //Choose random that is a valid index
        int temp = array_student_custom_ids[i];
        array_student_custom_ids[i] = array_student_custom_ids[random];       //Swaps the random position in the array with the current position of the array
        array_student_custom_ids[random] = temp;
    }

    //Since students array is now random, teacher can assign students in order
    int group_counter = 0;
    for (int i=0; i<N; i++) {
        array_group_assignments[array_student_custom_ids[i]] = group_counter;                           //Use the random array as the index to ensure a random group is getting assigned a group number
        array_group_numbers[group_counter]++;                                                           //Adds one to keep track of how many students in a particular group
        printf("Teacher: Student %d is in group %d\n", array_student_custom_ids[i], group_counter);
        num_student_to_answer_teacher = array_student_custom_ids[i];                                    //Keeps track of the number for which student should be responding 
        bool_teacher_assigned_group = true;                                                             
        pthread_cond_broadcast(&cond_teacher_assigned_groups);                                          //Wakes up student threads so they can respond
        while (bool_student_read_ids == false)                                                          //Dont really need the boolean (just one teacher thread), but included it in case of spurious awakening
            pthread_cond_wait(&cond_student_read_ids, &mutex);
        group_counter++;                                                                                
        if (group_counter == M)
            group_counter = 0;                                                                          //Resets group counter to 0 to ensure that the first groups will get any extra students 
        bool_student_read_ids = false;
    }

    //-------------------------------------------------------------------AFTER STUDENTS HAVE READ OUT IDs-----------------------------------------------------------------//
    
    printf("Teacher: I'm waiting for lab rooms to become available\n");
    bool_teacher_waiting_for_room = true;
    pthread_cond_signal(&cond_teacher_waiting_room);

    //----------------------------------------------------------------------AFTER LAB BECOMES AVAILABLE-------------------------------------------------------------------//
    /*if (M<K) {
        while (num_extra_tutors_to_be_dissmissed_teacher != 0)
        while (queue_tutor_release.size == 0)                                                   //Again, use a queue to ensure that the teacher does not miss a tutor said they finished
            pthread_cond_wait(&cond_tutor_release, &mutex);
        num_lab_number_for_tutor_release = remove_from_queue(&queue_tutor_release);
        printf("TEACHER: There are no students waiting. Tutor %d, you can go home now\n", num_lab_number_for_tutor_release);
        array_tutor_message[num_lab_number_for_tutor_release] = 1;                              //Tells tutor with this index that they are free to go (1 represents they can leave)
        pthread_cond_broadcast(&cond_tutor_message);
        pthread_mutex_unlock(&mutex);
        num_extra_tutors_to_be_dissmissed_teacher--;  
    }*/
   
   
    while (num_groups_left_teacher != 0 || return_size(&queue_tutor_rooms) != 0) {              //Teacher should run as long as there is a tutor whose room is available and there are groups waiting                                            
        while (return_size(&queue_tutor_rooms) == 0)                                            //Uses queue size to ensure that the teacher does not miss any tutors who have said their room is available
            pthread_cond_wait(&cond_tutor_room_available_for_teacher, &mutex);
        num_available_tutor_id = remove_from_queue(&queue_tutor_rooms);                         //Removing from the queue ensures a tutors room is not available for the teacher twice in row
        printf("Teacher: The lab %d is now available. Students in group %d can enter the room and start your lab exercise\n", num_available_tutor_id, num_next_group_up);
        array_lab_assignments[num_next_group_up] = num_available_tutor_id;                      //Index is group number and value is assigned lab number
        array_lab_assignments_backwards[num_available_tutor_id] = num_next_group_up;            //Index is lab number and value is assigned group
        num_next_group_up++;                                                                    //Keeps track of which group should be assigned next
        num_groups_left_teacher--;                                                              //Tracker for how many groups still need to be assigned (was initialized to M)
        bool_teacher_waiting_for_room = true;                                                   //Tells tutor teacher is ready for another room
        pthread_cond_broadcast(&cond_teacher_assigned_room);                                    //Tells students to begin entering the lab
        if (num_groups_left_teacher == 0)                                                       
            pthread_mutex_unlock(&mutex);                                                       //Ensures that if the teacher does not run again, it does not keep holding onto the mutex
    }
    while (num_tutors_left != 0) {                                                              //No one but teacher accessess num_tutors_left so no need for mutex
        pthread_mutex_lock(&mutex_lab);                                                         //Use different mutex because now we are dealing with tutor releasing
        while (queue_tutor_release.size == 0)                                                   //Again, use a queue to ensure that the teacher does not miss a tutor said they finished
            pthread_cond_wait(&cond_tutor_release, &mutex_lab);
        num_lab_number_for_tutor_release = remove_from_queue(&queue_tutor_release);
        printf("TEACHER: There are no students waiting. Tutor %d, you can go home now\n", num_lab_number_for_tutor_release);
        array_tutor_message[num_lab_number_for_tutor_release] = 1;                              //Tells tutor with this index that they are free to go (1 represents they can leave)
        pthread_cond_broadcast(&cond_tutor_message);
        pthread_mutex_unlock(&mutex_lab);
        num_tutors_left--;                                                                      //Keeps track of how many tutors there are left to release 
    }
    for (int i=0; i<K; i++)                                                                     //Wait for all tutor threads to complete
        pthread_join(array_tutor_system_ids[i], NULL);
    printf("Teacher: All students and teachers have left. I can now go home\n");
    pthread_mutex_unlock(&mutex_lab);
    return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------TUTOR ROUTINE----------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void* tutor_routine(void *arg) {
    int tutor_id = *(int*)arg;                                                            //Ensures the void* is an int* 
    int group_assignment = -1;                                                            //Keeps track of the group assigned to this tutor
    free(arg);                                         
    pthread_mutex_lock(&mutex);                                                                   
    while (num_groups_left_tutor != 0) {                                                  //See if there are still groups left to be assigned. Uses different counter than teacher for simplicity 
        num_groups_left_tutor--;                                                          //If the while is executing, then this tutor will eventually be getting assigned
        while (bool_teacher_waiting_for_room == false)                                    //Makes sure the teacher needs a room
            pthread_cond_wait(&cond_teacher_waiting_room, &mutex);
        
        //----------------------------------------------------------------ONCE TEACHER IS READY FOR A ROOM----------------------------------------------------------------//
        
        /*if (M<K && num_extra_tutors_to_be_dissmissed_tutor != 0) {           //For the case when M<K
            printf("Tutor %d: The lab room %d is vacated and ready for one group.\n", tutor_id, tutor_id);
            pthread_cond_signal(&cond_tutor_release);                                               //Tells teacher tutor is ready to be dismissed
            place_in_queue(&queue_tutor_release, tutor_id);                                         //Tutor is now ready to be dismissed
            while (array_tutor_message[tutor_id] != 1)  
                pthread_cond_wait(&cond_tutor_message, &mutex);
            printf("Tutor %d: Thanks Teacher. Bye!\n", tutor_id);
            pthread_mutex_unlock(&mutex);
            num_extra_tutors_to_be_dissmissed_tutor--;
            return NULL;
        }*/

        printf("Tutor %d: The lab room %d is vacated and ready for one group.\n", tutor_id, tutor_id); 
        place_in_queue(&queue_tutor_rooms, tutor_id);                                     //Ensures this tutor will get assigned no matter what
        pthread_cond_signal(&cond_tutor_room_available_for_teacher);                      //Signals to the teacher that a room is open
        pthread_cond_broadcast(&cond_teacher_waiting_room);                               //Since room activation done, signal another tutor thread to activate as well
        array_lab_assignments_backwards[tutor_id] = M;                                    //Since the condition for the while loop has this line in it but teacher may not have assigned new group, reset it to default value
        while (array_student_arrived[array_lab_assignments_backwards[tutor_id]] != array_group_numbers[array_lab_assignments_backwards[tutor_id]])     //Check if all students have arrived - will be -1 and -2 when M is the group assignment
            pthread_cond_wait(&cond_all_students_arrived, &mutex);                        //Wait for all of the students to enter the room 
        
        //------------------------------------------------------------------AFTER STUDENTS ENTERED ROOM-------------------------------------------------------------------//
        
        group_assignment = array_lab_assignments_backwards[tutor_id];                     //If all students have arrived, then that means that group has been assigned a lab room
        printf("Tutor %d: All students in group %d have entered the room %d. You can start your exercise now\n", tutor_id, group_assignment, tutor_id);
        array_lab_time[group_assignment] = rand() % (T - ((T+1)/2) + 1) + ((T+1)/2);      //Finds the group number, and uses that as index for the time it should take for assignment
        pthread_mutex_unlock(&mutex);                                                     //By unlocking the mutex, this allows the teacher/tutors to keep assigning rooms while the students complete the activity 
        sleep(array_lab_time[group_assignment]);
        
        //------------------------------------------------------------------AFTER ACTIVITY IS COMPLETED-------------------------------------------------------------------//
        
        pthread_mutex_lock(&mutex_lab);                                                   //Use a different mutex because lab activities and dismissal does not rely on assignment
        printf("Tutor %d: Students in group %d have completed the lab exercise in %d units of time. You may leave this room now\n", tutor_id, group_assignment, array_lab_time[group_assignment]);
        array_tutor_says_leave[group_assignment] = 1;                                      //Confirm to students in group (the index) they have permission to leave
        pthread_cond_broadcast(&cond_tutor_says_leave);
        while (array_all_students_left[group_assignment] != array_group_numbers[group_assignment])
            pthread_cond_wait(&cond_students_left_room, &mutex_lab);
        
        //--------------------------------------------------------------------ONCE STUDENTS HAVE LEFT---------------------------------------------------------------------//
        
        if (num_groups_left_tutor != 0) {
            pthread_mutex_unlock(&mutex_lab);
            pthread_mutex_lock(&mutex); 
        }
    }
    pthread_cond_signal(&cond_tutor_release);                                               //Tells students they can now leave
    place_in_queue(&queue_tutor_release, tutor_id);                                         //Tutor is now ready to be dismissed
    while (array_tutor_message[tutor_id] != 1)  
        pthread_cond_wait(&cond_tutor_message, &mutex_lab);
    
    //---------------------------------------------------------------------ONCE TEACHER DISMISSES TUTOR----------------------------------------------------------------//

    printf("Tutor %d: Thanks Teacher. Bye!\n", tutor_id);
    pthread_mutex_unlock(&mutex_lab);
    return NULL;
}                                                                      




int main() {
    printf("Enter the total number of students in the class, the total number of groups, the number of lab rooms, and the time limit for each group: ");  //Request user info
    scanf("%d %d %d %d", &N, &M, &K, &T);                                                                         //Scan and assign input
    if (N < M)
        M = N;
    if (M < K) 
        M = K;
        //num_extra_tutors_to_be_dissmissed_tutor = K-M;
        //num_extra_tutors_to_be_dissmissed_teacher = K-M;
    //Initialize Values
    pthread_mutex_init(&mutex, NULL);                               //Set default behavior for the mutex
    pthread_mutex_init(&mutex_lab, NULL);                               //Mutex to control activities inside of lab
    pthread_cond_init(&cond_students_arrived, NULL);                //Set default behavior for the conditional variable
    pthread_cond_init(&cond_teacher_assigned_groups, NULL);  
    pthread_cond_init(&cond_student_read_ids, NULL);
    pthread_cond_init(&cond_teacher_waiting_room, NULL);
    pthread_cond_init(&cond_tutor_room_available_for_teacher, NULL);
    pthread_cond_init(&cond_all_students_arrived, NULL);
    pthread_cond_init(&cond_tutor_says_leave, NULL);
    pthread_cond_init(&cond_students_left_room, NULL);
    pthread_cond_init(&cond_tutor_release, NULL);
    pthread_cond_init(&cond_tutor_message, NULL);
    pthread_cond_init(&cond_teacher_assigned_room, NULL);
    num_groups_left_tutor = M;                                      //Want both the teacher and the tutor to keep track of how many groups left to be assigned
    num_groups_left_teacher = M;
    num_tutors_left = K;


    //Create data structures to record thread IDs
    pthread_t teacher_id;                                     //Variable to hold the singular teacher ID
    array_student_system_ids = malloc(N*sizeof(pthread_t));   //Array that will store the system generated thread IDs, need N threads for N students
    array_student_custom_ids = malloc(N*sizeof(int));         //Array that stores custom created IDs for each student
    array_tutor_system_ids = malloc(K*sizeof(pthread_t));     //Stores system tutor IDs
    array_tutor_custom_ids = malloc(K*sizeof(int));           //Stores custom tutor IDs
    array_group_assignments = malloc(N*sizeof(int));
    array_group_numbers = malloc((M+1)*sizeof(int));          //Stores how many group members in each group
    array_group_numbers[M] = -1;
    array_student_arrived = calloc(M+1, sizeof(int));         //1 if all the students in the group (index) have arrived. M+1 is explained in tutor thread
    array_student_arrived[M]= -2;
    array_lab_assignments = malloc(M*sizeof(int));            //Stores the lab assigned for each group
    array_lab_assignments_backwards = malloc(M*sizeof(int));  //Stores lab number as index, group number assigned as value
    for (int i = 0; i<M; i++) {
        array_lab_assignments_backwards[i] = M;               //explained in the tutor thread
        array_lab_assignments[i] = M;
    }
    array_lab_time = malloc(M*sizeof(int));                   //Stores the time assigned to each lab (random)
    array_tutor_says_leave = calloc(M, sizeof(int));          //1 says for that group (the index), the tutor has said ok to leave
    array_all_students_left = calloc(M, sizeof(int));         //keeps track of how many student have not yet left
    array_tutor_message = calloc(M, sizeof(int));             //1 says that the tutor can say their message, index is lab number
    queue_initialize(&queue_tutor_release, K);
    queue_initialize(&queue_tutor_rooms, K);


    //Create teacher, student, tutor threads and assign IDs to appropriate data structure
    pthread_create(&teacher_id, NULL, teacher_routine, NULL);                                      //Creates the teacher thread
    for (int i=0; i<N; i++) {
        int *student_id = malloc(sizeof(int));                                                             //Need to send pointer as argument for thread routine
        array_student_custom_ids[i] = i;                                                           //Assigns a number to each student
        *student_id = array_student_custom_ids[i];                                                         //Dereferences the pointer and to change the value
        pthread_create(&array_student_system_ids[i], NULL, student_routine, student_id);                   //creates each individual student thread
    }
    for (int i=0; i<K; i++) {                                                                      //Creating tutor thread with same logic as student
        int *tutor_id = malloc(sizeof(int));
        array_tutor_custom_ids[i] = i;
        *tutor_id = array_tutor_custom_ids[i];
        pthread_create(&array_tutor_system_ids[i], NULL, tutor_routine, tutor_id);
    }

    //Wait for all threads to finish completing and allow the operating system to free all thread resources
    for (int i=0; i<N; i++)                                   //Wait for the student threads
        pthread_join(array_student_system_ids[i], NULL);
    for (int i=0; i<K; i++)                                   //Wait for tutor threads
        pthread_join(array_tutor_system_ids[i], NULL);
    pthread_join(teacher_id, NULL);                           //Wait for the teacher thread
    printf("Main thread: All students have completed their lab exercises. This is the end of the simulation.");

    //Manually free any dynamically used memory
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex_lab);
    pthread_cond_destroy(&cond_students_arrived);
    pthread_cond_destroy(&cond_teacher_assigned_groups);
    pthread_cond_destroy(&cond_student_read_ids);
    pthread_cond_destroy(&cond_teacher_waiting_room);
    pthread_cond_destroy(&cond_tutor_room_available_for_teacher);
    pthread_cond_destroy(&cond_all_students_arrived);
    pthread_cond_destroy(&cond_tutor_says_leave);
    pthread_cond_destroy(&cond_students_left_room);
    pthread_cond_destroy(&cond_tutor_release);
    pthread_cond_destroy(&cond_tutor_message);
    pthread_cond_destroy(&cond_teacher_assigned_room);
    free(array_student_system_ids);
    free(array_student_custom_ids);
    free(array_tutor_system_ids);
    free(array_tutor_custom_ids);
    free(array_group_assignments);
    free(array_group_numbers);
    free(array_student_arrived);
    free(array_lab_assignments);
    free(array_lab_assignments_backwards);
    free(array_lab_time);
    free(array_tutor_says_leave);
    free(array_all_students_left);
    free(array_tutor_message);
    free(queue_tutor_release.array);
    free(queue_tutor_rooms.array);
    return 0;                                           //Represents a successful execution
}