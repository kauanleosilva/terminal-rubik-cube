#pragma once

#include <stdlib.h>

#define BLOCK "\u25A0"
#define UH_BLOCK "\u2580"
#define BH_BLOCK "\u2584"
#define N_BLOCK "\u200B"

typedef struct Cube
{
    char faceValue[6][10];
    char seed[21];
	int facePosition[6];
    unsigned char faceComplete;
} Cube;

void initCube(Cube *);
void seedGenerator(char *, int, int *);
void scrambleCube(Cube *, char *, int);
void showCube(Cube, char *, size_t bufferSize);
void temporaryShowCube(Cube cube, char *buffer, size_t bufferSize);
char *convertBlock(char code);
void rotateCube(Cube *, int, int, int, int);