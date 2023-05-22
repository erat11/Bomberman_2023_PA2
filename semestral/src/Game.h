#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include "StartScreen.h"
#include "GetNameScreen.cpp"
#include "Player.cpp"
#include "GameMap.cpp"
#include "SettingsScreen.cpp"

class Game
{
public:
    Game();
    void Run();
    ~Game();
private:
    void StartGame();
    void GameLoop( bool p );
    Player player1, player2;
    GameMap playMap;

};

#endif