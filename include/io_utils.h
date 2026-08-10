#pragma once

#include "cube.h"

typedef struct pixel
{
    char block;
    int fgFace;
    int fgColor;
    int bgFace;
    int bgColor;
} pixel;

void setupConsole();
void setPixel(pixel canvas[][62], int row, int col, char block, int fgFace, int fgColor, int bgFace, int bgColor);
void startGame(Cube *cube);
void getAction(int *userAction, Cube *cube);
void getAnswer(char *userAnswer);
void convertAnswer(char *userAnswer, int *userColumn, int *userRow, int *userMove, int *specialMove, unsigned char *flag);
void endGame();
void clearBuffer();
void clearTerminal();
int throwError(char error);