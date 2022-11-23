CC = gcc
CFLAGS = -Wall -Wextra -Wcast-align -fstack-protector -fsanitize=address

sobel: sobel.o checker
	$(CC) $(CFLAGS) sobel.c -o sobel
	
sobel.o: sobel.c
	$(CC) sobel.c -o sobel.o

checker:
	cppcheck sobel.c
