#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "main.h"

//take in the input 
char * get_line()
{
    if (r != 0)
    {
        printf("KCBJSDF\n");
        return line;
    }
    
    cur = 0;
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
            r = 1;
            return buffer;
        } 
        else
        {
            if (c == '|')
                pipeline++ ;
            buffer[position] = c;
        } 
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

//evaluate the command and break it to meaningful pieces for piping and such.
char * evaluate()
{
    printf("dnvkj %d\n",cur);
    char * buffer = malloc(sizeof(char)*MAX_LENGTH);
    int c,position = 0;
    bufsize = MAX_LENGTH;

    if (!buffer)
    {
        printf("Allocation error\n");
        exit(1);
    }

    while (1) 
    {
        c = line[cur];
        //printf("char %c\n",c);

        // If we hit EOF, replace it with a null character and return.
        if (c == EOF || c == '\n')
        {
            buffer[position] = '\0';
            r = 0;
            cur = 0;
            return buffer;
        } 
        else if (c == '<')
        {
            redirect = 1;
            char f[20] ;
            scanf("%s",f);
            int file = open(f, O_RDONLY | O_CREAT); 
            if(file < 0)
            {
                printf("unable to open file %s\n",f);
                exit(1);
            }
            dup2(file,0);
            redirect_file = file;
        }
        else if (c == '>')
        {
            redirect = 2;
            char f[20] ;
            scanf("%s",f);
            int file = open(f, O_WRONLY| O_APPEND| O_CREAT);        
            if(file < 0)
            {
                printf("unable to open file %s\n",f);
                exit(1);
            }
            redirect_file = file;
            dup2(file,1);
        }
        else if(c == '|')
        {
            printf("%d\n",cur);
            pcur++;
            if (pipe(pipe_file[pcur-1]) != 0) {
                perror("bad pipe");
                exit(1);
            }
            printf("dfj\n");
            if (pcur == 1)
            {
                dup2(pipe_file[0][1],1);
                close(pipe_file[0][0]);
                close(pipe_file[0][1]);
            }
            else
            {
                dup2(pipe_file[pcur - 2][0],0);
                dup2(pipe_file[pcur - 1][1],1);
            }
            //printf("bcjds\n");
            cur++ ;
            return buffer ;
        }
        else
        {
            buffer[position] = c;
            position++;
        }
        cur++ ;

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
        //printf("%d %d %c\n",position,cur,line[cur]);
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
