#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>

int * getModularPos(int n, int i, int j, vector<vector<int> > & elevationMatrix);
int *** getDirection(int ** elevationMatrix);
void drop(float** ground);
void absorb(float ** ground, float amount, float ** absorption);
bool flow(float ** result, int *** direction);

































