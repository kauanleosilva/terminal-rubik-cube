#include <stdio.h>
#include <ctype.h>

#define MAROON "\x1b[38;5;88m"
#define BLUE "\x1b[34m"
#define GREEN "\x1b[32m"
#define ROSE "\x1b[38;5;201m"
#define YELLOW "\x1b[33m"
#define PURPLE "\x1b[38;5;129m"
#define RESET "\x1b[0m"
#define BLOCK "\u25A0"

#ifdef _WIN32
    #include <windows.h>
#endif

typedef struct Cube
{
    char faceValue[6][10];
	int facePosition[6];
    unsigned char solution;
} Cube;

void initCube(Cube *);
void showCube(Cube, int);
void rotateCube(Cube *, int, int, int);
char getColor(int);
char * convertColor(char);
void getAction(int *);
void getAnswer(char *);
int classifyAnswer(char);
void convertAnswer(char *, int *, int *, int *, int *);

int main()
{
    #ifdef _WIN32
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
            dwMode |= 0x0004;
            SetConsoleMode(hOut, dwMode);
        }
        SetConsoleOutputCP(65001);
    #endif
    int userAction[4] = {-1, -1, -1, -1};
    Cube cube;
    cube.solution = 0b00000000;

    initCube(&cube);
    while (1)
    {
        getAction(userAction);
        // rotateCube(*(userAction), *(userAction + 1), *(userAction + 2));
        showCube(cube, 5);
        break;
    };

    return 0;
}

void initCube(Cube *cube) {
    int f,b;
    for(f = 0; f < 6; f++) {
        for(b = 0; b < 9; b++) {
            cube->faceValue[f][b] = getColor(f);
        }
    };
	for(f = 0; f < 6; f++) {
		cube->facePosition[f] = f;
		cube->faceValue[f][9] = '\0';
	}
}

void showCube(Cube cube, int face) {
	int b, count=0;
	for(b = 0; b < 9; b++) {
		printf(" %s%s%s", convertColor(cube.faceValue[face][b]), BLOCK, RESET);
		count++;
		if(count%3==0)
		{
			printf("\n");
		}
	}
}

char getColor(int option) {
    char color;
    switch (option)
    {
    case 0:
        color = 'M';
        break;
    case 1:
        color = 'B';
        break;
    case 2:
        color = 'G';
        break;
    case 3:
        color = 'R';
        break;
    case 4:
        color = 'Y';
        break;
    case 5:
        color = 'P';
        break;
    }
    return color;
}

char * convertColor(char code) {
	char *color;
	switch(code) {
	case 'M':
		color = MAROON;
		break;
	case 'B':
		color = BLUE;
		break;
	case 'G':
		color = GREEN;
		break;
	case 'R':
		color = ROSE;
		break;
	case 'Y':
		color = YELLOW;
		break;
	case 'P':
		color = PURPLE;
		break;
	default:
		break;
	}
	return color;
}

void getAction(int *userAction)
{
    char userAnswer[5];

    while (1)
    {
        userAnswer[0] = '0';
        getAnswer(userAnswer);
        convertAnswer(userAnswer, userAction, userAction + 1, userAction + 2, userAction + 3);
        printf("%d %d %d", *userAction, *(userAction + 1), *(userAction + 2));
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
    printf("Third Line  %8c%10c Third Column  %8c\n", 'E', ' ', 'P');
    printf("Up          %8c%10c Right         %8c\n", 'U', ' ', 'R');
    printf("Down        %8c%10c Left          %8c\n\n", 'D', ' ', 'L');
    printf("Tip 1: To rotate the face, type LCM in any order.\n");
    printf("Tip 2: To rotate the cube, press only the corresponding movement key, preceded by the \"F\" key.\n");
    printf("Tip 3: To rotate the front face, press only the corresponding movement key (\"R\" for CW or \"L\" for CCW), preceded by the \"S\" key.\n\n");
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

void convertAnswer(char *userAnswer, int *userLine, int *userColumn, int *userMove, int *specialMove)
{
    int answerGroup;
    int validate[4] = {0};
    int i;
    for (i = 1; i < 4; i++)
    {
        char letter = toupper(*(userAnswer + i));
        if(*(userAnswer) == 'F' && )
		{
			
		} else if(*(userAnswer) == 'S')
		{
			
		}
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
