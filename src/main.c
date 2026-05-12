#include <stdio.h>
#include "cube.h"
#include "io_utils.h"

#ifdef _WIN32
#include <windows.h>
#endif

int main()
{
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    if (GetConsoleMode(hOut, &dwMode))
    {
        dwMode |= 0x0004;
        SetConsoleMode(hOut, dwMode);
    }
    SetConsoleOutputCP(65001);
#endif

    int userAction[4] = {-1, -1, -1, -1};
    Cube cube;
    cube.solution = 0b00000000;
    initCube(&cube);
    while (1)
    {
        showCube(cube, cube.facePosition[0]);
        getAction(userAction);
        // rotateCube(&cube, *(userAction), *(userAction + 1), *(userAction + 2), *(userAction + 3));
        if (cube.solution == 0b00111111)
        {
            /* parabeniza e pergunta se quer jogar de novo. se nao quiser, seta cube.solution para 0b10111111*/
        }
        if (cube.solution == 0b10111111)
        {
            clearTerminal();
            printf("\nGoodbye! (Press Enter to close)\n");
            clearBuffer();
            getchar();
            break;
        }
    };
    return 0;
}