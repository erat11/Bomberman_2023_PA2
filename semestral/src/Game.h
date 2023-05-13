#ifndef GAME_H
#define GAME_H

#include <vector>
#include "StartScreen.h"

class Game
{
public:
    Game();
    void Run();
    ~Game();

private:
    StartScreen screen;
};

#endif