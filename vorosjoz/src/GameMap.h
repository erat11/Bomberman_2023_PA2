#ifndef GAMEMAP_H
#define GAMEMAP_H

#include "Interface.h"
#include "Player.h"
#include "AI.cpp"
#include "AllBuffs.cpp"
#include "Leaderboard.cpp"
#define PATH_TO_MAP_FILE "./config/maps/selected"

class GameSettingsWrapper
{
	public:
		GameSettingsWrapper () {}
		GameSettingsWrapper ( int h, bool d, bool b ) : hp ( h ), debuffs ( d ), baloons ( b ) {}
	    int hp;
	    bool debuffs, baloons;
};

class GameMap : public Interface
{
	public:
		GameMap ();
		GameMap ( const char * caption );
		bool load ( const string & s1, const string & s2, GameSettingsWrapper gsw, BuffChancesWrapper bcw );
		void addDollars ( int chance );
		void setDropChance ( int chance );
		void print () override;
		void handleInput () override;
		int getSizeX();
		int getSizeY();
		~GameMap ();
	private:
		int sizeX, sizeY, startX, startY;
		vector<vector<GameObject*>> gameMap;
		void winScreen();
		void mapPrint ();
		void playerInfoPrint( Player * p, int writeX, int writeY );
		void printHearts ( int wx, int wy, int hp );
		void swapper ( int a, int b, int c, int d, Player * p );
		void handlePlayer ( char x, Player * player );
		void handleAI();
		bool isPosEmpty ( int i, int j );
		void updateBomb ( GameObject * y );
		Buff * pickRandomBuff();
		Player * player1, * player2;
		vector<Bomb> placedBombs;
		vector<Explosion> explosions;
		GameSettingsWrapper gsw;
		BuffChancesWrapper bcw;



};

#endif