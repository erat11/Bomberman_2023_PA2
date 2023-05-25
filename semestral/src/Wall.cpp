#include "GameObject.h"

class Wall : public GameObject
{
	public:
		Wall () : GameObject () { }
		Wall ( int hp, char r ) : GameObject ( hp, r ) 
		{
			if ( r == '|' || r == '-' ) { hp = -1; pernament = 1; visibility = 1; }
			if ( r == '#' )             { hp = 10; pernament = 0; visibility = 1; }
			if ( r == '~' )             { hp = 1;  pernament = 0; visibility = 1; }
			else                        { hp = 0,  pernament = 0; visibility = 0; }
		}
	private:
		bool pernament, visibility;
};