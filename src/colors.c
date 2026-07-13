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

char *convertColor(char code, char flag)
{
    char *color;

    if (flag)
    {
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
    }
    else
    {
        switch (code)
        {
        case 'M':
            color = BG_MAROON;
            break;
        case 'B':
            color = BG_BLUE;
            break;
        case 'G':
            color = BG_GREEN;
            break;
        case 'R':
            color = BG_ROSE;
            break;
        case 'Y':
            color = BG_YELLOW;
            break;
        case 'P':
            color = BG_PURPLE;
            break;
        default:
            break;
        }
    }

    return color;
}