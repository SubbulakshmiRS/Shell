# *SHELL*
A simple shell written completely in C ,using system calls and function libraries .

## *Install and Run*
Installation :
> git clone https://github.com/SubbulakshmiRS/Shell

Compile and Run :
> make
> 
> ./a.out

## *Commands*
The commands which have been implemented are from scratch are :
> **cd** 
> 
> **ls (flags -l,-a)**
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


## *Files*
shell.c
>The main file containing all the inputs 
>
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