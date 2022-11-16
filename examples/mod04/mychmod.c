#include <sys/types.h> 
#include <sys/stat.h> 
#include <stdio.h> 

int main(int argc,char *argv[]) { 
  struct stat buf; 

  if (stat(argv[2],&buf)) {
    perror ("Couldn't stat file");
  } else {

    buf.st_mode = atoi(argv[1]) ; 

    if (chmod(argv[2],buf.st_mode) == -1) {
      perror("chmod : filename"); 
    }
  }
}
