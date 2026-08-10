#include "io_utils.h"
#include "cube.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif

void setupConsole()
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

void setPixel(pixel canvas[][62], int row, int col, char block, int fgFace, int fgColor, int bgFace, int bgColor)
{
    canvas[row][col].block = block;
    canvas[row][col].fgFace = fgFace;
    canvas[row][col].fgColor = fgColor;
    canvas[row][col].bgFace = bgFace;
    canvas[row][col].bgColor = bgColor;
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
        if (throwError(error))
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
        if (throwError(userAnswer[0]))
            continue;
        convertAnswer(userAnswer, userAction, userAction + 1, userAction + 2, userAction + 3, &flag);
        if (throwError(userAnswer[0]))
            continue;
        if (flag ^ 0b10000000)
        {
            cube->faceComplete &= 0b01111111;
        }
        clearTerminal();
        break;
    }
}

void getAnswer(char *userAnswer)
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

void convertAnswer(char *userAnswer, int *userColumn, int *userRow, int *userMove, int *specialMove, unsigned char *flag)
{
    char lOne = toupper(*(userAnswer + 1)), lTwo = toupper(*(userAnswer + 2));

    if (lOne == 'X' && lTwo == '\0')
    {
        if (lTwo == '\0')
        {
            *flag &= 0b00000000;
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

int throwError(char error)
{
    switch ((int)error)
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
        return 1;
    default:
        return 0;
    }
}

void endGame()
{
    clearTerminal();
    printf("\nGoodbye! (Press Enter to close)\n");
    getchar();
}

void clearBuffer()
{
    int clear;

    while ((clear = getchar()) != '\n' && clear != EOF)
        ;
}

void clearTerminal()
{
    printf("\x1b[2J\x1b[3J\x1b[H");
}