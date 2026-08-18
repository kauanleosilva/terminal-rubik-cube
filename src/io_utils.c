#include <stdio.h>
#include <ctype.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "io_utils.h"
#include "cube.h"

static void getAnswer(char *userAnswer);
static void parseAnswer(char *userAnswer, int *userColumn, int *userRow, int *userMove, int *specialMove, unsigned char *flag);
static void drawPixel(pixel canvas[][21], int row, int col, char block, char *fgColor, char *bgColor);
static char *getVisualComponent(char code, char mode);
static void drawUpFace(pixel canvas[][21], char fgColorsCache[][9][24], char bgColorsCache[][9][24]);
static void drawFrontFace(pixel canvas[][21], char fgColorsCache[][9][24], char bgColorsCache[][9][24]);
static void drawRightFace(pixel canvas[][21], char fgColorsCache[][9][24], char bgColorsCache[][9][24]);
static void clearBuffer(void);
static void clearTerminal(void);

void setupConsole(void)
{
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    COORD bufferSize;
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    HWND hwnd = GetConsoleWindow();
    DWORD dwMode = 0;
    if (GetConsoleMode(hOut, &dwMode))
    {
        dwMode |= 0x0004;
        SetConsoleMode(hOut, dwMode);
    }
    SetConsoleOutputCP(65001);
    SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX);
    system("mode con: cols=150 lines=42");
    GetConsoleScreenBufferInfo(hOut, &csbi);
    bufferSize.X = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    bufferSize.Y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    SetConsoleScreenBufferSize(hOut, bufferSize);
#endif
}

void playSound(int frequency, int time) {
    #ifdef _WIN32
    Beep((DWORD)frequency, (DWORD)time);
    #endif
}

void startGame(Cube *cube)
{
    initCube(cube);
    while (1)
    {
        char userAnswer[22];
        int i;
        char error = 0;

        printf("Cube Seed (Max 20 characters): ");
        scanf("%21s", userAnswer);
        clearBuffer();
        for (i = 0; i < strlen(userAnswer); i++)
        {
            if (!(isprint((unsigned char)*(userAnswer + i))))
            {
                error = 1;
                break;
            }
        }
        if (strlen(userAnswer) > 20)
        {
            error = 2;
        }
        if (handleError(error))
            continue;
        strcpy(cube->seed, userAnswer);
        break;
    }
    clearTerminal();
    scrambleCube(cube, cube->seed, strlen(cube->seed));
}

void getAction(int *userAction, Cube *cube)
{
    while (1)
    {
        unsigned char flag = 0b10000000;
        char userAnswer[5] = {0, 'Z', 'Z', '\0'};

        *userAction = -1;
        *(userAction + 1) = -1;
        *(userAction + 2) = -1;
        *(userAction + 3) = -1;
        getAnswer(userAnswer);
        if (handleError(userAnswer[0]))
            continue;
        parseAnswer(userAnswer, userAction, userAction + 1, userAction + 2, userAction + 3, &flag);
        if (handleError(userAnswer[0]))
            continue;
        if (flag ^ 0b10000000)
        {
            cube->faceComplete &= 0b01111111;
        }
        clearTerminal();
        break;
    }
}

static void getAnswer(char *userAnswer)
{
    char limitCap[4];

    printf("\n\n%21c Keys:\n\n", ' ');
    printf("First Row  %8c%10c First Col  %8c\n", 'Q', ' ', 'I');
    printf("Second Row %8c%10c Second Col %8c\n", 'W', ' ', 'O');
    printf("Third Row  %8c%10c Third Col  %8c\n", 'E', ' ', 'P');
    printf("Move Up    %8c%10c Move Right %8c\n", 'U', ' ', 'R');
    printf("Move Down  %8c%10c Move Left  %8c\n\n", 'D', ' ', 'L');
    printf("Tip 1: To rotate a face, press a row/column and movement key in any order.\n");
    printf("Tip 2: To rotate the cube, press \"F\" followed by a movement key.\n");
    printf("Tip 3: To rotate the front face, press \"S\" followed by a movement key (\"R\" for CW or \"L\" for CCW).\n");
    printf("Tip 4: To end the game, press \"X\".\n\n");
    printf("Your move: ");
    scanf("%3s", limitCap);
    if (strlen(limitCap) > 2)
    {
        *userAnswer = 3;
    }
    else
    {
        strcpy(userAnswer + 1, limitCap);
    }
    clearBuffer();
}

static void parseAnswer(char *userAnswer, int *userColumn, int *userRow, int *userMove, int *specialMove, unsigned char *flag)
{
    char lOne = toupper(*(userAnswer + 1)), lTwo = toupper(*(userAnswer + 2));

    if (lOne == 'X' && lTwo == '\0')
    {
        if (lTwo == '\0')
        {
            *flag &= ~0b10000000;
        }
        else
        {
            *userAnswer = 9;
        }
    }
    else if (lOne == 'F')
    {
        *specialMove = 10;
        switch (lTwo)
        {
        case 'R':
            *userMove = 0;
            break;
        case 'U':
            *userMove = 1;
            break;
        case 'D':
            *userMove = 2;
            break;
        case 'L':
            *userMove = 3;
            break;
        default:
            *userAnswer = 7;
            break;
        }
    }
    else if (lOne == 'S')
    {
        *specialMove = 20;
        switch (lTwo)
        {
        case 'R':
            *userMove = 6;
            break;
        case 'L':
            *userMove = 4;
            break;
        default:
            *userAnswer = 8;
            break;
        }
    }
    else if (lOne == 'Q' || lOne == 'W' || lOne == 'E')
    {
        switch (lOne)
        {
        case 'Q':
            *userRow = 0;
            break;
        case 'W':
            *userRow = 1;
            break;
        case 'E':
            *userRow = 2;
            break;
        }
        switch (lTwo)
        {
        case 'R':
            *userMove = 6;
            break;
        case 'L':
            *userMove = 4;
            break;
        default:
            *userAnswer = 4;
            break;
        }
    }
    else if (lOne == 'I' || lOne == 'O' || lOne == 'P')
    {
        switch (lOne)
        {
        case 'I':
            *userColumn = 0;
            break;
        case 'O':
            *userColumn = 1;
            break;
        case 'P':
            *userColumn = 2;
            break;
        }
        switch (lTwo)
        {
        case 'U':
            *userMove = 8;
            break;
        case 'D':
            *userMove = 2;
            break;
        default:
            *userAnswer = 5;
            break;
        }
    }
    else
    {
        *userAnswer = 10;
    }
}

static void drawPixel(pixel canvas[][21], int row, int col, char block, char *fgColor, char *bgColor)
{
    canvas[row][col].block = block;
    strcpy(canvas[row][col].fgColor, fgColor);
    strcpy(canvas[row][col].bgColor, bgColor);
}

static void drawUpFace(pixel canvas[][21], char fgColorsCache[][9][24], char bgColorsCache[][9][24])
{
    int i, j, row = 0, col = 0;

    for (i = 0; i < 9; i++)
    {
        drawPixel(canvas, row, col, 'b', fgColorsCache[1][i], "null");
        for (j = 1; j < 4; j++)
        {
            drawPixel(canvas, row, col + j, 'm', fgColorsCache[1][i], bgColorsCache[1][i]);
        }
        if ((i + 1) % 3 == 0)
        {
            drawPixel(canvas, row, col + 4, 'u', fgColorsCache[1][i], bgColorsCache[2][2 - ((i - 2) / 3)]);
            row++;
            col = 0;
        }
        else
        {
            drawPixel(canvas, row, col + 4, 'u', fgColorsCache[1][i], "null");
            col += 5;
        }
    }
    for (i = 0; i < 6; i++)
    {
        for (j = 0; j < 5; j++)
        {
            drawPixel(canvas, row, col, 'b', fgColorsCache[0][i], "null");
            if (i < 3)
            {
                if (j == 4)
                {
                    drawPixel(canvas, row, col, 'u', fgColorsCache[1][i + 6], "null");
                }
                if (i == 2 && j == 4)
                {
                    drawPixel(canvas, row, col, 'u', fgColorsCache[1][8], bgColorsCache[2][0]);
                }
                else if (j >= 1 && j <= 3)
                {
                    drawPixel(canvas, row, col, 'b', fgColorsCache[0][i], bgColorsCache[1][i + 6]);
                }
            }
            else
            {
                drawPixel(canvas, row, col, 'b', fgColorsCache[0][i + 3], "null");
                if (j == 4)
                {
                    drawPixel(canvas, row, col, ' ', "null", "null");
                }
            }
            col++;
        }
        if ((i + 1) % 3 == 0)
        {
            if (i == 2)
            {
                row += 5;
            }
            else if (i == 5)
            {
                row -= 3;
            }
            col = 0;
        }
    }
}

static void drawFrontFace(pixel canvas[][21], char fgColorsCache[][9][24], char bgColorsCache[][9][24])
{
    int i, j, row = 5, col = 0;

    for (i = 0; i < 6; i++)
    {
        for (j = 0; j < 4; j++)
        {
            drawPixel(canvas, row, col + j, 'u', fgColorsCache[0][(i % 3) + (i / 3) * 6], "null");
        }
        drawPixel(canvas, row, col + 4, ' ', "null", "null");
        if ((i + 1) % 3 == 0)
        {
            if (i == 2)
            {
                row += 5;
            }
            else
            {
                row -= 6;
            }
            col = 0;
        }
        else
        {
            col += 5;
        }
    }
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (i >= 3 && i <= 5)
            {
                drawPixel(canvas, row + 1, col + j, 'm', fgColorsCache[0][i], bgColorsCache[0][i]);
            }
            drawPixel(canvas, row, col + j, 'm', fgColorsCache[0][i], bgColorsCache[0][i]);
        }
        drawPixel(canvas, row, col + 4, ' ', "null", "null");
        drawPixel(canvas, row + 1, col + 4, ' ', "null", "null");
        if ((i + 1) % 3 == 0)
        {
            if (i == 2)
            {
                row += 2;
            }
            else if (i == 5)
            {
                row += 3;
            }
            else
            {
                row = 1;
            }
            col = 0;
        }
        else
        {
            col += 5;
        }
    }
}

static void drawRightFace(pixel canvas[][21], char fgColorsCache[][9][24], char bgColorsCache[][9][24])
{
    int i, row = 4, col = 14;
    drawPixel(canvas, row, col, 'm', fgColorsCache[2][0], bgColorsCache[2][0]);
    drawPixel(canvas, row + 1, col, 'b', fgColorsCache[2][3], "null");
    drawPixel(canvas, row + 2, col, 'm', fgColorsCache[2][3], bgColorsCache[2][3]);
    drawPixel(canvas, row + 3, col, 'u', fgColorsCache[2][3], "null");
    drawPixel(canvas, row + 4, col, 'm', fgColorsCache[2][6], bgColorsCache[2][6]);
    drawPixel(canvas, row + 5, col, 'm', fgColorsCache[2][6], bgColorsCache[2][6]);
    row--;
    col++;
    for (i = 0; i < 3; i++)
    {
        drawPixel(canvas, row, col, 'm', fgColorsCache[2][i], bgColorsCache[2][i]);
        drawPixel(canvas, row, col + 1, 'm', fgColorsCache[2][i], bgColorsCache[2][i]);
        drawPixel(canvas, row + 2, col, 'm', fgColorsCache[2][i + 3], bgColorsCache[2][i + 3]);
        drawPixel(canvas, row + 2, col + 1, 'm', fgColorsCache[2][i + 3], bgColorsCache[2][i + 3]);
        drawPixel(canvas, row + 5, col, 'm', fgColorsCache[2][i + 6], bgColorsCache[2][i + 6]);
        drawPixel(canvas, row + 5, col + 1, 'm', fgColorsCache[2][i + 6], bgColorsCache[2][i + 6]);
        drawPixel(canvas, row + 1, col, 'u', fgColorsCache[2][i], "null");
        drawPixel(canvas, row + 1, col + 1, 'b', fgColorsCache[2][i + 3], "null");
        drawPixel(canvas, row + 3, col, 'm', fgColorsCache[2][i + 3], bgColorsCache[2][i + 3]);
        drawPixel(canvas, row + 3, col + 1, 'u', fgColorsCache[2][i + 3], "null");
        drawPixel(canvas, row + 4, col, 'b', fgColorsCache[2][i + 6], "null");
        drawPixel(canvas, row + 4, col + 1, 'm', fgColorsCache[2][i + 6], bgColorsCache[2][i + 6]);
        drawPixel(canvas, row + 6, col, 'u', fgColorsCache[2][i + 6], "null");
        row--;
        col += 2;
    }
}

static char *getVisualComponent(char code, char mode)
{
    if (mode == 0)
    {
        switch (code)
        {
        case 0:
            return BG_FFCOLOR;
        case 1:
            return BG_UFCOLOR;
        case 2:
            return BG_RFCOLOR;
        case 3:
            return BG_LFCOLOR;
        case 4:
            return BG_DFCOLOR;
        case 5:
            return BG_BFCOLOR;
        default:
            break;
        }
    }
    else if (mode == 1)
    {
        switch (code)
        {
        case 0:
            return FFCOLOR;
        case 1:
            return UFCOLOR;
        case 2:
            return RFCOLOR;
        case 3:
            return LFCOLOR;
        case 4:
            return DFCOLOR;
        case 5:
            return BFCOLOR;
        default:
            break;
        }
    }
    else if (mode == 2)
    {
        switch (code)
        {
        case 'u':
            return UH_BLOCK;
            break;
        case 'm':
            return BLOCK;
            break;
        case 'b':
            return BH_BLOCK;
            break;
        default:
            break;
        }
    }

    return "";
}

void renderCube(Cube cube, char *buffer, size_t bufferSize)
{
    int i, j, bufferIndex = 0;
    char fgColorsCache[3][9][24];
    char bgColorsCache[3][9][24];
    pixel canvas[11][21];

    for (i = 0; i < 11; i++)
    {
        for (j = 0; j < 21; j++)
        {
            canvas[i][j].block = '\0';
            strcpy(canvas[i][j].fgColor, "null");
            strcpy(canvas[i][j].bgColor, "null");
        }
    }
    memset(buffer, '\0', bufferSize);
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 9; j++)
        {
            strcpy(fgColorsCache[i][j], getVisualComponent(cube.faceValue[cube.facePosition[i]][j], 1));
            strcpy(bgColorsCache[i][j], getVisualComponent(cube.faceValue[cube.facePosition[i]][j], 0));
        }
    }
    drawUpFace(canvas, fgColorsCache, bgColorsCache);
    drawFrontFace(canvas, fgColorsCache, bgColorsCache);
    drawRightFace(canvas, fgColorsCache, bgColorsCache);
    bufferIndex += sprintf(buffer + bufferIndex, "%s", "\x1b[2;46H");
    for (i = 0; i < 11; i++)
    {
        for (j = 0; j < 21; j++)
        {
            if (canvas[i][j].block != '\0')
            {
                if (canvas[i][j].block == ' ')
                {
                    bufferIndex += sprintf(buffer + bufferIndex, "%c", ' ');
                }
                else if (canvas[i][j].block == 'n')
                {
                    bufferIndex += sprintf(buffer + bufferIndex, "%s", N_BLOCK);
                }
                else
                {
                    if (strcmp(canvas[i][j].bgColor, "null") != 0)
                    {
                        bufferIndex += sprintf(buffer + bufferIndex, "%s", canvas[i][j].bgColor);
                    }
                    if (strcmp(canvas[i][j].fgColor, "null") != 0)
                    {
                        bufferIndex += sprintf(buffer + bufferIndex, "%s", canvas[i][j].fgColor);
                    }
                    bufferIndex += sprintf(buffer + bufferIndex, "%s", getVisualComponent(canvas[i][j].block, 2));
                    bufferIndex += sprintf(buffer + bufferIndex, "%s", RESET);
                }
            }
        }
        bufferIndex += sprintf(buffer + bufferIndex, "\x1b[%d;%dH", i + 3, (i > 2) ? 40 : 44 - (i * 2));
    }
    fputs(buffer, stdout);
}

static void clearBuffer(void)
{
    int clear;

    while ((clear = getchar()) != '\n' && clear != EOF)
        ;
}

static void clearTerminal(void)
{
    printf("\x1b[2J\x1b[3J\x1b[H");
}

int handleError(char error)
{
    switch (error)
    {
    case 0:
        return 0;
    case 1:
        clearTerminal();
        printf("Invalid characters detected! Please use standard letters, numbers, or symbols only.\n\n");
        return 1;
    case 2:
        clearTerminal();
        printf("Seed too long! Maximum allowed is 20 characters.\n\n");
        return 1;
    case 3:
        clearTerminal();
        printf("Action too long! Maximum allowed is 2 characters.\n\n");
        return 1;
    case 4:
        clearTerminal();
        printf("Invalid action! Rows can only move right or left.\n\n");
        return 1;
    case 5:
        clearTerminal();
        printf("Invalid action! Columns can only move up or down.\n\n");
        return 1;
    case 6:
        clearTerminal();
        printf("Invalid action! Please do not use duplicate or invalid characters.\n\n");
        return 1;
    case 7:
        clearTerminal();
        printf("Invalid action! The entire cube can only be rotated up, down, left, or right.\n\n");
        return 1;
    case 8:
        clearTerminal();
        printf("The front face can only be rotated right or left.\n\n");
        return 1;
    case 9:
        clearTerminal();
        printf("Invalid action! Press only the X key to close, do not combine it with other keys.\n\n");
        return 1;
    case 10:
        clearTerminal();
        printf("Invalid action! Please read the tips and enter a valid command.\n\n");
        return 1;
    case 11:
        clearTerminal();
        printf("Insufficient RAM available. (Press Enter to close)");
        getchar();
        exit(1);
    default:
        return 0;
    }
}

void endGame(void)
{
    clearTerminal();
    printf("\nGoodbye! (Press Enter to close)\n");
    getchar();
}