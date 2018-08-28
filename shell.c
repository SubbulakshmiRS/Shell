#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LENGTH 1024
#define NUM 50
#define DELIM " \t\r\n\a"

int tokens_len,bufsize;
char cwd[MAX_LENGTH],hostname[MAX_LENGTH];
char * user ;
char home[MAX_LENGTH];
char cur_rel[MAX_LENGTH];
//char * cur_rel;


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
        // Read a character
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


void prompt()
{
    int i;

    if (getcwd(cwd, sizeof(cwd)) == NULL)
        perror("getcwd() error");
    
    char * h_str = home;
    char * c_str = cwd;
    int a =strlen(h_str),b=strlen(c_str);

    cur_rel[0] = '~';
    for(i=a;i<b;i++)
        cur_rel[i-a+1] = cwd[i];
    cur_rel[i-a+1] = '\0';

    printf("<%s@%s:%s>",user,hostname,cur_rel);

    /*free(c_str);
    free(h_str);*/
    return ;

}

void find_how_back()
{
    char * ret = malloc(MAX_LENGTH*sizeof(char));
    strcpy(ret,"");
    char * h_str = home;
    char * c_str = cwd;
    int a =strlen(h_str),b=strlen(c_str);

    for(int i = a;i<b;i++)
        if (cwd[i] == '/')
            strcat(ret,"../");

    if (chdir(ret) < 0)
        printf("directory not changed\n");
    
    free(ret);
    free(c_str);
    free(h_str);
    return ;
}

int main() 
{
    char * line ;
    char ** tokens;
    hostname[MAX_LENGTH-1] = '\0';
    gethostname(hostname, MAX_LENGTH-1);
    user = getenv("USER");
    getcwd(home,sizeof(home));

    while (1)
    {
        prompt();
        line = get_line();
        tokens = get_tokens(line);
        printf("%d\n",tokens_len);

        if (strcmp(tokens[0],"cd") == 0)
        {
            if( tokens_len == 1)
            {
                find_how_back();               
            }
            else if (tokens_len == 2)
            {
                if (chdir(tokens[1]) < 0)
                    printf("directory not changed\n");
            }
            else if (tokens_len > 2)
            {
                printf("more number of arguments than needed %d\n",tokens_len);
                continue;
            }
        }

        else if (strcmp(tokens[0],"pwd") == 0)
            if (tokens_len == 1)
                printf("%s\n",cwd);
            else 
            {
                printf("more number of arguments than needed\n");
                continue;
            }

        else if (strcmp(tokens[0],"echo") == 0)
        {
            char * p_str = malloc(bufsize*sizeof(char));

            strcpy(p_str,"");
            for (int i=1;i<tokens_len;i++)
            {
                strcat(p_str,tokens[i]);
                strcat(p_str," ");
            }
            printf("%s\n",p_str);
            free(p_str);
        }           
    }

    free(user);
    free(line);
    for(int i =0;i<tokens_len;i++)
        free(tokens[i]);
    free(tokens);
    return 0;
}