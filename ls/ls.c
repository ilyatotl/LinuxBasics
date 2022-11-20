#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

// gcc ls.c -o ls
// ./ls

int main() {
    char *curDir;
    curDir = getcwd(curDir, 0);
    DIR *dir = opendir(curDir);
    
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
    	if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0) {
    	    continue;
    	}
    	printf("%s\t", entry->d_name);
    }
    printf("\n");
    closedir(dir);
    free(curDir);
    exit(0);
}
