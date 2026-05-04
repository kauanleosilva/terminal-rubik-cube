#include <stdio.h>
#include <ctype.h>

#define PURPLE \x1b[48;5;129m
#define BLUE \x1b[44m
#define BURGUNDY \x1b[48;5;88m
#define YELLOW \x1b[43m
#define PINK \x1b[48;5;201m
#define GREEN \x1b[42m
#define RESET \x1b[0m

#if



#endif


char initCube(char *);
void getAction(int *);
void rotateCube(int, int, int);
void getAnswer(char *);
int classifyAnswer(char);
void convertAnswer(char *, int *, int *, int *);

typedef struct
{
    char face[6][9];
} Cube;

int main()
{
    int userAction[3] = {-1, -1, -1};
    Cube cube;
    unsigned char gameAnswer = 0b00000000;

    initCube(&cube);
    while (1)
    {
        getAction(userAction);
        rotateCube(*(userAction), *(userAction + 1), *(userAction + 2));
        break;
    };

    return 0;
};


char initCube(char *cube) {
    int f,b
    for()


};

void getAction(int *userAction)
{
    char userAnswer[5];

    while (1)
    {
        userAnswer[0] = '0';
        getAnswer(userAnswer);
        convertAnswer(userAnswer, userAction, userAction + 1, userAction + 2);
        printf("\n 1. %d %d %d \n", *userAction, *(userAction + 1), *(userAction + 2));
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
    printf("%22c Keys:\n\n", ' ');
    printf("First Line  %8c%10c First Column  %8c\n", 'Q', ' ', 'I');
    printf("Second Line %8c%10c Second Column %8c\n", 'W', ' ', 'O');
    printf("Third Line  %8c%10c Third Column  %8c\n", 'E', ' ', 'E');
    printf("Up          %8c%10c Right         %8c\n", 'U', ' ', 'R');
    printf("Down        %8c%10c Left          %8c\n\n", 'D', ' ', 'L');
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
    int i;
    for (i = 1; i < 4; i++)
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
        };
    }
}