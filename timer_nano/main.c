
#include <stdlib.h>

#include "timer_nano.h"

int main()
{
    const int FPS = 10;
    const int run_seconds = 2;
    const long long _NANOS_PER_SEC = NANOS_PER_SEC; /* Defined in timer_nano.h. */
    timer_nano* frame_timer = timer_nano_init();
    long long nanos_per_frame = NANOS_PER_SEC / FPS;
    int frame_max = FPS * run_seconds;
    int frame_counter = 0;
    char* tstamp;
    char* output;

    output = strfmt(output, 
                    "[ %s ] The program started. %d frames will be ran...\n", 
                    (tstamp = timestamp()), frame_max);
    free(tstamp);
    fprintf(stdout, "%s", output);
    free(output);

    while (frame_counter < frame_max)
    {
        if (timer_nano_alarm(*frame_timer, nanos_per_frame))
        {
            frame_counter++;

            output = strfmt(output, 
                    "[ %s ] Frame number %d ran...\n", 
                    (tstamp = timestamp()), frame_counter);
	        free(tstamp);
	        fprintf(stdout, "%s", output);
            free(output);

            timer_nano_reinit(frame_timer);
        }
    }
    
    output = strfmt(output, 
                    "[ %s ] The program ended. %d frames were run.\n", 
                    (tstamp = timestamp()), frame_counter);
    free(tstamp);
    fprintf(stdout, "%s", output);
    free(output);

    exit(EXIT_SUCCESS);
}
