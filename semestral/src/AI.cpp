#include "Player.h"

class AI : public Player
{
	public:
		AI () : Player () {}
		~AI () override
		{
			while ( ! bombs.empty() )
			{
				auto b = bombs.front();
				delete b;
				bombs.pop();
			}
		}
		AI ( int x, int y, int hp, char r, const string & name ) : Player ( x, y, hp, r, name ) { this->isBot = 1; }
		void initialize() {}
};