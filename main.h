#ifndef FOO_H_   /* Include guard */
#define FOO_H_

#define MAX_LENGTH 1024
#define NUM 50
#define DELIM " \t\r\n\a"

int current_pid,current_ppid;
int cur,r;
int Stdout,Stdin;
int redirect,pipeline,pend,pcur;
int redirect_file;
int pipe_file[10][2]; //assumption only 10 pipes will be needed
char * line ;
char * command ;
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
    int ppid;
}; //for different background processes

struct process PROC[MAX_LENGTH];
int p_len ;
int tokens_len,bufsize;

void prompt();
void sighandler_c(int signum);
void sighandler_z(int signum);
void sighandler_ch(int signum);
void store(char * name,int pid,int x, char * statement,int ppid);
void store_pid(int pid,int ppid);
int check_background();
void print_background();
char ** get_tokens(char * input);
char * get_line();
char * evaluate();
void command_fg(char ** tokens ,int tokens_len);
void command_bg(char ** tokens ,int tokens_len);
void command_kill();
void command_kjobs(char ** tokens , int tokens_len);
void command_jobs();
void command_setenv(char ** tokens,int tokens_len);
void command_unsetenv(char ** tokens,int tokens_len);
void command_cd(char ** tokens,int tokens_len,char cwd[MAX_LENGTH],char home[MAX_LENGTH]);
void command_pwd(char ** tokens,int tokens_len,char cwd[MAX_LENGTH]);
void command_echo(char ** tokens,int tokens_len);
char** command_ls(char ** tokens,int tokens_len,int * lsize);
void command_pinfo(char * shell_pid , char ** tokens,int tokens_len);
void find_how_back(char * h_str,char * c_str);
char ** list_all(char * cur,int * listsize );
char ** list_out_ls(char * cur,int * listsize);
void stat_file(char * path);
void pid_data(char * pid);
char * get_time();
void print_time();

#endif