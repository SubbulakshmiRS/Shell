#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

#include "main.h"

//for ls and we have to find how back we have to go to home
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

//storing the list of files in current directory
char ** list_all(char * cur,int * listsize )
{
    struct dirent *de;  
    char ** list = malloc(MAX_LENGTH*sizeof(char *));
    int l = MAX_LENGTH;

    DIR *dr = opendir(cur);
    if (dr == NULL)  // opendir returns NULL if couldn't open directory
    {
        printf("Could not open current directory" );
        return 0;
    }

    int pos = 0;
    while ((de = readdir(dr)) != NULL)
    {
        if (pos >= l)
        {
            l += MAX_LENGTH;
            list = realloc(list,l*sizeof(char *));
        } 
        list[pos] = de->d_name;
        pos++;

    }
    closedir(dr);
    *listsize = pos;
    return list ;
}

//print the list out 
char ** list_out_ls(char * cur,int * listsize)
{
    char ** list = list_all(cur,listsize);
    int l = *listsize;
    for(int i = 0;i<l;i++)
        if (list[i][0] != '.')
            printf("%s  ",list[i]);
    printf("\n"); 
    return list; 
}
