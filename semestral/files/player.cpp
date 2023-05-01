#include "gameObject.cpp"

class player : public gameObject
{
	public:
		player () : gameObject() {}
		player ( int x, int y, int r, true ) : gameObject ( x, y, r, true ) {}
};