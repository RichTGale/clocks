
#include "timer_nano.h"

/**
 * This function returns the number of bytes a string will need to be
 * allocated based on the variable argument list and a format string that are
 * provided to this function.
 */
size_t vbytesfmt(va_list lp, char* fmt)
{
    va_list lp_cpy; /* A Copy of the list of arguments. */
    size_t bytes;   /* The number of bytes the string needs. */

    /* Copying the argument list. */
    va_copy(lp_cpy, lp);

    /* Getting the number of bytes the string will need. Adding an extra
     * 1 char worth of bytes for the null character. */
    bytes = vsnprintf(NULL, 0, fmt, lp_cpy) + sizeof(char);

    /* Assuring a clean finish to the copy. */
    va_end(lp_cpy);

    /* Returning the number of bytes the string will need. */
    return bytes;
}

/**
 * This function dynamically allocates only the needed amount of memory to a
 * string based on the argument list, then concatenates the argument list into 
 * the supplied format and stores it in the supplied string pointer.
 */
void strfmt(char** sp, char *fmt, ...)
{
    va_list lp;     /* Pointer to the list of arguments. */
    size_t bytes;   /* The number of bytes the string needs. */

    /* Pointing to the first argument. */
    va_start(lp, fmt);

    /* Getting the number of bytes the string will need to be allocated. */
    bytes = vbytesfmt(lp, fmt);

    /* Allocating memory to the string. */
    *sp = (char*) malloc(bytes);

    /* Creating the string. */
    vsprintf(*sp, fmt, lp);

    /* Assuring a clean finish to the argument list. */
    va_end(lp);
}

/**
 * This function removes the char element from the string provided to it which
 * is at the element number/index provided to it.
 */
void sdelelem(char** sp, unsigned elem)
{
    char* to_elem;      /* Chars from start of string to element to delete. */
    char* from_elem;    /* Chars from element to delete to end of string. */
    unsigned c;         /* The current char in the string. */

    /* Allocating memory. */
    to_elem     = (char*) malloc(sizeof(char) * (elem + 1));
    from_elem   = (char*) malloc(sizeof(char) * (strlen(*sp) - elem));

    /* Storing the two sections of the string. */
    for (c = 0; c < strlen(*sp); c++)
    {
        if (c < elem)
            to_elem[c] = (*sp)[c];
        if (c > elem)
            from_elem[c] = (*sp)[c];
    }
    to_elem[elem] = '\0';
    from_elem[strlen(*sp) - elem - 1] = '\0';

    /* Recreating the string. */
    free(*sp);
    strfmt(sp, "%s%s", to_elem, from_elem);

    /* Cleaning up. */
    free(to_elem);
    free(from_elem);
}

/**
 * This function removes all cases of the provided char from the string at the
 * provided pointer.
 */
void sdelchar(char** sp, char remove)
{
    unsigned c;     /* Index of current char in the string. */

    /* Overwriting the unwanted characters. */
	for (c = 0; c < strlen(*sp); c++)
	{
        if ((*sp)[c] == remove)
        {
            sdelelem(sp, c);

            /* Decrementing the index so we will check the replacement 
             * character. */
            c--;
        }
    }
}

/**
 * This function returns a string that represent the current time.
 * For reasons detailed in a comment within this function, you must
 * free() the string that this function returns.
 */

char* timestamp()
{
    time_t current_time;    /* The current time. */
    char* stamp;            /* The time stamp. */
    char* stamp_cpy;        /* A Copy of the time stamp. */

    /* Obtaining the current time. */
    if ((current_time = time(NULL)) == ((time_t) - 1))
    {
        /* An error occured so we're printing an error message to and exiting
         * the program. */
        fprintf(stderr, 
                "ERROR: In function timestamp(): "
                "Calender time is not available\n");
        exit(EXIT_FAILURE);
    }

    /* Converting time to local time format. */
    if ((stamp = ctime(&current_time)) == NULL)
    {
        /* An error occured converting so we're printing an error message
         * and exiting the program. */
        fprintf(stderr, 
                "ERROR: In function timestamp(): "
                "Failure to convert the current time to a string.\n");
        exit(EXIT_FAILURE);
    }

    /* The string that ctime() returns is not one that should be be freed
     * with free() because the memory it uses was not allocated with malloc()
     * or a similar function. Because we are going to use sdelchar() to remove
     * the newline character that ctime() added to our timestamp, and
     * sdelchar() uses free() to remove chars from strings, we have to make
     * a copy of our stamp.
     * If this copy is not freed by the calling function, it will create a 
     * memory leak.
     */
    strfmt(&stamp_cpy, "%s", stamp);

    /* Removing the newline character that was added by ctime(). */
    sdelchar(&stamp_cpy, '\n');

    /* Returning the copy of the time stamp. */
    return stamp_cpy;
}

timer_nano* timer_nano_init()
{
    char* tstamp;

    timer_nano* tn = (timer_nano*) malloc(sizeof(timer_nano));

    if ((clock_gettime(CLOCK_REALTIME, &(tn->initial)) == -1))
    {
	    fprintf(stderr, 
	            "[ %s ] ERROR: in function timer_nano_init(): %s\n",
	            (tstamp = timestamp()), strerror(errno));

        free(tstamp);

        exit(EXIT_FAILURE);
    }


    return tn;
}

void timer_nano_reinit(timer_nano* tn)
{
    char* tstamp;

    if ((clock_gettime(CLOCK_REALTIME, &(tn->initial)) == -1))
    {
	    fprintf(stderr, 
	            "[ %s ] ERROR: in function timer_nano_reinit(): %s\n",
	            (tstamp = timestamp()), strerror(errno));

        free(tstamp);

        exit(EXIT_FAILURE);
    }
}

enum timer_states timer_nano_alarm(timer_nano tn, long long wait_time)
{
    char* tstamp;

    if ((clock_gettime(CLOCK_REALTIME, &(&tn)->current) == -1))
    {
	    fprintf(stderr, 
	            "[ %s ] ERROR: in function start_timer(): %s\n",
	            (tstamp = timestamp()), strerror(errno));

        free(tstamp);

        exit(EXIT_FAILURE);
    }

    tn.elapsed.tv_sec  = tn.current.tv_sec  - tn.initial.tv_sec;
    tn.elapsed.tv_nsec = tn.current.tv_nsec - tn.initial.tv_nsec;
    
    if ((tn.elapsed.tv_sec * NANOS_PER_SEC) + tn.elapsed.tv_nsec < wait_time)
    {
        return NOT_ELAPSED;
    }

    return HAS_ELAPSED;
}
