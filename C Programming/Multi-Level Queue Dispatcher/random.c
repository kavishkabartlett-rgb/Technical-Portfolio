/*
==================================================================================
    Title: Job List Generator for Multi-Queue Dispatcher
    Description: Asks user for number of jobs to generate, poisson distribution mean, 
        and exponential distribution mean as the inputs, and uses these values to generate random arrival 
        and execution times, as well as random priorities. Outputs them in txt file  

    Usage:
        ./job_generator <output_filename>
    
    Input: Number of jobs to generate, poisson distribution mean, exponential distribution mean

    Output:
        A file containing job arrival and execution times in the format:
        <arrival time> <execution time> <priority>

    Author: Kavishka Bartlett (Based on code from random.c from COMP3520)
    Created: October 2024
==================================================================================
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void generate_jobs(int no_of_jobs, float lambda_arrival, float lambda_service, FILE *output_stream);

int main(int argc, char *argv[])
{
    /* File pointer */
    FILE *output_stream = NULL;
    
    /* User-defined parameters */
    int no_of_jobs = 0;
    float lambda_arrival = 0;
    float lambda_service = 0;

    /* Validate command-line arguments */
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <output_filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Prompt user for input parameters */
    do
    {
        printf("Enter the number of jobs to generate (must be at least 1): ");
        scanf("%d", &no_of_jobs);
    } while (no_of_jobs < 1);

    printf("Enter the mean interval (Poisson distribution) between job arrivals: ");
    scanf("%f", &lambda_arrival);

    printf("Enter the inverse mean of job execution time (Exponential distribution): ");
    scanf("%f", &lambda_service);

    /* Seed the random number generator */
    srand(time(0));

    /* Open file for writing */
    if ((output_stream = fopen(argv[1], "w")) == NULL)
    {
        fprintf(stderr, "ERROR: Unable to open file for writing.\n");
        exit(EXIT_FAILURE);
    }

    /* Generate job list */
    generate_jobs(no_of_jobs, lambda_arrival, lambda_service, output_stream);

    /* Close file and exit */
    fclose(output_stream);
    printf("Job list successfully generated in '%s'.\n", argv[1]);
    return EXIT_SUCCESS;
}

/*
Generates job arrival and execution times using Poisson and Exponential distributions.
Saves the generated job list in the provided file.
*/
void generate_jobs(int no_of_jobs, float lambda_arrival, float lambda_service, FILE *output_stream)
{
    int i, k = 0, arrival_time = 0, service_time = 0;
    double L, p, u;

    for (i = 0; i < no_of_jobs; i++)
    {
        /* Generate job arrival time using Poisson distribution */
        L = exp(-lambda_arrival);
        k = 0;
        p = 1;
        do
        {
            k += 1;
            u = rand() / (double)RAND_MAX;
            p *= u;
        } while (p > L);
        arrival_time += k - 1;

        /* Generate job execution time using Exponential distribution */
        u = rand() / (double)RAND_MAX;
        service_time = 1 + floor(log(1 - u) / (-lambda_service));

        /* Save job to output file */
        fprintf(output_stream, "%d, %d\n", arrival_time, service_time);
    }
}