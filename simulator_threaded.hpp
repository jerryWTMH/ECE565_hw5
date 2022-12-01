#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <thread>
#include <mutex>
#include <vector>

int ** readElevationFile(const char * elevationFilename, int N);
void freeElevation(int ** elevation, int N);
int *** initializeDirectionMatrix(int N);
void getDirection(int *** direction, int ** elevation, int N, int startRow, int endRow);
void findMinPostion(int *** direction, int ** elevation, int i, int j, int N);
void freeDirection(int *** direction, int N);
double ** initializeDoubleMatrix(int N);
void freeDoubleMatrix(double ** matrix, int N);
void dropAndAbsorb(double** ground, double amount, double ** absorption, int n, int startRow, int endRow, bool stillRain);
void absorb(double ** ground, double amount, double ** absorption, int n, int startRow, int endRow);
void flow(double ** result, int *** direction, int n, bool* wet);
