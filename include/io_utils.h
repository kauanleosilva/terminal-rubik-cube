#pragma once
#include "cube.h"

void setupConsole();
void startGame(Cube *cube);
void getAction(int *, Cube *, int);
void getAnswer(char *);
int classifyAnswer(char);
void convertAnswer(char *, int *, int *, int *, int *, unsigned char *);
void endGame();
void clearBuffer();
void clearTerminal();