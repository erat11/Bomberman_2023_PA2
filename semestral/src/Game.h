#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include "StartScreen.cpp"
#include "GetNameScreen.cpp"
#include "Player.cpp"

class Game
{
public:
    Game();
    void Run();
    ~Game();
private:
    void StartGame();
    int GameLoop( bool p );
    Player player1, player2;

};

#endif