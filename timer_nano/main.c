#include <stdlib.h>

#include "timer_nano.h"

int main()
{
    const int FPS = 60;
    const int run_seconds = 60;
    const long long _NANOS_PER_SEC = NANOS_PER_SEC; /* Defined in timer_nano.h. */
    timer_nano* frame_timer = timer_nano_init();
    long long nanos_per_frame = NANOS_PER_SEC / FPS;
    int frame_max = FPS * run_seconds;
    int frame_counter = 0;
    char* output;

    print(stdout, strfmt(output,
                        "The program started. %d frames will be ran...\n",
                        frame_max)
            );

    while (frame_counter < frame_max)
    {
        if (timer_nano_alarm(*frame_timer, nanos_per_frame))
        {
            frame_counter++;

            print(stdout, strfmt(output,
                                "Frame number %d ran...\n",
                                frame_counter)
                    );

            timer_nano_reinit(frame_timer);
        }
    }
    
    print(stdout, strfmt(output, 
                        "The program ended. %d frames were run.\n",
                        frame_counter)
         );

    exit(EXIT_SUCCESS);
}
