/*
Name: Raghav Kirpekar
Roll No.: 1610110271

Name: Shivam Bansal
Roll No.:1610110351
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define MAX_SLEEP_SECS 3

/* Globals */
double *totalEatingTime;
int *chopsticks;
int numberOfPhilosophers, hasEaten, terminated;
pthread_mutex_t mutex;
pthread_cond_t cond_wait, cond_pause, cond_resume;

/* Function that makes the thread sleep for random secs*/
void randomSleep(int philosopher_number){
    double randSleepTime;
    srand(time(0));
    randSleepTime = (rand() % MAX_SLEEP_SECS) + 1.0;
    totalEatingTime[philosopher_number] += randSleepTime;
    sleep(randSleepTime);
}

void pickup_chopsticks(int philosopher_number){
    pthread_mutex_lock(&mutex);

    while (!chopsticks[philosopher_number] || !chopsticks[(philosopher_number+1) % numberOfPhilosophers])
        pthread_cond_wait(&cond_wait, &mutex);

    chopsticks[philosopher_number] = 0;
    printf("Philosopher %d picked up chopstick #%d\n", philosopher_number,philosopher_number);

    chopsticks[(philosopher_number+1) % numberOfPhilosophers] = 0;
    printf("Philosopher %d picked up chopstick #%d\n", philosopher_number,(philosopher_number+1) % numberOfPhilosophers);

    pthread_mutex_unlock(&mutex);
}

void return_chopsticks(int philosopher_number){
    pthread_mutex_lock(&mutex);

    chopsticks[philosopher_number] = 1;
    printf("Philosopher %d put down chopstick #%d\n", philosopher_number,philosopher_number);

    chopsticks[(philosopher_number+1) % numberOfPhilosophers] = 1;
    printf("Philosopher %d put down chopstick #%d\n", philosopher_number,(philosopher_number+1) % numberOfPhilosophers);

    hasEaten++;

    pthread_cond_signal(&cond_wait);
    pthread_mutex_unlock(&mutex);
}

/* Function to emulate philosopher activity cycle */
void *philosopherActivity(void *arg){
    int philosopher_number = *((int *) arg);
    while (!terminated){
        printf("Philosopher %d is hungry!\n", philosopher_number);
        pickup_chopsticks(philosopher_number);

        printf("Philosopher %d is eating!\n", philosopher_number);
        randomSleep(philosopher_number);

        return_chopsticks(philosopher_number);

        printf("Philosopher %d is thinking!\n", philosopher_number);

        pthread_mutex_lock(&mutex);
        if(hasEaten == numberOfPhilosophers)
            pthread_cond_signal(&cond_pause);
        while (hasEaten > 0)
            pthread_cond_wait(&cond_resume, &mutex);
        pthread_mutex_unlock(&mutex);
    }
    free(arg);
}

/* Main */
int main(int argc, char *argv[]){
    int i, *temp;
    char input = 'y';
    time_t startTime, endTime;
    pthread_t *philosopherThreads;

    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond_wait,NULL);
    pthread_cond_init(&cond_pause,NULL);
    pthread_cond_init(&cond_resume,NULL);

    startTime = time(0); // Store program start time
    if(argc < 2){
        puts("Command should be of type: ./philosophers <number>");
        exit(1);
    }

    numberOfPhilosophers = atoi(argv[1]);

    if (numberOfPhilosophers > 0){
        philosopherThreads = (pthread_t *) malloc(numberOfPhilosophers * sizeof(pthread_t));
        totalEatingTime = (double *) malloc(numberOfPhilosophers * sizeof(double));
        chopsticks = (int *) malloc(numberOfPhilosophers * sizeof(int));
        for(i = 0; i < numberOfPhilosophers; i++){
            chopsticks[i] = 1;
            temp = (int *) malloc(sizeof(int));
            *temp = i;
            pthread_create(&philosopherThreads[i], NULL, philosopherActivity, (void *) temp);
        }

        /* Program Loop */
        while(input == 'y' || input == 'Y'){
            pthread_mutex_lock(&mutex);
            while (hasEaten < numberOfPhilosophers)
                pthread_cond_wait(&cond_pause, &mutex);
            printf("Do you want to continue(y/n): ");
            scanf(" %c", &input);
            if(input != 'y' && input != 'Y'){
                terminated = 1;
            }
            hasEaten = 0;
            for (i=0; i < numberOfPhilosophers; i++){
                pthread_cond_signal(&cond_resume);
            }
            pthread_mutex_unlock(&mutex);
        }
        for(i=0; i < numberOfPhilosophers; i++){
            pthread_join(philosopherThreads[i], NULL);
        }

        /* Summary Section */
        endTime = time(0); // Store program end time
        puts("<-- Summary -->");
        printf("Total number of philosophers: %d\n", numberOfPhilosophers);
        printf("Total Time taken: %.2f secs\n\n", difftime(endTime, startTime));
        for(i=0; i < numberOfPhilosophers; i++){
            printf("Time taken by philosopher %d: %.2f secs\n", i, totalEatingTime[i]);
        }

        /* Cleanup Section */
        free(philosopherThreads);
        free(totalEatingTime);
        free(chopsticks);
    }
    else {
        puts("Invalid Input. It should be a valid non-zero positive integer.");
    }

    return 0;
}
