/**
 * main.c
 *
 * This file demonstrates how the timer_nano library may be utilized.
 * The timer_nano type is a timer that has nanosecond precision.
 *
 * Author(s): Richard Gale
 * Version 1.0
 */

#include <stdlib.h>

#include "timer_nano.h"

/**
 * This function demonstrates the timer_nano timer, a timer with nanosecond 
 * precision, running for 5 seconds at 60 frames per second.
 */
int main()
{
    const int FPS = 60;                                 /* The number of frames per second. */
    const int run_seconds = 5;                          /* The number of seconds the program will run for. */
    const long long _NANOS_PER_SEC = NANOS_PER_SEC;     /* The number of nanoseconds in a second. (Defined in timer_nano.h) */
    timer_nano* frame_timer = timer_nano_init();        /* The timer that will time how much time each frame will last. */
    long long nanos_per_frame = NANOS_PER_SEC / FPS;    /* The number of nano seconds each frame will last. */
    int frame_max = FPS * run_seconds;                  /* The number of frames the program will run. */
    int frame_counter = 0;                              /* The number of frames that have been run. */
    char* output;                                       /* Output to filestream(s). */

    /* Recording the time of the start of the program (or thereabouts). */
    print(stdout, strfmt(output,
                        "The program started. %d frames will be ran...\n",
                        frame_max)
            );

    /* Looping until all the frames have ran. */
    while (frame_counter < frame_max)
    {
        /* When the timer goes off, run a frame. */
        if (timer_nano_alarm(*frame_timer, nanos_per_frame))
        {
            /* Recording that a frame ran. */
            frame_counter++;

            /* Outputting sa message. */
            print(stdout, strfmt(output,
                                "Frame number %d ran...\n",
                                frame_counter)
                    );

            /* Restarting the timer. */
            timer_nano_reinit(frame_timer);
        }
    }
    
    /* Outputting an end-of-program message. */
    print(stdout, strfmt(output, 
                        "The program ended. %d frames were run.\n",
                        frame_counter)
         );

    /* Exiting the program. */
    exit(EXIT_SUCCESS);
}
