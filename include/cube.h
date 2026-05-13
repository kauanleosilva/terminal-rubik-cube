#pragma once

#define BLOCK "\u25A0"

typedef struct Cube
{
    char faceValue[6][10];
	int facePosition[6];
    unsigned char faceComplete;
} Cube;

void initCube(Cube *);
void seedGenerator(char *, int, int *);
void scrambleCube(Cube *, char *, int);
void showCube(Cube, int);
void rotateCube(Cube *, int, int, int, int);