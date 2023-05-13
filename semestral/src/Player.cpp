#include "Bomb.cpp"
#include "Buff.h"
#include <queue>
#include <set>

class Player : public GameObject
{
	public:
		Player () : GameObject() {}
		Player ( int x, int y, int r, bool v, bool p ) : GameObject ( x, y, r, v, p ) {}
		
	private:
		int speed, lives;
		queue<Bomb> bombs;
		set<Buff> buffs;
		Moveset move;
		//virtual moveset for keyboard, online and AI players
};