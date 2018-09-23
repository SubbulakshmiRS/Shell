#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "main.h"

//for ctrl+C
void sighandler_c(int signum) {

    if (current_pid != atoi(shell_pid) && current_pid != 0)
    {
        for(int i=0;i<p_len;i++)
            if(current_pid == PROC[i].pid)
            {
                if (PROC[i].stat == -1)
                    kill(current_pid,SIGKILL);
                return ;
            }
        kill(current_pid,SIGKILL); 
    }
} 

//for Ctrl+Z
void sighandler_z(int signum) {
    int n = getpid();
    int ppid = getppid();

    if(n != atoi(shell_pid))
    {
        store_pid(n,ppid);
        kill(n,SIGSTOP);
    }
}