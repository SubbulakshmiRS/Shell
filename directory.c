#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

#define MAX_LENGTH 1024

void find_how_back(char * h_str,char * c_str)
{
    char * ret = malloc(MAX_LENGTH*sizeof(char));
    strcpy(ret,"");
    int a =strlen(h_str),b=strlen(c_str);

    for(int i = a;i<b;i++)
        if (c_str[i] == '/')
            strcat(ret,"../");

    if (chdir(ret) < 0)
        printf("directory not changed\n");
    
    free(ret);
    return ;
}

char ** list_all(char * cur,int * listsize )
{
    struct dirent *de;  
    char ** list = malloc(MAX_LENGTH*sizeof(char *));
    listsize = MAX_LENGTH;

    DIR *dr = opendir(cur);
    if (dr == NULL)  // opendir returns NULL if couldn't open directory
    {
        printf("Could not open current directory" );
        return 0;
    }

    int pos = 0;
    while ((de = readdir(dr)) != NULL)
    {
        if (pos >= (*listsize))
        {
            listsize += MAX_LENGTH;
            list = realloc(list,(*listsize)*sizeof(char *));
        } 
        list[pos] = de->d_name;
        pos++;

    }
    closedir(dr); 
    listsize = pos;   
    return list ;
}

void list_out_ls(char * cur)
{
    int * listsize ;
    char ** list = list_all(cur,&listsize);
    for(int i = 0;i<(*listsize);i++)
        if (list[i][0] != '.')
            printf("%s  ",list[i]);
    printf("\n"); 
    return ; 
}
