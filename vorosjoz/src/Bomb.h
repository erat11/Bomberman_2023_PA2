#ifndef BOMB_H
#define BOMB_H

#include "GameObject.h"
#include <ctime>
#include <vector>
#include "Wall.cpp"
#include <cmath>

class Bomb : public GameObject
{
	public:
		Bomb ();
		Bomb ( bool p );
		void lightBomb();
		void detonate ( vector<vector<GameObject*>> & gameMap );
		bool ready( vector<vector<GameObject*>> gameMap );
		pair<int, int> getPos () const;
		void setPos ( int i, int j );
		void upgrade ( int i );
		Bomb * clone () const override;
		bool getPlayer() const;
		long long getScore () const override;
		int getLevel () const;
	protected:
		int expRadius, detonationTime, damage, expWidth, score, level;
		bool player; //0 = player1, 1 = player2
		pair<int, int> mapPos;
		chrono::system_clock::time_point start;
		void explodeSide( vector<vector<GameObject*>> & gameMap, int i, int j, int dir );
		void explodeSide( vector<vector<GameObject*>> & gameMap, int i, int j, pair<int, int> p );
};

#endif