#include "cube.h"
#include "io_utils.h"

int main()
{
    int userAction[4] = {-1, -1, -1, -1};
    Cube cube;
    cube.faceComplete = 0b10000000;
    setupConsole();
    startGame(&cube);
    /*while (cube.faceComplete & 0b10000000)
    {
        getAction(userAction, &cube, cube.facePosition[0]);
        // rotateCube(&cube, *(userAction), *(userAction + 1), *(userAction + 2), *(userAction + 3));
        if (!(cube.faceComplete ^ 0b10111111))
        {
            // tela de vitoria
        }     
    };*/
    // endGame();
    return 0;
}