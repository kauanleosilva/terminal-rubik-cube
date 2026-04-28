#include <stdio.h>
#include <ctype.h>

void getAction();
void getAnswer(char *);
void convertAnswer(char *, int *, int *, int *);
int classifyAnswer(char);


int main()
{
    char userAnswer[5];
    int userLine, userColumn, userMove;
    // unsigned char gameAnswer = 0b00000000;
    while (1)
    {
        while (1)
        {

            userAnswer[0] = '0';
            getAnswer(userAnswer);
            convertAnswer(userAnswer, &userLine, &userColumn, &userMove);
            if (userAnswer[0] != '0')
            { 
                printf("\nThe action was written incorrectly. Please try again.\n\n\n");
                continue;
            }
            break;
        }
        printf("%s", userAnswer);
        break;
    };

    return 0;
};

void getAnswer(char *userAnswer)
{

    printf("%15c Keys:\n\n", ' ');
    printf("First Line  %8c  %10c First Column  %8c\n", 'Q', ' ', 'I');
    printf("Second Line %8c  %10c Second Column %8c\n", 'W', ' ', 'O');
    printf("Third Line  %8c  %10c Third Column  %8c\n", 'E', ' ', 'E');
    printf("Up          %8c  %10c Right         %8c\n", 'U', ' ', 'R');
    printf("Down        %8c  %10c Left          %8c\n\n", 'D', ' ', 'L');

    printf("Enter your move (You can type in any order. Do not use spaces)\n");
    printf("Your move: ");
    scanf("%3s", userAnswer + 1);
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

void convertAnswer(char *userAnswer, int *userLine, int *userColumn, int *userMove)
{
    int answerGroup;
    int validate[4] = {0};

    for (int i = 1; i < 4; i++)
    {
        char letter = toupper(*(userAnswer + i));
        
        answerGroup = classifyAnswer(letter);
        if (answerGroup == 0 || validate[answerGroup] == 1)
        {
            *userAnswer = 1;
            continue;
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
        default:
            *userAnswer = '1';
            continue;
        };
    }
}