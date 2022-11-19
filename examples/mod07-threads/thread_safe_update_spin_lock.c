#define _GNU_SOURCE
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
static int counter=0;

static pthread_spinlock_t splock ;

void* fun(void* param){
    static int core=0;
    int n = core++;
    printf("setting affinity: %d\n",n);
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(n,&cpuset);
    pthread_t current_thread = pthread_self();
    pthread_setaffinity_np(current_thread,sizeof(cpu_set_t),&cpuset);
    int i; 

    for (i=0;i<1000000;++i){
       pthread_spin_lock(&splock);
       ++counter;
       pthread_spin_unlock(&splock);
    } 
    return 0;
} 

int main(){
     int i;
     int number_of_cores = sysconf(_SC_NPROCESSORS_ONLN);
     printf("cores: %d\n",number_of_cores);
     pthread_t *threads = (pthread_t *) 
                malloc(number_of_cores * sizeof(pthread_t));
     pthread_spin_init(&splock, 0);
     for (i=0; i<number_of_cores; ++i){
         pthread_create(threads+i, NULL, fun, NULL);
     }
     for (i=0; i<number_of_cores; ++i){
         pthread_join(threads[i], NULL);
         printf("Thread %d has joined.\n",i);
     }
     printf("counter: %d\n",counter);
     free(threads);
     return 0;
}
