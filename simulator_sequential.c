#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include "simulator_sequential.h"
#include <pthread.h>

int ** readElevationFile(const char * elevationFilename, int N) {
    FILE * elevationFile = fopen(elevationFilename, "r");
    if (elevationFile == NULL) {
        perror("Could not open elevation_file!");
        exit(EXIT_FAILURE);
    }
    int ** elevation = malloc(N * sizeof(*elevation));
    for (int i = 0; i < N; i++) {
        elevation[i] = malloc(N * sizeof(**elevation));
    }
    int currNum;
    int i = 0;
    while (fscanf(elevationFile, "%d", &currNum) == 1) {
        elevation[i/N][i%N] = currNum;
        i++;
    }
    if (fclose(elevationFile) != 0) {
        perror("Failed to close elevation_file!");
        exit(EXIT_FAILURE);
    }
    return elevation;
}

void freeElevation(int ** elevation, int N) {
    for (int j = 0; j < N; j++) {
        for(int k = 0; k < N; k++) {
            // printf("%d ", elevation[j][k]);
        }
        // printf("\n");
        free(elevation[j]);
    }
    free(elevation);
}

void findMinPostion(int *** direction, int ** elevation, int i, int j, int N) {
    // includes current position
    int dx[] = {0, 0, 1, 0, -1};
    int dy[] = {0, -1, 0, 1, 0};
    int numOfPos = 5;
    int minElev = elevation[i][j];
    for (int k = 0; k < numOfPos; k++) {
        int x = j + dx[k];
        int y = i + dy[k];
        if (x >= 0 && y >= 0 && x < N && y < N){
            int currElev = elevation[y][x];
            minElev = currElev < minElev ? currElev : minElev;
        }
    }
    bool minIsSelf = false;
    for (int k = 0; k < numOfPos; k++) {
        int x = j + dx[k];
        int y = i + dy[k];
        if (x >= 0 && y >= 0 && x < N && y < N){
            int currElev = elevation[y][x];
            if (currElev == minElev && !minIsSelf) {
                direction[i][j][k] = y * N + x;
                if (k == 0) { // current position is minElev then don't need to check neigh
                    minIsSelf = true;
                }
            } else {
                direction[i][j][k] = -1;
            }
        } else {
            direction[i][j][k] = -1;
        }
    }
    
}

int *** getDirection(int ** elevation, int N) {
    int *** direction = malloc(N * sizeof(*direction));
    for (int i = 0; i < N; i++) {
        direction[i] = malloc(N * sizeof(**direction));
        for (int j = 0; j < N; j++) {
            direction[i][j] = malloc((5) * sizeof(***direction));
            findMinPostion(direction, elevation, i, j, N);
        }
    }
    return direction;
}

void freeDirection(int *** direction, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < 5; k++) {
                printf("%d ", direction[i][j][k]);
            }
            printf("\n");
            free(direction[i][j]);
        }
        free(direction[i]);
    }
    free(direction);
}

int main(int argc, char *argv[]) {
    if (argc != 6) {
        printf("Syntax: ./simulator_seq <P> <M> <A> <N> <elevation_file>\n");
        return EXIT_FAILURE;
    }
    int M = atoi(argv[2]);
    double A = atof(argv[3]);
    int N = atoi(argv[4]);
    const char * elevationFilename = argv[5];

    int ** elevation = readElevationFile(elevationFilename, N);
    int *** direction = getDirection(elevation, N);
    double ** absorption = NULL;
    double ** totalAccumulation = NULL;
    
    struct timespec startTime, endTime;
    int currentStep = 0;

    freeElevation(elevation, N);
    freeDirection(direction, N);
    return EXIT_SUCCESS;
}