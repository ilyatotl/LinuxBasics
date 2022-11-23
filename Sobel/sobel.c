#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mtx;

const double redCoeff = 0.299, greenCoeff = 0.587, blueCoeff = 0.114;

char** src;
char** dst;
int width, height;

struct str {
    int start;
    int end;
};

int max(int a, int b) {
    return a < b ? b : a;
}

int min(int a, int b) {
    return a > b ? b : a;
}

int xGradient(int x, int y) {
    return src[y - 1][x - 1] +
           2 * src[y][x - 1] +
           src[y + 1][x - 1] -
           src[y - 1][x + 1] -
           2 * src[y][x + 1] -
           src[y + 1][x + 1];
}
 
 
int yGradient(int x, int y) {
    return src[y - 1][x - 1] +
           2 * src[y - 1][x] +
           src[y - 1][x + 1] -
           src[y + 1][x - 1] -
           2 * src[y + 1][x] -
           src[y + 1][x + 1];
}

void* SobelForLines(void* args) {
    int start = ((struct str*)args)->start;
    int end = ((struct str*)args)->end;
    for (int y = start; y < end; y++) {
        for (int x = 1; x < width - 1; x++) {
            int gx = xGradient(x, y);
            int gy = yGradient(x, y);
            int sum = abs(gx) + abs(gy);
            
            pthread_mutex_lock(&mtx);
            dst[y][x] = (char)(sum > 255 ? 255 : (sum < 0 ? 0 : sum));
            pthread_mutex_unlock(&mtx);
        }
    }
}


int main() {
    FILE *input = NULL, *output = NULL;
    if ((input = fopen("image.bmp", "r")) == NULL) {
        return -1;
    }
    
    char* info = malloc(54);
    
    for (int i = 0; i < 54; ++i) {
        info[i] = fgetc(input);
    }
    
    width = *(int*)&info[18];
    height = *(int*)&info[22];
    char r, g, b;
    int x;
    
    src = (char**)malloc(height * sizeof(char*));
    dst = (char**)malloc(height * sizeof(char*));
    for (int i = 0; i < height; ++i) {
        src[i] = malloc(width);
        dst[i] = malloc(width);
    }
    
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            r = fgetc(input);
            g = fgetc(input);
            b = fgetc(input);
            x = (int)(redCoeff * r + greenCoeff * g + blueCoeff * b) / 3;
            src[i][j] = (char)x;
            dst[i][j] = (char)0;
        }
    }
    
    pthread_mutex_init(&mtx, NULL);
    
    const int numberOfThreads = 2;
    pthread_t threads[numberOfThreads];
    struct str* args[numberOfThreads];
    
    for (int i = 0; i < numberOfThreads; ++i) {
    	args[i] = (struct str*)malloc(sizeof(struct str));
    	args[i]->start = max(1, height * i / numberOfThreads);
    	args[i]->end = min(height - 1, height * (i + 1) / numberOfThreads);
    	pthread_create(&threads[i], NULL, SobelForLines, (void*)args[i]);
    }
    
    for (int i = 0; i < numberOfThreads; ++i) {
        pthread_join(threads[i], NULL);
    }
    
    for (int i = 0; i < numberOfThreads; ++i) {
        free(args[i]);
    }
    
    pthread_mutex_destroy(&mtx);
    
    output = fopen("output.bmp", "w+");
    for (int i = 0; i < 54; ++i) {
        fputc(info[i], output);
    }
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            for (int k = 0; k < 3; ++k) {
                fputc(dst[i][j], output);
            }
        }
    }
    
    
    free(info);
    for (int i = 0; i < height; ++i) {
        free(src[i]);
        free(dst[i]);
    }
    free(src);
    free(dst);
}
