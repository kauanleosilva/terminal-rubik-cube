#pragma once

#define BLOCK "\u25A0"

typedef struct Cube
{
    char faceValue[6][10];
	int facePosition[6];
    unsigned char solution;
} Cube;

void initCube(Cube *);
void showCube(Cube, int);
void rotateCube(Cube *, int, int, int, int);