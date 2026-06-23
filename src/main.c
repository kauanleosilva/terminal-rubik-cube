#include "cube.h"
#include "io_utils.h"

int main()
{
    int userAction[4] = {-1, -1, -1, -1};
    char seed[21];
    Cube cube;

    cube.faceComplete = 0b10000000;
    setupConsole();
    startGame(&cube, seed);
    while (cube.faceComplete & 0b10000000)
    {
        getAction(userAction, &cube, seed);
        rotateCube(&cube, *(userAction), *(userAction + 1), *(userAction + 2), *(userAction + 3));
        if (!(cube.faceComplete ^ 0b10111111))
        {
            // tela de vitoria
        }
    };
    endGame();
    return 0;
}