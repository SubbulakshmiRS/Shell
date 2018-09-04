CC = gcc
CFLAGS = -Wall -c
OBJ = shell.o pinfo_data.o stat_ls.o directory.o args.o time_file.o commands.o

a.out: $(OBJ)
	$(CC) $(OBJ)

$@: shell.c
	$(CC) $(CFLAGS) $<

$@: commands.c
	$(CC) $(CFLAGS) $<

$@: directory.c
	$(CC) $(CFLAGS) $<

$@: pinfo_data.c
	$(CC) $(CFLAGS) $<

$@: stat_ls.c
	$(CC) $(CFLAGS) $<	

$@: args.c
	$(CC) $(CFLAGS) $<	

$@: time_file.c
	$(CC) $(CFLAGS) $<	

clean:
	rm -f *.o a.out