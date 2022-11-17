 #define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// int cores = sysconf(_SC_NPROCESSORS_ONLN);

#define ARRAY_SIZE 1000000

struct problem {
   int *array;
   int start;
   int size;
} ;

int bind_thread_to_core(int core_no){
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_no, &cpuset);

    pthread_t current_thread = pthread_self();
    return pthread_setaffinity_np(current_thread,sizeof(cpu_set_t),&cpuset);
}

void* partial_sum(void *arg){
    static int counter=0;
    int i;
    //fprintf(stderr,"partial_sum() is running\n");
    struct problem *p= (struct problem*) arg;
    int *arr = p->array + p->start;
    int part_sum = 0;
    //fprintf(stderr,"start: %d, size: %d\n",p->start,p->size);
    bind_thread_to_core(counter++);
    for(i=0;i<p->size;++i){
       part_sum += arr[i];
    }
    return (void*) part_sum;
}

int main(){
    int i,size,sum_of_array;
    int cores = sysconf(_SC_NPROCESSORS_ONLN);
    int dummy[16];
    int *my_sum;
    pthread_t *threads = (pthread_t *) malloc(cores * sizeof(pthread_t));
    struct problem *problem_instance = (struct problem *) malloc(sizeof(struct problem)*cores);
    int *array = (int *) malloc(sizeof(int) * ARRAY_SIZE);
    for(i=0;i<ARRAY_SIZE;++i)
       array[i]= 1;
    size = ARRAY_SIZE / cores;
    for (i=0;i<cores;++i){
        problem_instance[i].array = array;
        problem_instance[i].start = i*size ;
        problem_instance[i].size = size;
        fprintf(stderr,"Creating thread #%d...\n",i+1);
        pthread_create(threads+i, NULL, partial_sum, (struct problem*) &problem_instance[i]);
    }
    sum_of_array = 0;
    for (i=0;i<cores;++i){
        pthread_join(threads[i],(void**)(&my_sum));
        //fprintf(stderr,"thread #%d return %d\n",i+1,my_sum);
        sum_of_array += (int)my_sum;
        fprintf(stderr,"Partial sum is %d, %d.\n",my_sum,sum_of_array);
    }
    fprintf(stderr, "Sum: %d\n",sum_of_array);
    return 0;
}
