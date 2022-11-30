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

    return EXIT_SUCCESS;
}