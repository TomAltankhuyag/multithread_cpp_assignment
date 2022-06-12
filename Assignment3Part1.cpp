// Author: Dagvadorj (Tom) Altankhuyag

#include <thread>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <errno.h>
#include <stdlib.h>//standard 
#include <cstdio>
#include <cstring>
#include <sys/mman.h>//mmap
#include <semaphore.h> //semaphore()
#include <time.h>//time()
#include <chrono>
using namespace std::chrono;

int chopstickTotal;
double time1,time2,time3,time4,time5;

struct WaiterMonitor {

    int chopsticks;

    sem_t actionKey, nextAction,
            leftChopstick, rightChopstick;

    int nextCount, leftChopstickCount, rightChopstickCount;

    void init(){
        chopsticks = chopstickTotal;
        sem_init(&actionKey, 0,1);

        sem_init(&nextAction, 0, 0);
        nextCount = 0;

        sem_init(&leftChopstick,0,0);
        sem_init(&rightChopstick, 0,0);
        leftChopstickCount = 0;
        rightChopstickCount = 0;
    }

    void destroy(){
        sem_destroy(&actionKey);
        sem_destroy(&nextAction);
        sem_destroy(&leftChopstick);
        sem_destroy(&rightChopstick);   
    }

    void condition_wait(sem_t &request, int &requestCount){

        requestCount++;

        if(nextCount > 0){
            sem_post(&nextAction);
        }else{
            sem_post(&actionKey);
        }

        sem_wait(&request);
        requestCount--;
    }

    void condition_post(sem_t &request, int &requestCount){

    if(requestCount > 0){

        nextCount++;

        sem_post(&request);
        sem_wait(&nextAction);
        nextCount--;
    }

    }

    void request_left_chopstick(){
        sem_wait(&actionKey);

        while(chopsticks < 2){
            condition_wait(leftChopstick, leftChopstickCount);
        }

        chopsticks--;

        if(chopsticks>= 1){
            condition_post(rightChopstick, rightChopstickCount);

            if(chopsticks >= 2){
                condition_post(leftChopstick, leftChopstickCount);
            }
        }
        
        if(nextCount > 0){
            sem_post(&nextAction);
        }else{
            sem_post(&actionKey);
        }
    }///////////////END OF REQUEST LEFT CHOPSTICK

    void request_right_chopstick(){
        sem_wait(&actionKey);

    while(chopsticks < 1){
        condition_wait(rightChopstick, rightChopstickCount);
    }

    chopsticks--;
    if(chopsticks>= 1){
            condition_post(rightChopstick, rightChopstickCount);

            if(chopsticks >= 2){
                condition_post(leftChopstick, leftChopstickCount);
            }
        }

        if(nextCount > 0){
            sem_post(&nextAction);
        }else{
            sem_post(&actionKey);
        }

    }/////////////////////////END OF REQUEST RIGHT CHOPSTICK


    void return_chopsticks(){
        sem_wait(&actionKey);

        chopsticks += 2;

        if(chopsticks>= 1){
            condition_post(rightChopstick, rightChopstickCount);

            if(chopsticks >= 2){
                condition_post(leftChopstick, leftChopstickCount);
            }
        }

        if(nextCount >0){
            sem_post(&nextAction);
        }else{
            sem_post(&actionKey);
        }
    }///////////////////END OF RETURN CHOPSTICK



};

struct WaiterMonitor waiter;

void * eater (void * arg){
    int id = *((int*)arg);
    
    srand(time(NULL) + id);
    double time = 0;
   
    for(int i = 0; i < 3; i++){
        
        printf("Philosopher %d is now thinking\n", id);

        int think_time = 1 +(rand()%5);

        sleep(think_time);
        auto start = high_resolution_clock::now();
        printf("Philosopher %d is now hungry\n", id);

        waiter.request_left_chopstick();

        printf("Philosopher %d has picked up left chopstick\n", id);

        waiter.request_right_chopstick();
        auto stop = high_resolution_clock::now();
        printf("Philosopher %d has picked up right chopstick\n",id);
        printf("Philosopher %d is now eating\n",id);
        auto duration = duration_cast<microseconds>(stop - start);
        time = time + (duration.count()/ 1000000.0);
        sleep(5);
        printf("Philosopher %d is now done eating\n",id);

        waiter.return_chopsticks();
        
    }

   
    switch(id){
        case 1:
        time1 = time / 3.0;
        break;
        case 2:
        time2 = time / 3.0;
        break;
        case 3:
        time3 = time / 3.0;
        break;
        case 4:
        time4 = time / 3.0;
        break;
        case 5:
        time5 = time / 3.0;
        break;
    }
    
pthread_exit(NULL);

}


int main(int argc, char * argv[]){

//auto start = high_resolution_clock::now();
   if(argc >= 2)
    chopstickTotal = atoi(argv[1]);
    else chopstickTotal = 8;
    
  
    waiter.init();
    pthread_t eater1,eater2,eater3,eater4,eater5;
   // sleep(1);
//auto stop = high_resolution_clock::now();

//auto duration = duration_cast<milliseconds>(stop - start);
//printf("time : %f\n", duration.count()/1000.0);

    int id1= 1;
    int id2 = 2;
    int id3 = 3;
    int id4 = 4;
    int id5 = 5;
    ////////USER INPUTTED RUN
    #if 1
    pthread_create(&eater1, NULL, eater, (void*) &id1);
    pthread_create(&eater2, NULL, eater, (void*) &id2);
    pthread_create(&eater3, NULL, eater, (void*) &id3);
    pthread_create(&eater4, NULL, eater, (void*) &id4);
    pthread_create(&eater5, NULL, eater, (void*) &id5);

    pthread_join(eater1, NULL);
    pthread_join(eater2, NULL);
    pthread_join(eater3, NULL);
    pthread_join(eater4, NULL);
    pthread_join(eater5, NULL);
    printf("Philosopher 1 has waited on average %fs to eat their three dishes in this run\n", time1);
    
    printf("Philosopher 2 has waited on average %fs to eat their three dishes in this run\n", time2);
    
    printf("Philosopher 3 has waited on average %fs to eat their three dishes in this run\n", time3);
    
    printf("Philosopher 4 has waited on average %fs to eat their three dishes in this run\n", time4);
    
    printf("Philosopher 5 has waited on average %fs to eat their three dishes in this run\n", time5);
    #endif
   
    
    ////////////END OF USER INPUTTED RUN




    #if 0
    for(int i = 0; i < 6; i++){
        chopstickTotal = i +5;
        printf("\n\nRUNNING %d CHOPSTICKS\n\n", i +5);
        for(int j = 0; j < 5; j++){
            waiter.init();
            pthread_create(&eater1, NULL, eater, (void*) &id1);
            pthread_create(&eater2, NULL, eater, (void*) &id2);
            pthread_create(&eater3, NULL, eater, (void*) &id3);
            pthread_create(&eater4, NULL, eater, (void*) &id4);
            pthread_create(&eater5, NULL, eater, (void*) &id5);

            pthread_join(eater1, NULL);
            pthread_join(eater2, NULL);
            pthread_join(eater3, NULL);
            pthread_join(eater4, NULL);
            pthread_join(eater5, NULL);

            printf("\nPhilosopher 1 has waited on average %fs to eat their three dishes in this run\n", time1);
    
            printf("Philosopher 2 has waited on average %fs to eat their three dishes in this run\n", time2);
    
            printf("Philosopher 3 has waited on average %fs to eat their three dishes in this run\n", time3);
    
            printf("Philosopher 4 has waited on average %fs to eat their three dishes in this run\n", time4);
    
            printf("Philosopher 5 has waited on average %fs to eat their three dishes in this run\n", time5);
    
        }

        printf("\n\nFINISHED RUNNING %d CHOPSTICKS\n\n", i +5);
    }
    #endif



    waiter.destroy();
}