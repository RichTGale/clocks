
#include <stdlib.h>

#include "timer_nano.h"

int main()
{
    const int FPS = 10;
    const int run_seconds = 2;
    const long long _NANOS_PER_SEC = NANOS_PER_SEC; /* Defined in timer_nano.h. */
    timer_nano* frame_timer = timer_nano_init();
    long long nanos_per_frame = NANOS_PER_SEC / FPS;
    int frame_counter = 0;
    char* tstamp;
            
    fprintf(stderr, 
            "[ %s ] The program started...\n",
            (tstamp = timestamp()));
    free(tstamp);

    while (frame_counter < FPS * run_seconds)
    {
        if (timer_nano_alarm(*frame_timer, nanos_per_frame))
        {
            frame_counter++;
		    fprintf(stderr, 
		            "[ %s ] A frame passed...\n",
		            (tstamp = timestamp()));
            free(tstamp);

            timer_nano_reinit(frame_timer);
        }
    }
    
    fprintf(stderr, 
            "[ %s ] The program ended...\n",
            (tstamp = timestamp()));
    free(tstamp);

    exit(EXIT_SUCCESS);
}
