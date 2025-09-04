

#ifndef TIMER_NANO_H
#define TIMER_NANO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* strerror() */
// #include <stdbool.h>
#include <stdarg.h>
#include <time.h>
#include <errno.h>
// #include <unistd.h>
// #include <termios.h>

/**
 * This is the number of nanoseconds in a second.
 */
#define NANOS_PER_SEC 1000000000

enum timer_states 
{
    NOT_ELAPSED,
    HAS_ELAPSED
};

typedef struct timer_nano_data
{
    struct timespec initial;
    struct timespec current;
    struct timespec elapsed;
} timer_nano;

timer_nano* timer_nano_init();

void timer_nano_reinit(timer_nano* tn);

enum timer_states timer_nano_alarm(timer_nano tn, long long wait_time);

/**
 * This function dynamically allocates only the needed amount of memory to a
 * string based on the argument list, then concatenates the argument list into 
 * the supplied format and stores it in the supplied string pointer.
 */
char* strfmt(char* sp, char *fmt, ...);

/**
 * This function returns a string that represent the current time.
 * For reasons detailed in a comment within this function, you must
 * free() the string that this function returns.
 */
char* timestamp();

#endif /* TIMER_NANO_H */
