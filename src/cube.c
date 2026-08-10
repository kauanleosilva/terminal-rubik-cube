#include "cube.h"
#include "colors.h"
#include "io_utils.h"
#include <stdio.h>
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
            cube->faceValue[f][b] = f;
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

void showCube(Cube cube, char *buffer, size_t bufferSize)
{
    int i, j, bufferIndex = 0, col = 46, row = 0;
    char fgColorsCache[3][9][24];
    char bgColorsCache[3][9][24];
    pixel canvas[11][62];

    for (i = 0; i < 11; i++)
    {
        for (j = 0; j < 62; j++)
        {
            canvas[i][j].block = '\0';
            canvas[i][j].fgFace = -1;
            canvas[i][j].fgColor = -1;
            canvas[i][j].bgFace = -1;
            canvas[i][j].bgColor = -1;
        }
    }

    memset(buffer, '\0', bufferSize);

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 9; j++)
        {
            strcpy(fgColorsCache[i][j], convertColor(cube.faceValue[cube.facePosition[i]][j], 1));
            strcpy(bgColorsCache[i][j], convertColor(cube.faceValue[cube.facePosition[i]][j], 0));
        }
    }

    for (i = 0; i < 9; i++)
    {
        setPixel(canvas, row, col, 'b', 1, i, -1, -1);
        for (j = 1; j < 4; j++)
        {
            setPixel(canvas, row, col + j, 'm', 1, i, 1, i);
        }
        if ((i + 1) % 3 == 0)
        {
            setPixel(canvas, row, col + 4, 'u', 1, i, 2, 2 - ((i - 2) / 3));
            setPixel(canvas, row, col + 5, 'n', -1, -1, -1, -1);
            row++;
            col = 46 - (2 * row);
        }
        else
        {
            setPixel(canvas, row, col + 4, 'u', 1, i, -1, -1);
            col += 5;
        }
    }

    for (i = 0; i < 6; i++)
    {
        for (j = 0; j < 5; j++)
        {
            setPixel(canvas, row, col, 'b', 0, i, -1, -1);
            if (i < 3)
            {
                if (j == 4)
                {
                    setPixel(canvas, row, col, 'u', 1, i + 6, -1, -1);
                }
                if (i == 2 && j == 4)
                {
                    setPixel(canvas, row, col, 'u', 1, 8, 2, 0);
                }
                else if (j >= 1 && j <= 3)
                {
                    setPixel(canvas, row, col, 'b', 0, i, 1, i + 6);
                }
            }
            else
            {
                setPixel(canvas, row, col, 'b', 0, i + 3, -1, -1);
                if (j == 4)
                {
                    setPixel(canvas, row, col, ' ', -1, -1, -1, -1);
                }
            }
            col++;
        }
        if ((i + 1) % 3 == 0)
        {
            if (i == 2)
            {
                setPixel(canvas, row, col, 'n', -1, -1, -1, -1);
                row += 5;
            }
            else if (i == 5)
            {
                row -= 3;
            }
            col = 40;
        }
    }

    for (i = 0; i < 6; i++)
    {
        for (j = 0; j < 4; j++)
        {
            setPixel(canvas, row, col + j, 'u', 0, (i % 3) + (i / 3) * 6, -1, -1);
        }
        if ((i + 1) % 3 == 0)
        {
            setPixel(canvas, row, col + 4, 'n', -1, -1, -1, -1);
            if (i == 2)
            {
                row += 5;
            }
            else
            {
                row -= 6;
            }
            col = 40;
        }
        else
        {
            col += 5;
        }
    }

    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (i >= 3 && i <= 5)
            {
                setPixel(canvas, row + 1, col + j, 'm', 0, i, 0, i);
            }
            setPixel(canvas, row, col + j, 'm', 0, i, 0, i);
        }
        if ((i + 1) % 3 == 0)
        {
            setPixel(canvas, row, col + 4, 'n', -1, -1, -1, -1);
            setPixel(canvas, row + 1, col + 4, 'n', -1, -1, -1, -1);
            if (i == 2)
            {
                row += 2;
            }
            else if (i == 5)
            {
                row += 3;
            }
            else
            {
                row = 1;
            }
            col = 40;
        }
        else
        {
            col += 5;
        }
    }

    row = 4;
    col = 54;

    setPixel(canvas, row, col, 'm', 2, 0, 2, 0);
    setPixel(canvas, row + 1, col, 'b', 2, 3, -1, -1);
    setPixel(canvas, row + 2, col, 'm', 2, 3, 2, 3);
    setPixel(canvas, row + 3, col, 'u', 2, 3, -1, -1);
    setPixel(canvas, row + 4, col, 'm', 2, 6, 2, 6);
    setPixel(canvas, row + 5, col, 'm', 2, 6, 2, 6);
    row--;
    col++;

    for (i = 0; i < 3; i++)
    {
        setPixel(canvas, row, col, 'm', 2, i, 2, i);
        setPixel(canvas, row, col + 1, 'm', 2, i, 2, i);
        setPixel(canvas, row + 2, col, 'm', 2, i + 3, 2, i + 3);
        setPixel(canvas, row + 2, col + 1, 'm', 2, i + 3, 2, i + 3);
        setPixel(canvas, row + 5, col, 'm', 2, i + 6, 2, i + 6);
        setPixel(canvas, row + 5, col + 1, 'm', 2, i + 6, 2, i + 6);
        setPixel(canvas, row + 1, col, 'u', 2, i, -1, -1);
        setPixel(canvas, row + 1, col + 1, 'b', 2, i + 3, -1, -1);
        setPixel(canvas, row + 3, col, 'm', 2, i + 3, 2, i + 3);
        setPixel(canvas, row + 3, col + 1, 'u', 2, i + 3, -1, -1);
        setPixel(canvas, row + 4, col, 'b', 2, i + 6, -1, -1);
        setPixel(canvas, row + 4, col + 1, 'm', 2, i + 6, 2, i + 6);
        setPixel(canvas, row + 6, col, 'u', 2, i + 6, -1, -1);
        row--;
        col += 2;
    }

    for (i = 0; i < 11; i++)
    {
        for (j = 0; j < 62; j++)
        {
            if (canvas[i][j].block != '\0')
            {
                if (canvas[i][j].block == ' ')
                {
                    bufferIndex += sprintf(buffer + bufferIndex, "%c", ' ');
                }
                else if (canvas[i][j].block == 'n')
                {
                    bufferIndex += sprintf(buffer + bufferIndex, "%s", "g");
                }
                else
                {
                    if (canvas[i][j].bgFace != -1)
                    {
                        bufferIndex += sprintf(buffer + bufferIndex, "%s", bgColorsCache[canvas[i][j].bgFace][canvas[i][j].bgColor]);
                    }
                    if (canvas[i][j].fgFace != -1)
                    {
                        bufferIndex += sprintf(buffer + bufferIndex, "%s", fgColorsCache[canvas[i][j].fgFace][canvas[i][j].fgColor]);
                    }

                    bufferIndex += sprintf(buffer + bufferIndex, "%s", convertBlock(canvas[i][j].block));
                    bufferIndex += sprintf(buffer + bufferIndex, "%s", RESET);
                }
            }
            else
            {
                bufferIndex += sprintf(buffer + bufferIndex, "%c", 'l');
            }
        }
        bufferIndex += sprintf(buffer + bufferIndex, "\n");
    }

    fputs(buffer, stdout);
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
                    if ((move == 1 && (i == 4 || i == 5)) || (move == 2 && (i == 1 || i == 5)))
                    {
                        for (j = 0; j < 9; j++)
                        {
                            spinBackup[j] = cube->faceValue[cube->facePosition[i]][j];
                        }
                        for (j = 0; j < 9; j++)
                        {
                            cube->faceValue[cube->facePosition[i]][j] = spinBackup[8 - j];
                        }
                    }
                }
            }
        }
    }
}

char *convertBlock(char code)
{
    switch (code)
    {
    case 'u':
        return UH_BLOCK;
        break;
    case 'm':
        return BLOCK;
        break;
    case 'b':
        return BH_BLOCK;
        break;
    default:
        return "deu merda";
        break;
    }
}