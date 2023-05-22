#include "Bomb.cpp"
#include "Buff.h"
#include <queue>
#include <set>
#include "GameObject.h"
#include <string>

class Player : public GameObject
{
	public:
		Player () : GameObject() 
		{}
		Player ( int x, int y, int r, bool v, bool p ) : GameObject ( x, y, r, v, p ) {}
		void setName ( const string & n ) { name = n; }
		void initBot () {}
	private:
		int speed, lives;
		//queue<Bomb> bombs;
		//set<Buff> buffs;
		//Moveset move;
		string name;
		//virtual moveset for keyboard, online and AI players
};