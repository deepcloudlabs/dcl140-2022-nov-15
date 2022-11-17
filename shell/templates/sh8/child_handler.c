#include <sys/types.h>
#include <wait.h>
#include <errno.h>

void child_handler(int signo) {
    int status;
    pid_t pid;
    printf("child_handler %d\n\n",signo);
    for (;;){
       pid = waitpid(-1,&status, WNOHANG);
       if (pid == 0){
          break;
       } else if ( pid == -1 && errno == ECHILD){
          break;
       } else if( pid == -1) {
         perror("waitpid");
         abort();
       }
       display_status(pid,status);
    }

    return;
}
