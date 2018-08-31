#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


#define MAX_LENGTH 1024

void pid_data(char * pid)
{
    ssize_t size_read,len =0,r;
    struct stat sb;
    char * stat = malloc(MAX_LENGTH*sizeof(char));
    char * exe = malloc(MAX_LENGTH*sizeof(char));
    char line[MAX_LENGTH];
    char * linkname ;

    strcpy(stat,"/proc/");
    strcat(stat,pid);
    strcat(stat,"/stat");
    strcpy(exe,"/proc/");
    strcat(exe,pid);
    strcat(exe,"/exe");

	FILE * fd_input1 = fopen(stat,"r");
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

    if (lstat(exe, &sb) == -1) {
        
        exit(EXIT_FAILURE);
    }
    else 
    {
        linkname = malloc(sb.st_size + 1);
        if (linkname == NULL) {
            fprintf(stderr, "insufficient memory\n");
            exit(EXIT_FAILURE);
        }

        r = readlink(exe, linkname, sb.st_size + 1);

        if (r < 0) {
            perror("lstat");
            exit(EXIT_FAILURE);
        }
        //printf("%d\n",r); //always prints nothing not a.out
        linkname[r] = '\0';
        printf("Executable: %s\n", linkname);
        free(linkname);
    }

    free(stat);
    free(exe);
	fclose(fd_input1);
    return ;
}