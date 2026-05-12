#include "io_utils.h"
#include <stdio.h>
#include <ctype.h>

void getAction(int *userAction)
{
    while (1)
    {
        char userAnswer[5] = {'0', 'Z', 'Z', 'Z', '\0'};
        getAnswer(userAnswer);
        convertAnswer(userAnswer, userAction, userAction + 1, userAction + 2, userAction + 3);
        if (userAnswer[0] != '0')
        {
            printf("\nThe action was written incorrectly. Please try again.\n\n\n");
            *userAction = -1;
            *(userAction + 1) = -1;
            *(userAction + 2) = -1;
            continue;
        }
        break;
    }
}

void getAnswer(char *userAnswer)
{
    char backup = *(userAnswer + 3);
    printf("\n\n%22c Keys:\n\n", ' ');
    printf("First Line  %8c%10c First Column  %8c\n", 'Q', ' ', 'I');
    printf("Second Line %8c%10c Second Column %8c\n", 'W', ' ', 'O');
    printf("Third Line  %8c%10c Third Column  %8c\n", 'E', ' ', 'P');
    printf("Up          %8c%10c Right         %8c\n", 'U', ' ', 'R');
    printf("Down        %8c%10c Left          %8c\n\n", 'D', ' ', 'L');
    printf("Tip 1: To rotate the face, type LCM in any order.\n");
    printf("Tip 2: To rotate the cube, type the corresponding movement key, preceded by the \"F\" key.\n");
    printf("Tip 3: To rotate the front face, type the corresponding movement key (\"R\" for CW or \"L\" for CCW), preceded by the \"S\" key.\n\n");
    printf("Your move: ");
    scanf("%3s", userAnswer + 1);
    clearBuffer();
    if (*(userAnswer + 3) == '\0')
    {
        *(userAnswer + 3) = backup;
    }
}

void convertAnswer(char *userAnswer, int *userLine, int *userColumn, int *userMove, int *specialMove)
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

void clearBuffer() {
    int clear;
    while ((clear = getchar()) != '\n' && clear != EOF);
}

void clearTerminal() {
    printf("\033[2J\033[H");
}