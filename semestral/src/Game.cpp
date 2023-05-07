#include <vector>
#include <SDL2/SDL.h>
#include "Baloon.cpp"
#include "BotPlayer.cpp"
#include "KeyboardPlayer.cpp"
#include "DestructiveBomb.cpp"
#include "HolyGranadeBomb.cpp"
#include "GameMap.cpp"
#include "OptionsTable.cpp"
#include "ScoreTable.cpp"
#include "StartScreen.cpp"
#include "AllWallPassBuff.cpp"
#include "SpeedUpgradeBuff.cpp"
#include "SpeedDowngradeBuff.cpp"
#include "CanPassObjectsBuff.cpp"
#include "ImmortalityBuff.cpp"
#include "Wall.cpp"

class Game
{
	public:
		Game ()
		{

		}



		~Game ()
		{

		}
	private:
		vector<Player *> players;
		vector<Bomb *> players;
		Interface * currentScreen;

};