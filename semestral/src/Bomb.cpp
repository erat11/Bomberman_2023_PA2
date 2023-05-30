#include "Bomb.h"

Bomb::Bomb ()
{
	mapRep = 'O';
	hp = 1;
	expRadius = 4;
	damage = 2;
	detonationTime = 2;
	type = 1;
}

void Bomb::lightBomb()
{
	start = chrono::high_resolution_clock::now();
}

bool Bomb::ready()
{
	auto now = chrono::high_resolution_clock::now();
	auto z = chrono::duration_cast<chrono::seconds>(now - start).count();
	return z > detonationTime;
}

void Bomb::detonate( vector<vector<GameObject*>> & gameMap )
{
	int i = mapPos.first, j = mapPos.second, dir = 0;// 0 = NORTH, 1 = SOUTH, 2 = EAST, 3 = WEST
	for ( dir = 0; dir < 4; ++dir) explodeSide( gameMap, i, j, dir );
}

void Bomb::explodeSide( vector<vector<GameObject*>> & gameMap, int i, int j, int dir )
{
	int x = 0, y = 0;
	switch ( dir )
	{
		case(0): { x = -1; break; }
		case(1): { x = 1; break; }
		case(2): { y = 1; break; }
		default: { y = -1; break; }
	}
	for (int k = 0; k < expRadius; ++k)
	{
		i += x;
		j += y;
		if ( abs(i + 1) < gameMap.size() && i > 0 && abs(j + 1) < gameMap[0].size() && j > 0 )
			if ( gameMap[i][j]->getHP() <= damage && gameMap[i][j]->getType() != 2 )
			{
				delete gameMap[i][j];
				gameMap[i][j] = new Explosion();
				
			}
			else { gameMap[i][j]->decreaseHP( damage ); break; }
	}
}



pair<int, int> Bomb::getPos () const { return mapPos; }
void Bomb::setPos ( int i, int j ) { mapPos = make_pair ( i, j ); }
