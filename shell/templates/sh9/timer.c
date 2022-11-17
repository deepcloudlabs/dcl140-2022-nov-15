/*
 *  timer.c : contains two timing functions to measure process time
 */

#include <sys/times.h>
#include <unistd.h>
#include <stdio.h>
#include "shell.h"

/* Storage for baseline times. */
static clock_t old;

/* Save a baseline of user and system CPU times, plus wallclock time. */
void set_timer(void) {
  struct tms tmbuf;

  old = times(&tmbuf); 
}


/* Get second set of times, and take delta to get wallclock time.  Display. */
void stop_timer(void) {
  struct tms tmbuf;
  clock_t new;
  double ticks;
  ticks = sysconf(_SC_CLK_TCK);
  /* Get delta times and print them out. */
  new = times(&tmbuf); 
  if (old){
    fprintf(stderr, "Sys %6.2f User %6.2f Real %6.2f\n",tmbuf.tms_cstime/ticks,
       tmbuf.tms_cutime/ticks, (new-old)/ticks ); 
    old=0;
  }
}

