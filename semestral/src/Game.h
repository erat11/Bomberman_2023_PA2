#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include "StartScreen.h"
#include "GetNameScreen.cpp"
#include "Player.h"
#include "GameMap.h"
#include "SettingsScreen.cpp"
#include "MapChooseScreen.cpp"

class Game
{
public:
    Game();
    void Run();
    ~Game();
private:
    void StartGame();
    void GameLoop( bool p );
    GameMap playMap;

};

#endif