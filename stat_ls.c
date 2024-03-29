#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#include "main.h"

//for ls -l , all info is got from stat
void stat_file(char * path)
{
    if (path == NULL || strcmp(path,"") == 0)
        return ;
    struct group *grp;
    struct passwd *pwd;
    struct stat fileStat;
    char * alt = malloc(MAX_LENGTH*sizeof(char));

    strcpy(alt,path);

    if(stat(alt,&fileStat) < 0)    
    {
        printf("stat error for file: %s\n",path);
        return ;
    }

    pwd = getpwuid(fileStat.st_uid);
    grp = getgrgid(fileStat.st_gid);
    char * time = ctime(&fileStat.st_mtime);
    printf("%s ",path);
    printf("%d ",(int)fileStat.st_size);
    printf("%d ",(int)fileStat.st_nlink);
    printf("%s ", pwd->pw_name);
    printf("%s ", grp->gr_name);
    printf(" ");
    printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
    printf(" %s",time);
 
    free(alt);
}