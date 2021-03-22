
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define NUM_THREADS 8       //number of threads
#define TOT_COUNT 500000000 //total number of iterations

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;
long gen;
float randNumGen()
{

    int random_value = rand();                          //Generate a random number
    float unit_random = random_value / (float)RAND_MAX; //make it between 0 and 1
    return unit_random;
}

/**
The task allocated to a thread
**/
void doCalcs()
{

    //obtain the integer value of thread id

    //using malloc for the return variable in order make
    //sure that it is not destroyed once the thread call is finished
    int in_count = 0;

    //get the total number of iterations for a thread
    int tot_iterations = TOT_COUNT / NUM_THREADS;

    int counter = 0;

    //calculation
    for (counter = 0; counter < tot_iterations; counter++)
    {
        float x = randNumGen();
        float y = randNumGen();

        float result = sqrt((x * x) + (y * y));

        if (result < 1)
        {
            pthread_mutex_lock(&lock);
            in_count += 1; //check if the generated value is inside a unit circle
            pthread_mutex_unlock(&lock);
        }
    }
    pthread_mutex_lock(&lock2);
    gen += in_count; //return the in count
    pthread_mutex_unlock(&lock2);
}

int main(int argc, char *argv[])
{
    clock_t begin = clock();
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;
    float tot_in = 0;

    for (t = 0; t < NUM_THREADS; t++)
    {
        rc = pthread_create(&threads[t], NULL, (void *)doCalcs, NULL);
        if (rc)
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    //join the threads
    for (t = 0; t < NUM_THREADS; t++)
    {

        pthread_join(threads[t], NULL);
        //printf("Return from thread %ld is : %f\n",t, *(float*)status);

        //keep track of the total in count
    }
    float gen2 = gen;
    printf("Value for Pi is %f \n", 4 * (gen2 / TOT_COUNT));

    /* Last thing that main() should do */
    pthread_mutex_destroy(&lock);
    pthread_mutex_destroy(&lock2);

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time Spent : %f\n", time_spent);
    return 0;
}