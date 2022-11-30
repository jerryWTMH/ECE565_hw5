#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include "simulator_sequential.h"
#include <pthread.h>

void readElevationFile(const char * elevationFilename, int ** elevation, int N) {
    FILE * elevationFile = fopen(elevationFilename, "r");
    if (elevationFile == NULL) {
        perror("Could not open elevation_file!");
        exit(EXIT_FAILURE);
    }
    char * curr = NULL;
    int * row = NULL;
    size_t sz;
    int i = 0;
    while (getline(&curr, &sz, elevationFile) >= 0) {
        //elevation = realloc(elevation, (i + 1) * sizeof(*elevation));
        // row = malloc(N * sizeof(*row));
        printf("%d\n", atoi(curr));
        free(curr);
        curr = NULL;
        i++;
    }
    //free(elevation);
    if (fclose(elevationFile) != 0) {
        perror("Failed to close elevation_file!");
        exit(EXIT_FAILURE);
    }

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
                double dropToFlow = result[i][j] >= 1.0 ? 1.0 : result[i][j];
                double fractionDrop = dropToFlow / modularSize;
                for(int k = 0; k < modularSize; ++k){
                    int currX = modularPos[k]/n;
                    int currY = modularPos[k]%n;
                    flowMatrix[currX][currY] += fractionDrop;
                }
                flowMatrix[i][j] -= dropToFlow;
            }else{
                result[i][j] = 0.0;
            }
        }
    }
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

    int ** elevation = NULL;
    readElevationFile(elevationFilename, elevation, N);
    int *** direction = NULL;
    double ** absorption = NULL;
    double ** totalAccumulation = NULL;
    
    struct timespec startTime, endTime;
    int currentStep = 0;

    //TODO TEST
    //float elapsed_ns = calc_time(start_time, end_time);
    float elapsed_ns = 0.0;
    printf("Rainfall simulation took %d time steps to complete.\n", currentStep); 
    printf("Runtime = %d seconds\n", elapsed_ns / 1000000000); 
    printf("\n");
    printf("The following grid shows the number of raindrops absorbed at each point:\n");
    size_t sizeAbsorption = sizeof(absorption) / sizeof(absorption[0]);
    size_t sizeSubAbsorption = sizeof(absorption[0]) / sizeof(absorption[0][0]);
    for(int i = 0; i < sizeAbsorption; i++){
        for(int j = 0; j < sizeSubAbsorption; j++){
            printf("%8d", absorption[i][j]);
        }
        printf("\n");
    }
    return EXIT_SUCCESS;
}