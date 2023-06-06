#include "GameObject.h"

class Baloon : public GameObject
{
	public:
		Baloon () : GameObject () {}
		Baloon ( int i, int j ) : GameObject ( 2, 'B' ), mapPos ( make_pair ( i, j ) )
		{
		}
	private:
		pair<int,int> mapPos;
};