#pragma once

#include <stddef.h>
#include "cube.h"

#define BLOCK "\u25A0"
#define UH_BLOCK "\u2580"
#define BH_BLOCK "\u2584"
#define N_BLOCK "\u200B"
#define FFCOLOR "\x1b[38;5;15m"
#define UFCOLOR "\x1b[38;5;21m"
#define RFCOLOR "\x1b[38;5;196m"
#define LFCOLOR "\x1b[38;5;208m"
#define DFCOLOR "\x1b[38;5;34m"
#define BFCOLOR "\x1b[38;5;226m"
#define BG_FFCOLOR "\x1b[48;5;15m"
#define BG_UFCOLOR "\x1b[48;5;21m"
#define BG_RFCOLOR "\x1b[48;5;196m"
#define BG_LFCOLOR "\x1b[48;5;208m"
#define BG_DFCOLOR "\x1b[48;5;34m"
#define BG_BFCOLOR "\x1b[48;5;226m"
#define RESET "\x1b[0m"

typedef struct pixel
{
    char block;
    int fgFace;
    int fgColor;
    int bgFace;
    int bgColor;
} pixel;

void setupConsole(void);
void startGame(Cube *cube);
void getAction(int *userAction, Cube *cube);
void renderCube(Cube cube, char *buffer, size_t bufferSize);
int handleError(char error);
void endGame(void);