#include <signal.h>

main() {

  return kill(getpid(), SIGABRT);
}
