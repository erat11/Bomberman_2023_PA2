#include "Player.h"

class AI : public Player
{
	public:
		AI () : Player () {}
		AI ( int x, int y, int hp, char r, const string & name ) : Player ( x, y, hp, r, name ) { this->bot = 1; }
		void initialize() {}
};