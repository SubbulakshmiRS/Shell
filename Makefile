CC = gcc
CFLAGS = -Wall -c
OBJ = shell.o pinfo_data.o stat_ls.o directory.o args.o time_file.o

a.out: $(OBJ)
	$(CC) $(OBJ)

%.o: %.c 
	$(CC) $(CFLAGS) $<
