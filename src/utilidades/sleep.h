
#ifndef __SLEEP_H__
#define __SLEEP_H__

#include <time.h>

inline int milisleep(int miliseconds) {
  struct timespec tim;
  tim.tv_sec = 0;
  tim.tv_nsec = miliseconds * 1000000L;
  return nanosleep(&tim, NULL);
};


#endif
