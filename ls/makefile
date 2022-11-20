CC = gcc
CFLAGS = -fsanitize=address

ls: ls.o checker
	$(CC) $(CFLAGS) ls.o -o ls

ls.o: ls.c
	$(CC) ls.c -o ls.o

checker:
	cppcheck ls.c
