#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LENGTH 1024
#define NUM 50
#define DELIM " \t\r\n\a"

int tokens_len,bufsize;

//take in the input 
char * get_line()
{
    char * buffer = malloc(sizeof(char)*MAX_LENGTH);
    int c,position =0;
    bufsize = MAX_LENGTH;

    if (!buffer)
    {
        printf("Allocation error\n");
        exit(1);
    }

    while (1) 
    {
        c = getchar();

        // If we hit EOF, replace it with a null character and return.
        if (c == EOF || c == '\n')
        {
            buffer[position] = '\0';
            return buffer;
        } 
        else 
            buffer[position] = c;
        position++;

        // If we have exceeded the buffer, reallocate.
        if (position >= bufsize) 
        {
            bufsize += MAX_LENGTH;
            buffer = realloc(buffer, bufsize);
            if (!buffer) 
            {
                printf("Reallocation error\n");
                exit(1);
            }
        }
    }
}

//tokenize the input line with given delimiters
char ** get_tokens(char * input)
{
    char ** tokens = malloc(sizeof(char *)*NUM);
    char * t;
    int position  =0;
    int bufsize = NUM;
    tokens[0] = strtok(input,DELIM);
    position ++;
    t = strtok(NULL,DELIM);

    while(t)
    {
        tokens[position] = t;
        position ++;
        if (position >= bufsize )
        {
            bufsize += NUM ;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens)
            {
                printf("tokens not reallocated properly\n");
                exit(1);
            }
        }
        t = strtok(NULL,DELIM);
    }
    tokens[position] = NULL;
    tokens_len = position ;
    return tokens;
}
