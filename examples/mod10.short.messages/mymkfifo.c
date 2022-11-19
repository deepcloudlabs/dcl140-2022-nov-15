#include <sys/types.h> 
#include <sys/stat.h>
#include <stdlib.h>

#define DIE(x) perror(x),exit(1) 

main() { 

  if(mknod("pipe1",S_IFIFO | 0666,0) == -1) 
    DIE("FIFO1"); 

  if(mkfifo("pipe2",0666) == -1) 
    DIE("FIFO2"); 
}
