#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "main.h"

void command_jobs()
{
    printf("dfbv %d\n",p_len);
    for(int i = 0;i<p_len;i++)
        printf("[%d] %s %s[%d]\n",i+1,PROC[i].stat?"Stopped":"Running",PROC[i].name,PROC[i].pid);
}

void command_cd(char ** tokens,int tokens_len,char cwd[MAX_LENGTH],char home[MAX_LENGTH])
{
    if( tokens_len == 1)
        find_how_back(home,cwd);               
    else if (tokens_len == 2)
        {
            if (chdir(tokens[1]) < 0)
            printf(" %s directory not changed\n",tokens[1]);
        }
    else if (tokens_len > 2)
        printf("more number of arguments than needed %d\n",tokens_len);
}

void command_pwd(char ** tokens,int tokens_len,char cwd[MAX_LENGTH])
{
    if (tokens_len == 1)
        printf("%s\n",cwd);
    else 
        printf("more number of arguments than needed\n");
}

void command_echo(char ** tokens,int tokens_len)
{   
    char * p_str = malloc(MAX_LENGTH*sizeof(char));

    strcpy(p_str,"");
    for (int i=1;i<tokens_len;i++)
    {
        strcat(p_str,tokens[i]);
        strcat(p_str," ");
    }
    printf("%s\n",p_str);
    free(p_str);
}

char** command_ls(char ** tokens,int tokens_len,int * lsize)
{
    int x =0 ;
    int * listsize = &x;
    char ** list;

    if (tokens[1] == NULL)
        list=list_out_ls(".",listsize);
    else if (strcmp(tokens[1],"-l") == 0 && (tokens[2] == NULL || strcmp(tokens[2],"-a")!=0))
    {
        char * cur = (tokens[2] != NULL)?tokens[2]:(".");
        list = list_all(cur,listsize);
        int l = *listsize;
        for(int i = 0;i<l;i++)
            if (list[i] != NULL && list[i][0] != '.')
            {
                stat_file(list[i]);
            }
    }
    else if (strcmp(tokens[1],"-a") == 0  && (tokens[2] == NULL || strcmp(tokens[2],"-a")!=0))
    {
        char * cur = (tokens[2] != NULL)?tokens[2]:(".");
        list = list_all(cur,listsize);
        for(int i = 0;i<(*listsize);i++)
            printf("%s  ",list[i]);
    }
    else if (strcmp(tokens[1],"-al") == 0 || strcmp(tokens[1],"-la") == 0)
    {
        char * cur = (tokens[2] != NULL)?tokens[2]:(".");
        list = list_all(cur,listsize);
        for(int i = 0;i<(*listsize);i++)
        {
            stat_file(list[i]);
        }                
    }
    else if ((strcmp(tokens[1],"-a") == 0 && strcmp(tokens[2],"-l")== 0) || (strcmp(tokens[1],"-l")==0 && strcmp(tokens[2],"-a")==0))
    {
        char * cur = (tokens[3] != NULL)?tokens[3]:(".");
        list = list_all(cur,listsize);
        for(int i = 0;i<(*listsize);i++)
        {
            stat_file(list[i]);
        } 
    }
    else
    {
        char * cur = (tokens[1] != NULL)?tokens[1]:(".");
        list= list_out_ls(cur,listsize);                
    }
    lsize = (listsize);
    return list;
}

void command_pinfo(char * shell_pid , char ** tokens,int tokens_len)
{
    if (tokens[1] == NULL)
        pid_data(shell_pid);
    else 
        pid_data(tokens[1]);
}
