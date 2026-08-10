#include "colors.h"

char *convertColor(char code, char flag)
{
    char *color;

    if (flag)
    {
        switch (code)
        {
        case 0:
            color = MAROON;
            break;
        case 1:
            color = BLUE;
            break;
        case 2:
            color = GREEN;
            break;
        case 3:
            color = ROSE;
            break;
        case 4:
            color = YELLOW;
            break;
        case 5:
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
        case 0:
            color = BG_MAROON;
            break;
        case 1:
            color = BG_BLUE;
            break;
        case 2:
            color = BG_GREEN;
            break;
        case 3:
            color = BG_ROSE;
            break;
        case 4:
            color = BG_YELLOW;
            break;
        case 5:
            color = BG_PURPLE;
            break;
        default:
            break;
        }
    }

    return color;
}