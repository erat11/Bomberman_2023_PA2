#ifndef PLAYER_H
#define PLAYER_H

#include "Bomb.h"
#include "Buff.h"
#include "GameObject.h"
#include <queue>
#include <set>
#include <string>

class Moveset
{
	public:
		Moveset () {}
		Moveset ( const Moveset & m )
		{
			u = m.up(); d = m.down();
			l = m.left(); r = m.right();
			b = m.bomb();
			dir = 0;
		}
		Moveset ( char u, char d, char l, char r, char b ) : 
		u ( u ), d ( d ), l ( l ), r ( r ), b( b ) {}
		bool contain ( char k ) 
		{ 
			if ( k == u ) dir = 0;
			if ( k == d ) dir = 1;
			if ( k == l ) dir = 2;
			if ( k == r ) dir = 3;
			return ( k == u || k == d || k == l || k == r || k == b );
		}
		char up    () const { return u; }
		char down  () const { return d; }
		char left  () const { return l; }
		char right () const { return r; }
		char bomb  () const { return b; }
		char direction () const { return dir; }
	private:
		char u, d, l, r, b;
		int dir; // 0 = NORTH, 1 = SOUTH, 2 = EAST, 3 = WEST
};

class Player : public GameObject
{
	public:
		Player ();
		Player ( int x, int y, int hp, char r,  const string & name ); 
		~Player () override;
		/*Player ( const Player & p );
		Player & operator = ( const Player & p );*/
		int getSpeed () const;
		int getHP () const;
		int getScore () const;
		queue<Bomb*> getBombStack() const;
		pair<int, int> getPos () const;
		void setPos ( int i, int j );
		string getName() const;
		bool bot() const;
		void setMoveset( char a, char b, char c, char d, char e );
		bool hasInMoveset ( char k );
		Moveset getMoveset () const;
		void placeBomb( vector<vector<GameObject*>> & gameMap );
		pair<int, int> getDirectedPos() const;
		void setHP ( int nhp );
	protected:
		pair<int, int> mapPos;
		int speed, hp, score;
		Moveset move;
		queue<Bomb*> bombs;
		//set<Buff> buffs;
		//Moveset move;
		string name;
		bool isBot = 0;
		void initBombStack();
};

#endif