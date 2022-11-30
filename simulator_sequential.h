#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>

int ** readElevationFile(const char * elevationFilename, int N);
void freeElevation(int ** elevation, int N);
int *** getDirection(int ** elevationMatrix, int N);
void findMinPostion(int *** direction, int ** elevation, int i, int j, int N);
void freeDirection(int *** direction, int N);
void drop(double** ground);
void absorb(double ** ground, double amount, double ** absorption);
bool flow(double ** result, int *** direction);
void freeFlowMatrix(double ** flowMatrix, size_t n);

































