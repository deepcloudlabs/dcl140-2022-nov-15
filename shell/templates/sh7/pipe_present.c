/*
 *  pipe_present.c :  check for | 
 */

#include <stdio.h>
#include "shell.h"

/*
 * Return index offset into argv of where "|" is,
 * -1 if in an illegal position (first or last index in the array),
 * or 0 if not present.
 */
int pipe_present(char ** myArgv) {
  int i;

  /* Search through myCurrentArgv for a match on "|". */
  for (i=0;myArgv[i] != NULL;++i){
       if (myArgv[i][0]== '|'){
          break;
       }
   }  
  if ((i==0) || ((myArgv[i] != NULL) && (myArgv[i+1] == NULL))) {
    return -1;
  } else if (myArgv[i] == NULL ) {
    return 0;
  } else {
    return i;
  }
}
