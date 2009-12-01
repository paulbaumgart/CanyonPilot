#ifndef _UTIL_H_
#define _UTIL_H_

#include <sys/time.h>

double getMicroTime() {
  struct timeval t;
  gettimeofday(&t, NULL);
  
  return t.tv_sec + (t.tv_usec / 1.0e6);
}

#endif