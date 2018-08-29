#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "stat_ls.c"
#include "args.c"
#include "pinfo_data.c"


#define MAX_LENGTH 1024
#define NUM 50
#define DELIM " \t\r\n\a"

char * line ;
char ** tokens;
char ** list;
int listsize;
char cwd[MAX_LENGTH],hostname[MAX_LENGTH];
char * user ;
char home[MAX_LENGTH];
char cur_rel[MAX_LENGTH];
int pid ;
int background;
char shell_pid[MAX_LENGTH];

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
    /*free(c_str);
    free(h_str);*/
    return ;
}

char ** list_all(char * cur)
{
    struct dirent *de;  // Pointer for directory entry
    char ** list = malloc(MAX_LENGTH*sizeof(char *));
    listsize = MAX_LENGTH;

    // opendir() returns a pointer of DIR type. 
    DIR *dr = opendir(cur);
    if (dr == NULL)  // opendir returns NULL if couldn't open directory
    {
        printf("Could not open current directory" );
        return 0;
    }

    int pos = 0;
    while ((de = readdir(dr)) != NULL)
    {
        if (pos >= listsize)
        {
            listsize += MAX_LENGTH;
            list = realloc(list,listsize*sizeof(char *));
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
    list = list_all(cur);
    for(int i = 0;i<listsize;i++)
        if (list[i][0] != '.')
            printf("%s  ",list[i]);
    printf("\n"); 
    return ; 
}

void check_background()
{
    pid = -1;
    background = 0;
    for(int i=0;i<tokens_len;i++)
        if (strcmp(tokens[i],"&") == 0)   
            background = 1;
    pid = fork();
}

int main() 
{
    hostname[MAX_LENGTH-1] = '\0';
    gethostname(hostname, MAX_LENGTH-1);
    user = getenv("USER");
    getcwd(home,sizeof(home));
    int x = getpid();
    sprintf(shell_pid, "%d", x);

    while (1)
    {
        prompt();
        line = get_line();
        tokens = get_tokens(line);
        //printf("%d\n",tokens_len);

        check_background();

        if (pid == 0)
        {
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

            else if(strcmp(tokens[0],"ls") == 0)
            { 
                if (tokens[1] == NULL)
                    list_out_ls("."); 
                else if (strcmp(tokens[1],"-l") == 0)
                {
                    char * cur = (tokens[2] != NULL)?tokens[2]:(".");
                    list = list_all(cur);
                    for(int i = 0;i<listsize;i++)
                        if (list[i][0] != '.')
                        {
                            stat_file(list[i]);
                        } 
                }
                else if (strcmp(tokens[1],"-a") == 0)
                {
                    char * cur = (tokens[2] != NULL)?tokens[2]:(".");
                    list = list_all(cur);
                    for(int i = 0;i<listsize;i++)
                        printf("%s  ",list[i]);
                    printf("\n");
                }
                else if (strcmp(tokens[1],"-al") == 0 || strcmp(tokens[1],"-la") == 0)
                {
                    char * cur = (tokens[2] != NULL)?tokens[2]:(".");
                    list = list_all(cur);
                    for(int i = 0;i<listsize;i++)
                    {
                        stat_file(list[i]);
                    }                
                }
                else
                {
                    char * cur = (tokens[2] != NULL)?tokens[2]:(".");
                    list_out_ls(cur);                
                }
                            
            }
            else if (strcmp(tokens[0],"pinfo") == 0)
            {
                if (tokens[1] == NULL)
                    pid_data(shell_pid);
                else 
                    pid_data(tokens[1]);
            }
            else 
                execvp(tokens[0],tokens);
        }
        else if (background == 0 && pid != -1)
            wait(NULL);//background stops
    }

    free(user);
    free(line);
    for(int i =0;i<tokens_len;i++)
        free(tokens[i]);
    free(tokens);
    for(int i =0;i<listsize;i++)
        free(list[i]);
    free(list);
    return 0;
}