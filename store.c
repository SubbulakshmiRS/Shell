#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "main.h"

//store info about each background process called 
void store(char * name,int pid,int x, char * statement,int ppid)
{
    if(x != 0)
    {
        PROC[p_len].pid = pid;
        PROC[p_len].name = name;
        PROC[p_len].statement = statement;
        PROC[p_len].ppid = ppid;
        p_len++;
    }

}

void store_pid(int pid,int ppid)
{
    char * stat = malloc(MAX_LENGTH*sizeof(char));
    char line[MAX_LENGTH];
    char * p ;

    sprintf(p, "%d", pid);
    strcpy(stat,"/proc/");
    strcat(stat,p);
    strcat(stat,"/comm");  
	FILE * fd_input1 = fopen(stat,"r");
	if(fd_input1 == NULL)
	{
		fprintf(stderr, "Failed to open file \n");
        return ;
	}
    fscanf(fd_input1, "%1023s", line);
    store(line,pid,1,NULL,ppid);
    PROC[p_len-1].stat = 1;
	fclose(fd_input1);
    free(stat);
    return ;
}
