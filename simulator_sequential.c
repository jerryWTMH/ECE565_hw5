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
            printf("%d ", elevation[j][k]);
        }
        printf("\n");
        free(elevation[j]);
    }
    free(elevation);
}

//TODO TEST
void drop(double** ground){
    size_t m = sizeof(ground) / sizeof(ground[0]);
    size_t n = sizeof(ground[0]) / sizeof(ground[0][0]);
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            ground[i][j] += 1.0;
        }
    }
}

//TODO TEST
void absorb(double ** ground, double amount, double ** absorption){
    size_t m = sizeof(ground) / sizeof(ground[0]);
    size_t n = sizeof(ground[0]) / sizeof(ground[0][0]);
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            absorption[i][j] += ground[i][j] >= amount ? amount : ground[i][j];
            ground[i][j] -= amount;
        }
    }
}

//TODO TEST
bool flow(double ** result, int *** direction){
    size_t n = sizeof(result) / sizeof(result[0]);
    bool wet = false;
    double ** flowMatrix = malloc(n * sizeof(double *));
    for(int i = 0; i < n; i++){
        double * array = malloc(n * sizeof(double));
        for(int j = 0; j < n; j++){
            array[j] = 0.0;
        }
        flowMatrix[i] = array;
    }

    for(int i =0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(result[i][j]>0){
                wet = true;
                direction[i][j];
                int * modularPos = direction[i][j];
                size_t modularSize = sizeof(modularPos) / sizeof(modularPos[0]);
                int validSize = 0;
                for(int p = 0; p < modularSize; p++){
                    if(modularPos[p] >= 0){
                        validSize++;
                    }
                } 
                double dropToFlow = result[i][j] >= 1.0 ? 1.0 : result[i][j];
                double fractionDrop = dropToFlow / validSize;
                for(int k = 0; k < validSize; ++k){
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
    freeFlowMatrix(flowMatrix, n);
    free(flowMatrix);
    return wet;
}

void freeFlowMatrix(double ** flowMatrix, size_t n){
    for(int i =0; i < n; i++){
        free(flowMatrix[i]);
    }
}
// int *** getDirection(int ** elevationMatrix, int N) {

// }

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
    int *** direction = NULL;
    double ** absorption = NULL;
    double ** totalAccumulation = NULL;
    
    struct timespec startTime, endTime;
    int currentStep = 0;

    //TODO TEST
    //float elapsed_ns = calc_time(start_time, end_time);
    float elapsed_ns = 0.0;
    printf("Rainfall simulation took %d time steps to complete.\n", currentStep); 
    printf("Runtime = %f seconds\n", elapsed_ns / 1000000000); 
    printf("\n");
    printf("The following grid shows the number of raindrops absorbed at each point:\n");
    size_t sizeAbsorption = sizeof(absorption) / sizeof(absorption[0]);
    size_t sizeSubAbsorption = sizeof(absorption[0]) / sizeof(absorption[0][0]);
    for(int i = 0; i < sizeAbsorption; i++){
        for(int j = 0; j < sizeSubAbsorption; j++){
            printf("%8f", absorption[i][j]);
        }
        printf("\n");
    }
    freeElevation(elevation, N);
    return EXIT_SUCCESS;
}