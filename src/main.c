#include <stdlib.h>
#include "cube.h"
#include "io_utils.h"

int main(void)
{
    char *buffer;
    size_t bufferSize = 8192;
    int userAction[4] = {-1, -1, -1, -1};
    Cube cube;

    if ((buffer = malloc(bufferSize * sizeof(char))) == NULL)
        handleError(11);
    cube.faceComplete = 0b10000000;
    setupConsole();
    startGame(&cube);
    // função que renderiza o background
    while (cube.faceComplete & 0b10000000)
    {
        renderCube(cube, buffer, bufferSize);
        getAction(userAction, &cube);
        rotateCube(&cube, userAction[0], userAction[1], userAction[2], userAction[3]);
        evaluateFaces(&cube);
        if (!(cube.faceComplete ^ 0b10111111))
        {
            // tela de vitoria
            cube.faceComplete &= ~0b10000000;
        }
    }
    endGame();
    free(buffer);
    return 0;
}