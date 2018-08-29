#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include<fcntl.h>

#define MAX_LENGTH 1024

void pid_data(char * pid)
{
    ssize_t size_read,len =0;
    char * stat = malloc(MAX_LENGTH*sizeof(char));
    char * exe = malloc(MAX_LENGTH*sizeof(char));
    char line[MAX_LENGTH];

    strcpy(stat,"/proc/");
    strcat(stat,pid);
    strcat(stat,"/stat");
    strcpy(exe,"/proc/");
    strcat(exe,pid);
    strcat(exe,"/exe");

	FILE * fd_input1 = fopen(stat,"r");
    FILE * fd_input2 = fopen(exe, "r");
	if(fd_input1 == NULL)
	{
		fprintf(stderr, "Failed to open file \n");
        return ;
	}

    int lno = 0;
    printf("pid: %s\n",pid);

    while (fscanf(fd_input1, " %1023s", line) == 1) {
        lno++;
        if(lno == 3)
            printf("Status: %s\n",line);
        else if(lno == 18)
        {
            printf("Virtual Memory: %s\n",line);
            break;
        }
    }
//do i need to tokenize it ? or make sure it doesnt cross the limit of 1024
    while (fscanf(fd_input2, " %1023s", line) == 1) {
        lno++;
        if(lno == 3)
            printf("Status: %s\n",line);
        else if(lno == 18)
        {
            printf("Virtual Memory: %s\n",line);
            break;
        }
    }

// the exe file seems to have garbage 
    /*
    lno =0;
    len = 0;
    while ((size_read = getline(&exe,&len, fd_input2)) != -1)
    {
        lno ++;
    }*/
    free(stat);
    free(exe);
	fclose(fd_input1);
	fclose(fd_input2);
    return ;
}