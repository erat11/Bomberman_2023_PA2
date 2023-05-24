#ifndef PLAYER_H
#define PLAYER_H

#include "Bomb.cpp"
#include "Buff.h"
#include "GameObject.h"
#include <queue>
#include <set>
#include <string>

class Player : public GameObject
{
	public:
		Player ();
		Player ( int x, int y, int hp, char r,  const string & name ); 
		void setPos ( int x, int y );
		int getSpeed ();
		int getHP ();
		int getScore ();
		pair<int, int> getPos () const;
		string getName();
	protected:
		pair<int, int> mapPos;
		int speed, hp, score;
		//queue<Bomb> bombs;
		//set<Buff> buffs;
		//Moveset move;
		string name;
		//virtual moveset for keyboard, online and AI players
		bool bot = 0;
};

#endif