/////////////////////////////////////////////////////////////////////////////
// Authored by Jeong-Mo Hong at Dongguk University CSE  //
// jeongmo.hong@gmail.com                                                  //
// Do whatever you want license.                                           //
/////////////////////////////////////////////////////////////////////////////

#include "ConsoleGame.h"

ConsoleGame::ConsoleGame()
{
    resetBuffers();
    
    // hide cursor
    CONSOLE_CURSOR_INFO CurInfo;
    CurInfo.dwSize = 1;
    CurInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

void ConsoleGame::resetBuffers()
{
    // initialize buffers
    for (int j = 0; j < SCR_HEIGHT; j++)
        for (int i = 0; i < SCR_WIDTH; i++)
        {
            front_buffer[j][i] = '\0';
            back_buffer[j][i] = '\0';
        }
}

void ConsoleGame::drawToBackBuffer(const int i, const int j, char *image)
{
    int ix = 0;
    while (true)
    {
        if (image[ix] == '\0') break;

        assert(i + ix < SCR_WIDTH);
        assert(i >= 0);
        assert(j >= 0);
        assert(j < SCR_HEIGHT);

        back_buffer[j][i + ix] = image[ix];

        ix++;
    }
}

void ConsoleGame::moveCursorTo(const short x, const short y)     // move cursor to (x, y) of screen space to start console text output
{
    const COORD pos = { x, SCR_HEIGHT - y - 1 };  // To make left bottom is (0, 0).  In windows screen coordinates, left up is (0, 0).

                                                  // https://msdn.microsoft.com/ko-kr/library/windows/desktop/ms686025(v=vs.85).aspx
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
};

void ConsoleGame::render()
{
    // re-draw changed parts only
    for (int j = 0; j < SCR_HEIGHT; j++)
        for (int i = 0; i < SCR_WIDTH; i++)
        {
            if (back_buffer[j][i] != front_buffer[j][i])
            {
                moveCursorTo(i, j);

                if (back_buffer[j][i] == '\0')
                    printf("%c", ' ');
                else
                    printf("%c", back_buffer[j][i]);
            }
        }

    // draw all of back-buffer (debugging)
    //for (int j = 0; j < SCR_HEIGHT; j++)
    //    for (int i = 0; i < SCR_WIDTH; i++)
    //    {
    //        moveCursorTo(i, j);

    //        if (back_buffer[j][i] == '\0')
    //            printf("%c", ' ');
    //        else
    //            printf("%c", back_buffer[j][i]);
    //    }

    // update front buffer and clear back buffer
    for (int j = 0; j < SCR_HEIGHT; j++)
        for (int i = 0; i < SCR_WIDTH; i++)
        {
            front_buffer[j][i] = back_buffer[j][i];
            back_buffer[j][i] = '\0';
        }

    // draw all of front-buffer (debugging)
    //for (int j = 0; j < SCR_HEIGHT; j++)
    //    for (int i = 0; i < SCR_WIDTH; i++)
    //    {
    //        moveCursorTo(i, j);

    //        if (front_buffer[j][i] == '\0')
    //            printf("%c", ' ');
    //        else
    //            printf("%c", front_buffer[j][i]);
    //    }
}