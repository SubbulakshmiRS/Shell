#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include "main.h"

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


int main() 
{
    p_len = 0;
    hostname[MAX_LENGTH-1] = '\0';
    gethostname(hostname, MAX_LENGTH-1);
    user = getenv("USER");
    getcwd(home,sizeof(home));
    sprintf(shell_pid, "%d", getpid());
    Stdout = dup(1);
    Stdin = dup(0);

    while (1)
    {
        signal(SIGINT, sighandler_c);
        signal(SIGTSTP,sighandler_z);
        signal(SIGCHLD,sighandler_ch);
        if (pipeline == 0 && semi_end == 0)
                prompt();
        line = get_line();
        command = evaluate();

        if(strlen(command) == 0)
        {
            r = 0;
            continue;
        }

        tokens = get_tokens(command);

        if (strcmp(tokens[0],"cd") == 0)
            command_cd(tokens,tokens_len,cwd,home);
        else if (strcmp(tokens[0],"pwd") == 0)
            command_pwd(tokens,tokens_len,cwd);
        else if (strcmp(tokens[0],"echo") == 0)
            command_echo(tokens,tokens_len);
        else if(strcmp(tokens[0],"ls") == 0)
            list = command_ls(tokens,tokens_len,listsize);
        else if (strcmp(tokens[0],"pinfo") == 0)
            command_pinfo(shell_pid,tokens,tokens_len);
        else if (strcmp(tokens[0],"jobs") == 0)
            command_jobs();  
        else if (strcmp(tokens[0],"fg") == 0)
            command_fg(tokens,tokens_len); 
        else if (strcmp(tokens[0],"bg") == 0)
            command_bg(tokens,tokens_len);   
        else if (strcmp(tokens[0],"setenv") == 0)
        {
            if (getpid() == atoi(shell_pid))
                command_setenv(tokens,tokens_len);
        }
        else if (strcmp(tokens[0],"unsetenv") == 0)
        {
            if (getpid() == atoi(shell_pid))
                command_unsetenv(tokens,tokens_len);
        }
        else if (strcmp(tokens[0],"kjobs") == 0)
            command_kjobs(tokens,tokens_len);
        else if (strcmp(tokens[0],"overkill") == 0)
            command_kill();            
        else if (strcmp(tokens[0],"exit()") == 0 || strcmp(tokens[0],"quit") == 0 )
        {
            printf("\nBYE!\n");
            return 0;
        }
        else if (strcmp(tokens[0],"remindme") == 0)
        {
            int pid = fork();
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
                store("remindme",pid,1,(tokens[2]?tokens[2]:""),getpid());
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
                int cpid = pid;
                current_pid = pid;
                current_ppid = getppid();
                setpgid(pid, pid);
                //int status;
                store(tokens[0],cpid,x,NULL,getpid());
                if (x != 1)
                    pause();
                    //waitpid(cpid, &status, 0);
            }
        }

        //end of semicolon 
        if (semi_end != 0)
        {
            dup2(Stdout,1);
            dup2(Stdin,0);           
        }

        //end of redirections 
        if (redirect != 0)
        {
            close(redirect_file);
            dup2(Stdout,1);
            dup2(Stdin,0);
            redirect = 0;
            redirect_file = 0;
        }

        //end of pipings
        if (pend != 0)
        {
            for(int i = 0;i<pipeline;i++)
            {
                close(pipe_file[i][0]);
                close(pipe_file[i][1]);
            }
            pipeline = 0;
            pend = 0;
            pcur = 0;
            dup2(Stdout,1);
            dup2(Stdin,0);  
        }
    }

    free(user);
    free(line);
    free(command);
    for(int i =0;i<tokens_len;i++)
        free(tokens[i]);
    free(tokens);
    for(int i =0;i<(*listsize);i++)
        free(list[i]);
    free(list);
    return 0;
}