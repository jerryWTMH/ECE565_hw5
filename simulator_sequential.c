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
                // printf("%d ", direction[i][j][k]);
            }
            // printf("\n");
            free(direction[i][j]);
        }
        free(direction[i]);
    }
    free(direction);
}

//TODO TEST
void drop(double** ground, int n){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            ground[i][j] += 1.0;
        }
    }
}

//TODO TEST
void absorb(double ** ground, double amount, double ** absorption, int n){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            absorption[i][j] += ground[i][j] >= amount ? amount : ground[i][j];
            ground[i][j] -= amount;
        }
    }
}

//TODO TEST
bool flow(double ** result, int *** direction, int n){
    bool wet = false;
    double ** flowMatrix = initializeDoubleMatrix(n);

    for(int i =0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(result[i][j]>0){
                wet = true;
                int * modularPos = direction[i][j];
                int modularPosSize = 5;
                int validSize = 0;
                for(int p = 0; p < modularPosSize; p++){
                    if(modularPos[p] >= 0){
                        validSize++;
                    }
                } 
                double dropToFlow = result[i][j] >= 1.0 ? 1.0 : result[i][j];
                double fractionDrop = dropToFlow / validSize;
                for(int k = 0; k < modularPosSize; ++k){
                    if(modularPos[k] >= 0){
                        int currX = modularPos[k]/n;
                        int currY = modularPos[k]%n;
                        flowMatrix[currX][currY] += fractionDrop;
                    }
                }
                flowMatrix[i][j] -= dropToFlow;
            }else{
                result[i][j] = 0.0;
            }
        }
    }

    for(int i=0;i<n;++i){
        for(int j=0;j<n;++j){
        result[i][j] += flowMatrix[i][j];
        }
    }

    freeFlowMatrix(flowMatrix, n);
    free(flowMatrix);
    return wet;
}

void freeFlowMatrix(double ** flowMatrix, int n){
    for(int i =0; i < n; i++){
        free(flowMatrix[i]);
    }
}

double ** initializeDoubleMatrix(int N) {
    double ** matrix = malloc(N * sizeof(*matrix));
    for (int i = 0; i < N; i++) {
        matrix[i] = malloc(N * sizeof(**matrix));
        for (int j = 0; j < N; j++) {
            matrix[i][j] = 0.0;
        }
    }
    return matrix;
}

void freeDoubleMatrix(double ** matrix, int N) {
    for (int i = 0; i < N; i++) {
        free(matrix[i]);
    }
    free(matrix);
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
    double ** absorption = initializeDoubleMatrix(N);
    double ** totalAccumulation = initializeDoubleMatrix(N);
    
    struct timespec startTime, endTime;
    int currentStep = 0;

    // TODO TEST
    float elapsed_ns = calc_time(startTime, endTime);

    bool keepSimulate = true;
    while(keepSimulate){
      if(currentStep < M){
        drop(totalAccumulation, N);
      }
      absorb(totalAccumulation, A, absorption, N);
      keepSimulate = flow(totalAccumulation, direction, N);
      currentStep++;
    }
    
    printf("Rainfall simulation took %d time steps to complete.\n", currentStep); 
    printf("Runtime = %f seconds\n", elapsed_ns / 1000000000); 
    printf("\n");
    printf("The following grid shows the number of raindrops absorbed at each point:\n");
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            printf("%8f", absorption[i][j]);
        }
        printf("\n");
    }
    freeElevation(elevation, N);
    freeDirection(direction, N);
    freeDoubleMatrix(absorption, N);
    freeDoubleMatrix(totalAccumulation, N);

    return EXIT_SUCCESS;
}