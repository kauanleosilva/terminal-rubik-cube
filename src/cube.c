#include "cube.h"
#include "colors.h"
#include "stdio.h"

void initCube(Cube *cube)
{
    int f, b;
    for (f = 0; f < 6; f++)
    {
        for (b = 0; b < 9; b++)
        {
            cube->faceValue[f][b] = getColor(f);
        }
    };
    for (f = 0; f < 6; f++)
    {
        cube->facePosition[f] = f;
        cube->faceValue[f][9] = '\0';
    }
}

void showCube(Cube cube, int face)
{
    int b, count = 0;
    printf("Cube: \n");
    printf("%22c", ' ');
    for (b = 0; b < 9; b++)
    {
        printf(" %s%s%s", convertColor(cube.faceValue[face][b]), BLOCK, RESET);
        count++;
        if (count % 3 == 0)
        {
            printf("\n");
            if (count != 9)
            {
                printf("%22c", ' ');
            }
        }
    }
}