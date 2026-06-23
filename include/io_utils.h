#pragma once

#include "cube.h"

void setupConsole();
void startGame(Cube *cube, char *seed);
void getAction(int *userAction, Cube *cube, char *seed);
void getAnswer(char *userAnswer);
void convertAnswer(char *userAnswer, int *userColumn, int *userRow, int *userMove, int *specialMove, unsigned char *flag);
void endGame();
void clearBuffer();
void clearTerminal();
int throwError(char error);