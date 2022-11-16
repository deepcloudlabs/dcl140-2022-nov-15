/* 
 * redirect_in.c  :  check for <
 */

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "shell.h"

/*
 * Look for "<" in myArgv, then redirect input to the file.
 * Returns 0 on success, sets errno and returns -1 on error.
 */
int redirect_in(char ** myArgv) {
  int i;
  int fd;

  /* search forward for < */
  for (i=0;myArgv[i] != NULL;++i){
       if (myArgv[i][0]== '<'){
          break;
       }
   }  

  if (myArgv[i]) {	/* found "<" in vector. */

    /* Open file. */
   if(myArgv[i+1]==NULL){
      errno = EINVAL;
      return -1; 
    }   
    if( (fd = open(myArgv[i+1], O_RDONLY)) == -1){
      perror(myArgv[i+1]);
      return -1;
    }

    dup2(fd,STDIN_FILENO);
    close(fd);

    for (i=i+2;myArgv[i]!= NULL;++i)
        myArgv[i-2]= myArgv[i];
    myArgv[i-2] = NULL;

  }
  return 0;
}
