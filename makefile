CFLAGS := -Wall

shell: shell_func.c shell.c shell.h
	$(CC) $(CFLAGS) shell_func.c shell.c -o shell

run:
	clear
	./shell
clean:
	rm shell
