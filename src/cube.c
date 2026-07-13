#include "cube.h"
#include "colors.h"
#include "stdio.h"
#include <ctype.h>
#include <stdint.h>
#include <string.h>

#define quantMoves 30

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
    uint32_t seed = 0;

    for (i = 0; i < size; i++)
    {
        uint32_t asciiCode = (uint32_t)*(rawSeed + i);
        seed ^= (asciiCode * asciiCode * asciiCode) * (i + 1);
        seed = (seed << 8) | (seed >> 24);
    }
    for (i = 0; i < quantMoves; i++)
    {
        while (1)
        {
            do
            {
                seed ^= seed * ((i + 1) * 20);
                seed = (seed << 17) | (seed >> 15);
            } while (seed >= 4294967292U);
            action = seed % 14;
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
    int actionCode[quantMoves], i;

    seedGenerator(rawSeed, size, actionCode);
    for (i = 0; i < quantMoves; i++)
    {
        switch (actionCode[i])
        {
        case 0:
            rotateCube(cube, 0, -1, 8, -1);
            break;
        case 1:;
            rotateCube(cube, 1, -1, 8, -1);
            break;
        case 2:
            rotateCube(cube, 2, -1, 8, -1);
            break;
        case 3:
            rotateCube(cube, -1, 0, 6, -1);
            break;
        case 4:
            rotateCube(cube, -1, 1, 6, -1);
            break;
        case 5:
            rotateCube(cube, -1, 2, 6, -1);
            break;
        case 6:
            rotateCube(cube, -1, -1, 6, 20);
            break;
        case 7:
            rotateCube(cube, -1, -1, 4, 20);
            break;
        case 8:
            rotateCube(cube, -1, 2, 4, -1);
            break;
        case 9:
            rotateCube(cube, -1, 1, 4, -1);
            break;
        case 10:
            rotateCube(cube, -1, 0, 4, -1);
            break;
        case 11:
            rotateCube(cube, 2, -1, 2, -1);
            break;
        case 12:
            rotateCube(cube, 1, -1, 2, -1);
            break;
        case 13:
            rotateCube(cube, 0, -1, 2, -1);
            break;
        }
    }
}

void showCube(Cube cube)
{
    int i, j, k;
    char lColor[12];

    printf("Cube: \n\n\n");
    printf("%46c", ' ');
    for (i = 0; i < 9; i++)
    {
        strcpy(lColor, convertColor(cube.faceValue[cube.facePosition[1]][i], 1));
        printf("%s%s%s", lColor, BH_BLOCK, RESET);
        for (j = 0; j < 3; j++)
        {
            printf("%s%s%s%s", convertColor(cube.faceValue[cube.facePosition[1]][i], 0), lColor, BLOCK, RESET);
        }
        if (i == 2)
        {
            printf("%s%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][2], 0), lColor, UH_BLOCK, RESET);
            printf("\u200B\n%44c", ' ');
        }
        else if (i == 5)
        {
            printf("%s%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][1], 0), lColor, UH_BLOCK, RESET);
            printf("%s%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][2], 0), convertColor(cube.faceValue[cube.facePosition[2]][2], 1), BLOCK, RESET);
            printf("%s%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][2], 0), convertColor(cube.faceValue[cube.facePosition[2]][2], 1), BLOCK, RESET);
            printf("\u200B\n%42c", ' ');
        }
        else if (i == 8)
        {
            printf("%s%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][0], 0), lColor, UH_BLOCK, RESET);
            printf("%s%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][1], 0), convertColor(cube.faceValue[cube.facePosition[2]][1], 1), BLOCK, RESET);
            printf("%s%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][1], 0), convertColor(cube.faceValue[cube.facePosition[2]][1], 1), BLOCK, RESET);
            printf("%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][2], 1), UH_BLOCK, RESET);
            printf("%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][5], 1), BH_BLOCK, RESET);
            printf("\u200B\n%40c", ' ');
        }
        else
        {
            printf("%s%s%s", lColor, UH_BLOCK, RESET);
        }
    }
    for (i = 0; i < 3; i++)
    {
        strcpy(lColor, convertColor(cube.faceValue[cube.facePosition[0]][i], 1));
        printf("%s%s%s", lColor, BH_BLOCK, RESET);
        for (j = 0; j < 3; j++)
        {
            printf("%s%s%s%s", convertColor(cube.faceValue[cube.facePosition[1]][6 + i], 0), lColor, BH_BLOCK, RESET);
        }
        if (i == 2)
        {
            printf("%s%s%s%s", convertColor(cube.faceValue[cube.facePosition[1]][6 + i], 0), convertColor(cube.faceValue[cube.facePosition[2]][0], 1), BH_BLOCK, RESET);
            printf("%s%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][0], 0), convertColor(cube.faceValue[cube.facePosition[2]][0], 1), BLOCK, RESET);
            printf("%s%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][0], 0), convertColor(cube.faceValue[cube.facePosition[2]][0], 1), BLOCK, RESET);
            printf("%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][1], 1), UH_BLOCK, RESET);
            printf("%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][4], 1), BH_BLOCK, RESET);
            printf("%s%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][5], 0), convertColor(cube.faceValue[cube.facePosition[2]][5], 1), BLOCK, RESET);
            printf("%s%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][5], 0), convertColor(cube.faceValue[cube.facePosition[2]][5], 1), BLOCK, RESET);
            printf("\u200B\n%40c", ' ');
        }
        else
        {
            printf("%s%s%s", convertColor(cube.faceValue[cube.facePosition[1]][6 + i], 1), UH_BLOCK, RESET);
        }
    }
    for (i = 0; i < 3; i++)
    {
        strcpy(lColor, convertColor(cube.faceValue[cube.facePosition[0]][i], 1));
        for (j = 0; j < 4; j++)
        {
            printf("%s%s%s%s", convertColor(cube.faceValue[cube.facePosition[0]][i], 0), lColor, BLOCK, RESET);
        }
        if (i == 2)
        {
            printf("%s%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][0], 0), convertColor(cube.faceValue[cube.facePosition[2]][0], 1), BLOCK, RESET);
            printf("%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][0], 1), UH_BLOCK, RESET);
            printf("%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][3], 1), BH_BLOCK, RESET);
            printf("%s%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][4], 0), convertColor(cube.faceValue[cube.facePosition[2]][4], 1), BLOCK, RESET);
            printf("%s%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][4], 0), convertColor(cube.faceValue[cube.facePosition[2]][4], 1), BLOCK, RESET);
            printf("%s%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][5], 0), convertColor(cube.faceValue[cube.facePosition[2]][5], 1), BLOCK, RESET);
            printf("%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][5], 1), UH_BLOCK, RESET);
            printf("\u200B\n%40c", ' ');
        }
        else
        {
            printf(" ");
        }
    }
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 4; j++)
        {
            printf("%s%s%s", convertColor(cube.faceValue[cube.facePosition[0]][i], 1), UH_BLOCK, RESET);
        }
        if (i == 2)
        {
            printf("%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][3], 1), BH_BLOCK, RESET);
            printf("%s%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][3], 0), convertColor(cube.faceValue[cube.facePosition[2]][3], 1), BLOCK, RESET);
            printf("%s%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][3], 0), convertColor(cube.faceValue[cube.facePosition[2]][3], 1), BLOCK, RESET);
            printf("%s%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][4], 0), convertColor(cube.faceValue[cube.facePosition[2]][4], 1), BLOCK, RESET);
            printf("%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][4], 1), UH_BLOCK, RESET);
            printf("%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][8], 1), BH_BLOCK, RESET);
            printf("%s%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][8], 0), convertColor(cube.faceValue[cube.facePosition[2]][8], 1), BLOCK, RESET);
            printf("\u200B\n%40c", ' ');
        }
        else
        {
            printf(" ");
        }
    }
    for (i = 0; i < 2; i++)
    {
        for (j = 3; j < 6; j++)
        {
            for (k = 0; k < 4; k++)
            {
                printf("%s%s%s%s", convertColor(cube.faceValue[cube.facePosition[0]][j], 0), convertColor(cube.faceValue[cube.facePosition[0]][j], 1), BLOCK, RESET);
            }
            if (j == 5 && i == 0)
            {
                printf("%s%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][3], 0), convertColor(cube.faceValue[cube.facePosition[2]][3], 1), BLOCK, RESET);
                printf("%s%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][3], 0), convertColor(cube.faceValue[cube.facePosition[2]][3], 1), BLOCK, RESET);
                printf("%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][3], 1), UH_BLOCK, RESET);
                printf("%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][7], 1), BH_BLOCK, RESET);
                printf("%s%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][7], 0), convertColor(cube.faceValue[cube.facePosition[2]][7], 1), BLOCK, RESET);
                printf("%s%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][8], 0), convertColor(cube.faceValue[cube.facePosition[2]][8], 1), BLOCK, RESET);
                printf("%s%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][8], 0), convertColor(cube.faceValue[cube.facePosition[2]][8], 1), BLOCK, RESET);
                printf("\u200B\n%40c", ' ');
            }
            else if (j == 5 && i == 1)
            {
                printf("%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][3], 1), UH_BLOCK, RESET);
                printf("%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][6], 1), BH_BLOCK, RESET);
                printf("%s%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][6], 0), convertColor(cube.faceValue[cube.facePosition[2]][6], 1), BLOCK, RESET);
                printf("%s%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][7], 0), convertColor(cube.faceValue[cube.facePosition[2]][7], 1), BLOCK, RESET);
                printf("%s%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][7], 0), convertColor(cube.faceValue[cube.facePosition[2]][7], 1), BLOCK, RESET);
                printf("%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][8], 1), UH_BLOCK, RESET);
                printf("\u200B\n%40c", ' ');
            }
            else
            {
                printf(" ");
            }
        }
    }
    for (i = 6; i < 9; i++)
    {
        for (j = 0; j < 4; j++)
        {
            printf("%s%s%s", convertColor(cube.faceValue[cube.facePosition[0]][i], 1), BH_BLOCK, RESET);
        }
        if (i == 8)
        {
                printf("%s%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][6], 0), convertColor(cube.faceValue[cube.facePosition[2]][6], 1), BLOCK, RESET);
                printf("%s%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][6], 0), convertColor(cube.faceValue[cube.facePosition[2]][6], 1), BLOCK, RESET);
                printf("%s%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][6], 0), convertColor(cube.faceValue[cube.facePosition[2]][6], 1), BLOCK, RESET);
                printf("%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][7], 1), UH_BLOCK, RESET);
                printf("\u200B\n%40c", ' ');
        } else {
            printf(" ");
        }
    }
    for (i = 6; i < 9; i++)
    {
        for (j = 0; j < 4; j++)
        {
            printf("%s%s%s%s", convertColor(cube.faceValue[cube.facePosition[0]][i], 0), convertColor(cube.faceValue[cube.facePosition[0]][i], 1), BLOCK, RESET);
        }
        if (i == 8)
        {
            printf("%s%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][6], 0), convertColor(cube.faceValue[cube.facePosition[2]][6], 1), BLOCK, RESET);
            printf("%s%s%s", convertColor(cube.faceValue[cube.facePosition[2]][6], 1), UH_BLOCK, RESET);
            printf("\u200B\n%40c", ' ');
        } else {
            printf(" ");
        }
    }
    for (i = 6; i < 9; i++)
    {
        for (j = 0; j < 4; j++)
        {
            printf("%s%s%s", convertColor(cube.faceValue[cube.facePosition[0]][i], 1), UH_BLOCK, RESET);
        }
        printf(" ");
    }
    printf("\u200B\n%40c", ' ');
}

void rotateCube(Cube *cube, int column, int row, int move, int specialMove)
{
    char backup[12] = {};
    int i;
    int rotationMap[2][9] = {{6, 3, 0, 7, 4, 1, 8, 5, 2}, {2, 5, 8, 1, 4, 7, 0, 3, 6}};

    if (column != -1)
    {
        int direction = (move == 8) ? 0 : 1;
        int position[4] = {0, 1, 4, 5};
        int initIndex[2][4] = {{6, 0, 11, 5}, {3, 11, 0, 8}};
        int steps[2][4] = {{1, 1, -1, -1}, {1, -1, 1, -1}};
        int invertedCol = 2 - column;

        for (i = 0; i < 4; i++)
        {
            if (i == 3)
            {
                backup[i * 3] = cube->faceValue[cube->facePosition[position[i]]][invertedCol];
                backup[(i * 3) + 1] = cube->faceValue[cube->facePosition[position[i]]][invertedCol + 3];
                backup[(i * 3) + 2] = cube->faceValue[cube->facePosition[position[i]]][invertedCol + 6];
            }
            else
            {
                backup[i * 3] = cube->faceValue[cube->facePosition[position[i]]][column];
                backup[(i * 3) + 1] = cube->faceValue[cube->facePosition[position[i]]][column + 3];
                backup[(i * 3) + 2] = cube->faceValue[cube->facePosition[position[i]]][column + 6];
            }
        }
        for (i = 0; i < 4; i++)
        {
            if (i == 3)
            {
                cube->faceValue[cube->facePosition[position[i]]][invertedCol] = backup[initIndex[direction][i]];
                cube->faceValue[cube->facePosition[position[i]]][invertedCol + 3] = backup[initIndex[direction][i] + 1 * steps[direction][i]];
                cube->faceValue[cube->facePosition[position[i]]][invertedCol + 6] = backup[initIndex[direction][i] + 2 * steps[direction][i]];
            }
            else
            {
                cube->faceValue[cube->facePosition[position[i]]][column] = backup[initIndex[direction][i]];
                cube->faceValue[cube->facePosition[position[i]]][column + 3] = backup[initIndex[direction][i] + 1 * steps[direction][i]];
                cube->faceValue[cube->facePosition[position[i]]][column + 6] = backup[initIndex[direction][i] + 2 * steps[direction][i]];
            }
        }
        if (column != 1)
        {
            int faceToRotate = (column == 0) ? 3 : 2;

            for (i = 0; i < 9; i++)
            {
                backup[i] = cube->faceValue[cube->facePosition[faceToRotate]][i];
            }
            for (i = 0; i < 9; i++)
            {
                cube->faceValue[cube->facePosition[faceToRotate]][i] = backup[rotationMap[(direction + faceToRotate % 2) % 2][i]];
            }
        }
    }
    else if (row != -1)
    {
        int direction = (move == 6) ? 0 : 1;
        int position[4] = {0, 2, 3, 5};
        int initIndex[2][4] = {{6, 0, 9, 3}, {3, 9, 0, 6}};

        for (i = 0; i < 4; i++)
        {
            backup[i * 3] = cube->faceValue[cube->facePosition[position[i]]][(row * 3)];
            backup[(i * 3) + 1] = cube->faceValue[cube->facePosition[position[i]]][(row * 3) + 1];
            backup[(i * 3) + 2] = cube->faceValue[cube->facePosition[position[i]]][(row * 3) + 2];
        }
        for (i = 0; i < 4; i++)
        {
            cube->faceValue[cube->facePosition[position[i]]][(row * 3)] = backup[initIndex[direction][i]];
            cube->faceValue[cube->facePosition[position[i]]][(row * 3) + 1] = backup[initIndex[direction][i] + 1];
            cube->faceValue[cube->facePosition[position[i]]][(row * 3) + 2] = backup[initIndex[direction][i] + 2];
        }
        if (row != 1)
        {
            int faceToRotate = (row == 0) ? 1 : 4;

            for (i = 0; i < 9; i++)
            {
                backup[i] = cube->faceValue[cube->facePosition[faceToRotate]][i];
            }
            for (i = 0; i < 9; i++)
            {
                cube->faceValue[cube->facePosition[faceToRotate]][i] = backup[rotationMap[(direction + faceToRotate % 2) % 2][i]];
            }
        }
    }
    else if (specialMove != -1)
    {
        if (specialMove == 20)
        {
            int direction = (move == 6) ? 0 : 1;
            int adjacentFaces[4][2] = {{3, 2}, {1, 6}, {2, 0}, {4, 0}};
            int targetBlock[2][4] = {{2, 8, 0, 2}, {8, 6, 6, 0}};
            int sourceBackup[2][4] = {{9, 0, 3, 6}, {3, 6, 9, 0}};
            int step;

            for (i = 0; i < 9; i++)
            {
                backup[i] = cube->faceValue[cube->facePosition[0]][i];
            }
            for (i = 0; i < 9; i++)
            {
                cube->faceValue[cube->facePosition[0]][i] = backup[rotationMap[direction][i]];
            }
            for (i = 0; i < 4; i++)
            {
                step = (i % 2 == 0) ? 3 : 1;
                backup[i * 3] = cube->faceValue[cube->facePosition[adjacentFaces[i][0]]][adjacentFaces[i][1]];
                backup[(i * 3) + 1] = cube->faceValue[cube->facePosition[adjacentFaces[i][0]]][adjacentFaces[i][1] + (1 * step)];
                backup[(i * 3) + 2] = cube->faceValue[cube->facePosition[adjacentFaces[i][0]]][adjacentFaces[i][1] + (2 * step)];
            }
            for (i = 0; i < 4; i++)
            {
                switch (direction)
                {
                case 0:
                    step = (i % 2 == 0) ? 3 : -1;
                    break;
                case 1:
                    step = (i % 2 == 0) ? -3 : 1;
                    break;
                }
                cube->faceValue[cube->facePosition[adjacentFaces[i][0]]][targetBlock[direction][i]] = backup[sourceBackup[direction][i]];
                cube->faceValue[cube->facePosition[adjacentFaces[i][0]]][targetBlock[direction][i] + (1 * step)] = backup[sourceBackup[direction][i] + 1];
                cube->faceValue[cube->facePosition[adjacentFaces[i][0]]][targetBlock[direction][i] + (2 * step)] = backup[sourceBackup[direction][i] + 2];
            }
        }
        else if (specialMove == 10)
        {
            char facesToCycle[4][6] = {{3, -9, 0, 5, 14, 2}, {4, 0, 12, -7, 5, 1}, {1, 5, -8, 13, 0, 4}, {2, 11, 5, 0, -6, 3}};
            char spinBackup[9];
            int j;

            for (i = 0; i < 6; i++)
            {
                backup[i] = cube->facePosition[i];
            }
            for (i = 0; i < 6; i++)
            {
                if (facesToCycle[move][i] < 0 || facesToCycle[move][i] > 10)
                {
                    unsigned char faceToSpin = (facesToCycle[move][i] < 0) ? (facesToCycle[move][i] + 10) : (facesToCycle[move][i] - 10);
                    int direction = (facesToCycle[move][i] > 10) ? 0 : 1;

                    for (j = 0; j < 9; j++)
                    {
                        spinBackup[j] = cube->faceValue[cube->facePosition[faceToSpin]][j];
                    }
                    for (j = 0; j < 9; j++)
                    {
                        cube->faceValue[cube->facePosition[faceToSpin]][j] = spinBackup[rotationMap[direction][j]];
                    }
                }
                else
                {
                    cube->facePosition[i] = backup[(int)facesToCycle[move][i]];
                }
            }
        }
    }
}