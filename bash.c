#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdbool.h>

void listDirectories(char *curDir) {
    DIR *dir = opendir(curDir);
    
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
    	if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0) {
    	    continue;
    	}
    	printf("%s\t", entry->d_name);
    }
    closedir(dir);
}

int main() {
    printf("Available commands: ls\tcd\texit\n");
    char *curDir;
    while (true) {
    	curDir = getcwd(curDir, 100);
    	printf("ilya-bash: %s$ ", curDir);
    	char command[10];
    	scanf("%s", command);
    	if (strcmp(command, "ls") == 0) {
    	    listDirectories(curDir);
    	} else if (strcmp(command, "cd") == 0) {
    	    char path[100];
    	    scanf("%s", path);
    	    if (chdir(path) != 0) {
    	        printf("Ошибка в открытии директории %s/%s", curDir, path);
    	    }
    	} else if (strcmp(command, "exit") == 0) {
    	    break;
    	} else {
    	    printf("Unknown command %s, try again", command);
    	}
    	printf("\n");
    	free(curDir);
    }
    exit(0);
}
