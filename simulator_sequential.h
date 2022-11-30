#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>

int * getModularPos(int n, int i, int j, int ** elevationMatrix);
int ** readElevationFile(const char * elevationFilename, int N);
int *** getDirection(int ** elevationMatrix);
void drop(double** ground, size_t n);
void absorb(double ** ground, double amount, double ** absorption, size_t n);
bool flow(double ** result, int *** direction, size_t n);
void freeFlowMatrix(double ** flowMatrix, size_t n);

































