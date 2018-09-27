# *SHELL*
A simple shell written completely in C ,using system calls and function libraries . The shell can be used for redirection and piping ( | , > , < , >>)

## *Install and Run*
Installation :
> git clone https://github.com/SubbulakshmiRS/Shell

Compile and Run :
> make
> 
> ./a.out

Clean the files :
> make clean


## *Commands*
The commands which have been implemented are from scratch are :
> **cd** 
> 
> **ls (flags -l,-a)**
> 
> **Ctrl+C**
> 
> **Ctrl+Z**
> 
> **pwd**
> 
> **echo** 

Personal commands have also been implemented :
> **clock -t** 
>
> This command prints every n seconds (argument ) the system local time .This operation will stop by Ctrl+C .
> *example*: clock -t 3 - every 3 seconds

> **remindme** 
>
> This command prints a reminder (statement is an argument) after n seconds (argument) .This is a background process .
> *example*: remindme 10 hello - this prints "hello" after 20 seconds .

> **pinfo**
>
> This command prints the information of any process whose pid is passed . If no number is passed , then the process information of the running base shell is printed out .
> *example*: pinfo 

> **setenv** 
>
> Can create or overwrite values of environment variables of the shell
> *example*: setenv NUKE_PATH /SharedDisk/Nuke/

> **unsetenv** 
>
> Can unset any environment variable .
> *example*: unsetenv NUKE_PATH

> **jobs** 
>
> Prints out all the current / forcefully stopped jobs with their process id .
> *example*: jobs 

> **kjob** 
>
> can send specific signals to jobs having the given number as shown with the above command(jobs).
> *example*: kjob 2 9

> **fg** 
>
> brings a running or a stopped background job with given job
number to foreground.
> *example*: fg 1

> **bg** 
>
> changes a stopped background job to a running background
job.
> *example*: bg 1

> **overkill** 
>
> kills all background process at once.
> *example*: overkill

> **quit / exit()** 
>
> exits out of the shell
> *example*: quit / exit()


## *Files*
Makefile
> For creating the object files and executables . Also for deleting unneccessary files by make clean .

main.h
>Header file containing all function declarations and global variables .

shell.c
>The main file containing all the inputs 

args.c
>Contains all functions for the taking inputs and turning into tokens.

commands.c
>Contains the code for calling all functions for executing the built-in commands 

directory.c
>Contains function for changing directories and making a list of all files in that folder 

stat_ls.c 
>Contains the code for collecting and printing out all the information for all files like the bash -"ls -l " - command .

time_file.c
>Contains the functions for getting the system local time .

pinfo_data.c
>For a given pid , all information from the system files are given about the process .By default the pid is that of the base parent process (shell).

bg_check.c
>Contains the commands to check and print the condition of all background processes

signal_handle.c
>Contains the functions for handling SIGCHLD , SIGINT and SIGSTOP signals .

store.c
>Functions for storing details about current or force-stopped background processes . (for jobs command)


