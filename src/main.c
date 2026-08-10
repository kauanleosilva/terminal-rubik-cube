#include "cube.h"
#include "io_utils.h"
#include <stdlib.h>

int main()
{
    char *buffer;
    size_t bufferSize = 8192;
    int userAction[4] = {-1, -1, -1, -1};
    Cube cube;

    if ((buffer = malloc((size_t) bufferSize * sizeof(char))) == NULL) throwError(11);
    cube.faceComplete = 0b10000000;
    setupConsole();
    startGame(&cube);
    while (cube.faceComplete & 0b10000000)
    {
        showCube(cube, buffer, bufferSize);
        getAction(userAction, &cube);
        rotateCube(&cube, *(userAction), *(userAction + 1), *(userAction + 2), *(userAction + 3));
        // função pra conferir vitoria
        if (!(cube.faceComplete ^ 0b10111111))
        {
            // tela de vitoria
        }
    };
    endGame();
    free(buffer);
    return 0;
}