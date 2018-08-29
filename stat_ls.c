#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>

#define MAX_LENGTH 1024
 
void stat_file(char * path)
{
    struct group *grp;
    struct passwd *pwd;
    struct stat fileStat;
    char * alt = malloc(MAX_LENGTH*sizeof(char));

    strcpy(alt,path);

    if(stat(alt,&fileStat) < 0)    
    {
        printf("stat error\n");
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