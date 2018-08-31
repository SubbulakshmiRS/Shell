#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include "commands.c"
#include "args.c"
#include "time_file.c"
//#include "directory.c"

#define MAX_LENGTH 1024
#define NUM 50
#define DELIM " \t\r\n\a"

char * line ;
char ** tokens;
char ** list;
int * listsize;
char cwd[MAX_LENGTH],hostname[MAX_LENGTH];
char * user ;
char home[MAX_LENGTH];
char cur_rel[MAX_LENGTH];
char shell_pid[MAX_LENGTH];

struct process 
{
    int pid;
    char * name ;
    int stat;
    char * statement;
};
struct process PROC[MAX_LENGTH];
int p_len ;

void sighandler(int signum) {
    if (getpid() != atoi(shell_pid))
        kill(getpid(),SIGHUP);
}

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

    return ;
}

void store(char * name,int pid,int x, char * statement)
{
    if(x != 0)
    {
        PROC[p_len].pid = pid;
        PROC[p_len].name = name;
        PROC[p_len].statement = statement;
        p_len++;
    }

}

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

void print_background()
{
    int s,pid;
    while ( (pid = waitpid(-1, &s, WNOHANG)) > 0)
        for(int i=0;i<p_len;i++)
        {
            if (PROC[i].pid == pid)
            {
                if (strcmp(PROC[i].name,"remindme") == 0)
                    printf("Reminder: %s\n",PROC[i].statement);               
                printf("Process has exited normally %d %s\n",PROC[i].pid,PROC[i].name);
                PROC[i].stat = 1;  
                break; 
            }
        }
    
    return ;
}

int main() 
{
    p_len = 0;
    hostname[MAX_LENGTH-1] = '\0';
    gethostname(hostname, MAX_LENGTH-1);
    user = getenv("USER");
    getcwd(home,sizeof(home));
    sprintf(shell_pid, "%d", getpid());

    while (1)
    {
        signal(SIGINT, sighandler);
        print_background();
        prompt();
        line = get_line();
        tokens = get_tokens(line);

        if (strcmp(tokens[0],"cd") == 0)
            command_cd(tokens,tokens_len,cwd,home);
        else if (strcmp(tokens[0],"pwd") == 0)
            command_pwd(tokens,tokens_len,cwd);
        else if (strcmp(tokens[0],"echo") == 0)
            command_echo(tokens,tokens_len);
        else if(strcmp(tokens[0],"ls") == 0)
            list = command_ls(tokens,tokens_len,&listsize);
        else if (strcmp(tokens[0],"pinfo") == 0)
            command_pinfo(shell_pid,tokens,tokens_len);
        else if (strcmp(tokens[0],"exit()") == 0)
        {
            printf("\nBYE!\n");
            return 0;
        }
        else if (strcmp(tokens[0],"remindme") == 0)
        {
            int pid = fork();
            char * statement ;
            strcpy(statement,tokens[2]);
            if (pid == 0) 
            {
                // Child process
                strcpy(tokens[0],"sleep");
                tokens[2] = NULL;
                tokens_len = 2;                
                if (execvp(tokens[0],tokens) == -1) {
                perror("lsh");
                }
                exit(EXIT_FAILURE);
            } 
            else if (pid < 0) 
            {
                // Error forking
                perror("lsh");
            } 
            else 
            {
                // Parent process
                store("remindme",pid,1,statement);
            }
           
        }
        else if(strcmp(tokens[0],"clock") == 0 && strcmp(tokens[1],"-t") == 0)
        {
            int pid = fork();
            if (pid == 0)
                while(1)
                {
                    sleep(atoi(tokens[2]));
                    print_time();
                }   
            else if (pid > 0)
            {
                int status ;
                waitpid(pid, &status, 0);
            }

        } 
        else 
        {
            int pid;
            int x=check_background();
            pid = fork();
            if (pid == 0) 
            {
                // Child process
                if (execvp(tokens[0],tokens) == -1) {
                perror("lsh");
                }
                exit(EXIT_FAILURE);
            } 
            else if (pid < 0) 
            {
                // Error forking
                perror("lsh");
            } 
            else 
            {
                // Parent process
                int status;
                store(tokens[0],pid,x,NULL);
                if (x != 1)
                    waitpid(pid, &status, 0);
            }
        }
    }

    free(user);
    free(line);
    for(int i =0;i<tokens_len;i++)
        free(tokens[i]);
    free(tokens);
    for(int i =0;i<(*listsize);i++)
        free(list[i]);
    free(list);
    return 0;
}