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
		void lightBomb();
		void detonate( vector<vector<GameObject*>> & gameMap );
		bool ready();
		pair<int, int> getPos () const;
		void setPos ( int i, int j );
		void upgrade ();
		Bomb * clone () const override;
	protected:
		int expRadius, detonationTime, damage, expWidth;
		pair<int, int> mapPos;
		chrono::system_clock::time_point start;
		void explodeSide( vector<vector<GameObject*>> & gameMap, int i, int j, int dir );
};

#endif