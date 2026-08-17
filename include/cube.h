#pragma once

#include <stdlib.h>
#include <stddef.h>

typedef struct Cube
{
    char faceValue[6][10];
    char seed[21];
	int facePosition[6];
    unsigned char faceComplete;
} Cube;

void initCube(Cube *cube);
void scrambleCube(Cube *cube, char *rawSeed, int size);
void rotateCube(Cube *cube, int column, int row, int move, int specialMove);
void evaluateFaces(Cube *cube);