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
    printf("Available commands: \tls\tcd\trm\tmkdir\texit\n");
    char *curDir;
    while (true) {
        curDir = NULL;
    	curDir = getcwd(curDir, 100);
    	printf("ilya-bash: %s$ ", curDir);
    	char command[10];
    	scanf("%s", command);
    	if (strcmp(command, "ls") == 0) {
    	    listDirectories(curDir);
    	    printf("\n");
    	} else if (strcmp(command, "cd") == 0) {
    	    char path[100];
    	    scanf("%s", path);
    	    if (chdir(path) != 0) {
    	        printf("Ошибка в открытии директории %s/%s\n", curDir, path);
    	    }
    	} else if (strcmp(command, "rm") == 0) {
    	    char path[100];
    	    scanf("%s", path);
    	    if (remove(path) == -1) {
    	        printf("Ошибка удаления файла %s/%s\n", curDir, path);
    	    }
    	} else if (strcmp(command, "mkdir") == 0) {
    	    char path[100];
    	    scanf("%s", path);
    	    mode_t mode = (S_IRWXU | S_IRWXG | S_IRWXO);
    	    char absolutePath[256];
    	    snprintf(absolutePath, sizeof(absolutePath), "%s/%s", curDir, path);
    	    if (mkdir(absolutePath, mode) != 0) {
    	       printf("Error in creating a directory, try again\n");
    	    }
    	} else if (strcmp(command, "exit") == 0) {
    	    break;
    	} else {
    	    printf("Unknown command %s, try again\n", command);
    	}
    	free(curDir);
    }
    exit(0);
}
