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
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    if (GetConsoleMode(hOut, &dwMode))
    {
        dwMode |= 0x0004;
        SetConsoleMode(hOut, dwMode);
    }
    SetConsoleOutputCP(65001);
#endif
}

void startGame(Cube *cube)
{
    char seed[21];
    initCube(cube);
    while (1)
    {
        char userAnswer[22];
        int i, valid = 0;
        printf("Cube Seed (Max 20 characters): ");
        scanf("%21s", userAnswer);
        clearBuffer();
        for (i = 0; i < strlen(userAnswer); i++)
        {
            if (!(isprint((unsigned char)*(userAnswer + i))))
            {
                valid = -1;
                break;
            }
        }
        if (valid == -1)
        {
            printf("Invalid characters detected! Please use standard letters, numbers, or symbols only.\n\n");
            continue;
        } else if (strlen(userAnswer) > 20)
        {
            printf("Seed to long! Maximum allowed is 20 characters.\n\n");
            continue;
        }
        strcpy(seed, userAnswer);
        break;
    }
    scrambleCube(cube, seed, strlen(seed));
}

void getAction(int *userAction, Cube *cube, int face)
{
    clearTerminal();
    while (1)
    {
        unsigned char flag = 0b10000000;
        char userAnswer[5] = {'0', 'Z', 'Z', 'Z', '\0'};
        showCube(*cube, face);
        getAnswer(userAnswer);
        convertAnswer(userAnswer, userAction, userAction + 1, userAction + 2, userAction + 3, &flag);
        if (userAnswer[0] != '0')
        {
            clearTerminal();
            printf("\nThe action was written incorrectly. Please try again.\n\n\n");
            *userAction = -1;
            *(userAction + 1) = -1;
            *(userAction + 2) = -1;
            continue;
        }
        else if (flag ^ 0b10000000)
        {
            cube->faceComplete &= 0b01111111;
        }
        break;
    }
}

void getAnswer(char *userAnswer)
{
    char backup = *(userAnswer + 3);
    printf("\n\n%21c Keys:\n\n", ' ');
    printf("First Row  %8c%10c First Col  %8c\n", 'Q', ' ', 'I');
    printf("Second Row %8c%10c Second Col %8c\n", 'W', ' ', 'O');
    printf("Third Row  %8c%10c Third Col  %8c\n", 'E', ' ', 'P');
    printf("Move Up    %8c%10c Move Right %8c\n", 'U', ' ', 'R');
    printf("Move Down  %8c%10c Move Left  %8c\n\n", 'D', ' ', 'L');
    printf("Tip 1: To rotate a face, press a row, column and movement key in any order.\n");
    printf("Tip 2: To rotate the cube, press \"F\" followed by a movement key.\n");
    printf("Tip 3: To rotate the front face, press \"S\" followed by a movement key (\"R\" for CW or \"L\" for CCW).\n");
    printf("Tip 4: To end the game, press \"X\".\n\n");
    printf("Your move: ");
    scanf("%3s", userAnswer + 1);
    clearBuffer();
    if (*(userAnswer + 3) == '\0')
    {
        *(userAnswer + 3) = backup;
    }
    if (*(userAnswer + 2) == '\0')
    {
        *(userAnswer + 2) = backup;
    }
}

void convertAnswer(char *userAnswer, int *userLine, int *userColumn, int *userMove, int *specialMove, unsigned char *flag)
{
    char lOne = toupper(*(userAnswer + 1)), lTwo = toupper(*(userAnswer + 2)), lThree = *(userAnswer + 3);

    if (lOne == 'F' && (lTwo == 'R' || lTwo == 'U' || lTwo == 'D' || lTwo == 'L') && lThree == 'Z')
    {
        switch (lTwo)
        {
        case 'R':
            *specialMove = 16;
            break;
        case 'U':
            *specialMove = 18;
            break;
        case 'D':
            *specialMove = 12;
            break;
        case 'L':
            *specialMove = 14;
            break;
        }
    }
    else if (lOne == 'S' && (lTwo == 'R' || lTwo == 'L') && lThree == 'Z')
    {
        switch (lTwo)
        {
        case 'R':
            *specialMove = 26;
            break;
        case 'L':
            *specialMove = 24;
            break;
        }
    }
    else
    {
        int answerGroup, validate[4] = {0}, i;
        for (i = 1; i < 4; i++)
        {
            if (lOne == 'X' && lTwo == 'Z' && lThree == 'Z')
            {
                *flag &= 0b00000000;
                break;
            }
            char letter = toupper(*(userAnswer + i));
            answerGroup = classifyAnswer(letter);

            if (answerGroup == 0 || validate[answerGroup] == 1)
            {
                *userAnswer = 1;
                break;
            }
            else
            {
                validate[answerGroup] = 1;
            }
            switch (letter)
            {
            case 'Q':
                *userLine = 0;
                break;
            case 'W':
                *userLine = 1;
                break;
            case 'E':
                *userLine = 2;
                break;
            case 'I':
                *userColumn = 0;
                break;
            case 'O':
                *userColumn = 1;
                break;
            case 'P':
                *userColumn = 2;
                break;
            case 'U':
                *userMove = 8;
                break;
            case 'D':
                *userMove = 2;
                break;
            case 'L':
                *userMove = 4;
                break;
            case 'R':
                *userMove = 6;
                break;
            };
        }
    }
}

int classifyAnswer(char userAnswer)
{
    switch (userAnswer)
    {
    case 'Q':
        return 1;
    case 'W':
        return 1;
    case 'E':
        return 1;
    case 'I':
        return 2;
    case 'O':
        return 2;
    case 'P':
        return 2;
    case 'U':
        return 3;
    case 'D':
        return 3;
    case 'L':
        return 3;
    case 'R':
        return 3;
    default:
        return 0;
    }
}

void endGame()
{
    clearTerminal();
    printf("\nGoodbye! (Press Enter to close)\n");
    clearBuffer();
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