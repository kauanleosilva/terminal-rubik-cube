#include "colors.h"

char getColor(int option)
{
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

char *convertColor(char code)
{
    char *color;
    switch (code)
    {
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