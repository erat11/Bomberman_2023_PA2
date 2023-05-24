#ifndef GAMEMAP_H
#define GAMEMAP_H

#include "Interface.h"
#include "Player.h"
#include "AI.cpp"
#include "Wall.cpp"
#define PATH_TO_MAP_FILE "./config/maps/selected"

class GameMap : public Interface
{
	public:
		GameMap ();
		GameMap ( const char * caption );
		bool load ( int h, const string & s1, const string & s2 );
		void addDollars ( int chance );
		void print () override;
		void handleInput () override;
		int getSizeX();
		int getSizeY();
	private:
		int sizeX, sizeY, startX, startY;
		vector<vector<GameObject>> gameMap;
		void mapPrint ();
		void playerPrint();
		void printHearts ( int wx, int wy, int hp );
		void swapper ( int a, int b, int c, int d, bool player );
		Player player1, player2;
		AI bot;
};

#endif