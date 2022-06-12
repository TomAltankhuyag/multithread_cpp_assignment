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


sem_t actionKey;
int primeTotal;
int maxNum = 100000000;
int threadsTotal;




bool isComposite(int n){
	// Corner cases
	if (n <= 1) return false;
	if (n <= 3) return false;

	// This is checked so that we can skip
	// middle five numbers in below loop
	if (n%2 == 0 || n%3 == 0) return true;

	for (int i=5; i*i<=n; i=i+6)
		if (n%i == 0 || n%(i+2) == 0)
		return true;

	return false;
}




void * func (void * arg){
    int id = *((int*)arg);
    int start, stop;
    int portion = id - 1;
   
    if(id==threadsTotal){
        start = (portion * (int)(maxNum/threadsTotal));
        stop = maxNum;
        
    }else{

     start = (portion * (int)(maxNum/threadsTotal));
     stop = ((portion+1) * (maxNum /threadsTotal)) - 1;
     
    }
   if(start == 0)start++;
    for(int i = start; i < stop + 1 ; i++){
    
      if(!isComposite(i)){
           sem_wait(&actionKey);
      primeTotal++;
      sem_post(&actionKey);
      }
       
       
    }
    
    printf("THREAD NUMBER : %d\n",id );
    printf("START NUMBER: %d\n", start);
    printf("END NUMBER: %d\n", stop);

    
pthread_exit(NULL);

}

int main(int argc, char * argv[]){
//for this assignments, the range of number will be from 0 to 100,000,000

primeTotal = 0;

 sem_init(&actionKey, 0,1);

if(argc >= 2)
 threadsTotal = atoi(argv[1]);
else
threadsTotal = 2;

pthread_t threads[threadsTotal];
int id[threadsTotal];

for(int i = 0; i < threadsTotal; i++){
    id[i] = i + 1;
    pthread_create(&threads[i],NULL,func,(void*) &id[i]);
}

    for(int i = 0; i < threadsTotal; i++){
    pthread_join(threads[i],NULL);
    }
    printf("\nThere are %d composite numbers\n",maxNum - primeTotal);
    sem_destroy(&actionKey);

}


// printf("thread : %d\n",i+1);
// printf("start number: %d\n", i * (int)(maxNum/threads));
// printf("end number : %d\n", ((i+1) * (maxNum /threads)) );


// printf("thread : %d\n",i+1);
// printf("start number: %d\n", i * (int)(maxNum/threads));
// printf("end number : %d\n", maxNum);