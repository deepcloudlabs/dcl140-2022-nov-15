#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS	5

pthread_t tid[NUM_THREADS];

void* sleeping(void *arg){
    int sleep_time = *((int*)(&arg));
    fprintf(stderr, "Thread %li is sleeping for %d seconds.\n",(unsigned long int)pthread_self(),sleep_time);
    sleep(sleep_time);
    return ((void*)sleep_time);
}

void thread_start(void){
    int i;
    for (i=0;i<NUM_THREADS;++i){
        pthread_create(&tid[i], NULL,sleeping,(void*)(5+rand() % 5)); 
    }
}

void thread_wait(void){
    int i,*return_value;
    for (i=0;i<NUM_THREADS;++i){
        pthread_join(tid[i],(void**)(&return_value) );
        fprintf(stderr, "Thread %li return %d\n", (unsigned long int)tid[i],(int)return_value);
    }
}

int main(){
    srand(time(NULL));
    thread_start();
    thread_wait();
    return 0;
}
