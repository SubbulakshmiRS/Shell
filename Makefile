CC = gcc
CFLAGS = -Wall -c
OBJ1 = shell.o pinfo_data.o stat_ls.o directory.o args.o time_file.o commands.o 
OBJ2 = signal_handle.o bg_check.o store.o

a.out: $(OBJ1) $(OBJ2)
	$(CC) $(OBJ1) $(OBJ2)

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

$@: signal_handle.c
	$(CC) $(CFLAGS) $<	

$@: bg_check.c
	$(CC) $(CFLAGS) $<	

$@: store.c
	$(CC) $(CFLAGS) $<	

clean:
	rm -f *.o a.out