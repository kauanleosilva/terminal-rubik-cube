#include "cube.h"
#include "colors.h"
#include "stdio.h"
#include <ctype.h>
#include <stdint.h>

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
        // position 0: front
        // position 1: top
        // position 2: right
        // position 3: left
        // position 4: bottom
        // position 5: back
        cube->faceValue[f][9] = '\0';
    }
}

void seedGenerator(char *rawSeed, int size, int *actionCode)
{
    int i, lastAction = -1, action = -1;
    uint32_t seed;
    for (i = 0; i < size; i++)
    {
        uint32_t asciiCode = (uint32_t)*(rawSeed + i);
        seed ^= (asciiCode * asciiCode * asciiCode) * (i + 1);
        seed = (seed << 8) | (seed >> 24);
    }
    for (i = 0; i < 30; i++)
    {
        while (1)
        {
            do
            {
                seed ^= seed * ((i + 1) * 20);
                seed = (seed << 17) | (seed >> 15);
            } while (seed >= 4294967292U);
            action = seed % 14;
            printf("%u\n", seed);
            if (lastAction == -1 || lastAction + action != 13)
            {
                *(actionCode + i) = action;
                lastAction = action;
                break;
            }
        }
    }
}

void scrambleCube(Cube *cube, char *rawSeed, int size)
{
    int actionCode[30], i;
    seedGenerator(rawSeed, size, actionCode);
    for (i = 0; i < 30; i++)
    {
        switch (actionCode[i])
        {
        case 0:
            printf(".0");
            // rotateCube(cube, 0, -1, 8, -1);
            break;
        case 1:
            printf(".1");
            // rotateCube(cube, 1, -1, 8, -1);
            break;
        case 2:
            printf(".2");
            // rotateCube(cube, 2, -1, 8, -1);
            break;
        case 3:
            printf(".3");
            // rotateCube(cube, -1, 0, 6, -1);
            break;
        case 4:
            printf(".4");
            // rotateCube(cube, -1, 1, 6, -1);
            break;
        case 5:
            printf(".5");
            // rotateCube(cube, -1, 2, 6, -1);
            break;
        case 6:
            printf(".6");
            // rotateCube(cube, -1, -1, 6, 26);
            break;
        case 7:
            printf(".7");
            // rotateCube(cube, -1, -1, 4, 24);
            break;
        case 8:
            printf(".8");
            // rotateCube(cube, -1, 2, 4, -1);
            break;
        case 9:
            printf(".9");
            // rotateCube(cube, -1, 1, 4, -1);
            break;
        case 10:
            printf(".10");
            // rotateCube(cube, -1, 0, 4, -1);
            break;
        case 11:
            printf(".11");
            // rotateCube(cube, 2, -1, 2, -1);
            break;
        case 12:
            printf(".12");
            // rotateCube(cube, 1, -1, 2, -1);
            break;
        case 13:
            printf(".13");
            // rotateCube(cube, 0, -1, 2, -1);
            break;
        }
    }
}

void showCube(Cube cube, int face)
{
    int b, count = 0;
    printf("Cube: \n");
    printf("%21c", ' ');
    for (b = 0; b < 9; b++)
    {
        printf(" %s%s%s", convertColor(cube.faceValue[face][b]), BLOCK, RESET);
        count++;
        if (count % 3 == 0)
        {
            printf("\n");
            if (count != 9)
            {
                printf("%21c", ' ');
            }
        }
    }
}

void rotateCube(Cube *cube, int column, int row, int move, int specialMove)
{
    if (column != -1)
    {
        char backup[13];
        int i, direction = (move == 8) ? 0 : 1;
        int position[4] = {0, 1, 4, 5};
        int initIndex[2][4] = {{6, 0, 11, 5}, {3, 11, 0, 8}};
        int steps[2][4] = {{1, 1, -1, -1}, {1, -1, 1, -1}};
        for (i = 0; i < 4; i++)
        {
            backup[i * 3] = cube->faceValue[cube->facePosition[position[i]]][column];
            backup[(i * 3) + 1] = cube->faceValue[cube->facePosition[position[i]]][column + 3];
            backup[(i * 3) + 2] = cube->faceValue[cube->facePosition[position[i]]][column + 6];
        }
        for (i = 0; i < 4; i++)
        {
            cube->faceValue[cube->facePosition[position[i]]][column] = backup[initIndex[direction][i]];
            cube->faceValue[cube->facePosition[position[i]]][column + 3] = backup[initIndex[direction][i] + 1 * steps[direction][i]];
            cube->faceValue[cube->facePosition[position[i]]][column + 6] = backup[initIndex[direction][i] + 2 * steps[direction][i]];
        }
        if (column != 1)
        {
            /* codigo pra girar as faces laterais no proprio eixo. */
        }
        
    }
    else if (row != -1)
    {
        /* code */
    }
    else
    {
        /* code */
    }
}