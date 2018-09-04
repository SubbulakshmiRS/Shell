#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "main.h"

char * get_time()
{
    time_t current_time;
    char* c_time_string;

    current_time = time(NULL);

    if (current_time == ((time_t)-1))
    {
        (void) fprintf(stderr, "Failure to obtain the current time.\n");
        exit(EXIT_FAILURE);
    }

    /* Convert to local time format. */
    c_time_string = ctime(&current_time);

    if (c_time_string == NULL)
    {
        (void) fprintf(stderr, "Failure to convert the current time.\n");
        exit(EXIT_FAILURE);
    }

    return c_time_string;
}

void print_time()
{
    //read the time and date and print in out
    char * ans= get_time();
    printf("%s",ans);
    return ;

}