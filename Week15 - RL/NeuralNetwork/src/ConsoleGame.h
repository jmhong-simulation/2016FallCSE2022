/////////////////////////////////////////////////////////////////////////////
// Authored by Jeong-Mo Hong at Dongguk University CSE  //
// jeongmo.hong@gmail.com                                                  //
// Do whatever you want license.                                           //
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <stdbool.h>
#include <assert.h>

#define SCR_WIDTH   20
#define SCR_HEIGHT  20

class ConsoleGame
{
public:
    char front_buffer[SCR_HEIGHT][SCR_WIDTH]; // note [j][i] notation
    char back_buffer[SCR_HEIGHT][SCR_WIDTH];  // note [j][i] notation

public:
    ConsoleGame();

public:
    void resetBuffers();
    void drawToBackBuffer(const int i, const int j, char *image);
    void moveCursorTo(const short x, const short y);
    void render();
};
