#include <stdio.h>
int main( int argc, char *argv[]) {

  int m;

  for( m = 0; m < argc; m++ ) {
    printf("Argument %d = %s\n", m, argv[m]);
  }
  return 0;
}
