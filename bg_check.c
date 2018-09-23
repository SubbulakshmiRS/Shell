#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#include "main.h"

// check whether the process is a background one
int check_background()
{
    int background = 0;
    for(int i=0;i<tokens_len;i++)
        if (strcmp(tokens[i],"&") == 0)   
            {
                background = 1;
                tokens[i] = NULL;
                tokens_len--;
            }
    return background;
}

//print for all background process which have ended smoothly
void print_background()
{
    int s,pid;
    while ( (pid = waitpid(-1, &s, WNOHANG)) > 0)
        for(int i=0;i<p_len;i++)
        {
            if (PROC[i].pid == pid)
            {
                if (WIFEXITED(s))
                {
                    if (strcmp(PROC[i].name,"remindme") == 0)
                        printf("Reminder: %s\n",PROC[i].statement);
                    printf("%s with pid: %d exited normally\n",PROC[i].name,PROC[i].pid);
                }
                else if (WIFSIGNALED(s))
                    printf("%s with pid: %d killed by signal\n",PROC[i].name,PROC[i].pid);
                else if (WIFSTOPPED(s)) 
                    printf("%s with pid: %d stopped by signal\n",PROC[i].name,PROC[i].pid);  
                else 
                    printf("%s with pid: %d ended due to unknown reasons\n",PROC[i].name,PROC[i].pid);             
                PROC[i].stat = 1;  
                break; 
            }
        }
    
    return ;
}