#include "Bomb.h"

Bomb::Bomb () : GameObject ( 1, 'O')
{
	expRadius = 3;
	damage = 1;
	detonationTime = 2;
	type = 1;
	expWidth = 1;
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
	if ( expWidth > 1 )
	{
		for (int k = 1; k < ((expWidth - 1) / 2)+1; ++k)
		{
			explodeSide( gameMap, i + k, j + k, make_pair(1, 2) );//SE
			explodeSide( gameMap, i + k, j - k, make_pair(1, 3) );//SW
			explodeSide( gameMap, i - k, j - k, make_pair(0, 3) );//NW
			explodeSide( gameMap, i - k, j + k, make_pair(0, 2) );//NE
		}
	}
}

void Bomb::explodeSide( vector<vector<GameObject*>> & gameMap, int i, int j, pair<int, int> p )
{
	explodeSide( gameMap, i, j, p.first );
	explodeSide( gameMap, i, j, p.second );
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
		if ( abs(i + 1) < gameMap.size() && i > 0 && abs(j + 1) < gameMap[0].size() && j > 0 )
		{
			if ( gameMap[i][j]->getHP() <= damage && gameMap[i][j]->getType() != 2 )
			{
				char w = gameMap[i][j]->getMapRep();
				delete gameMap[i][j];
				gameMap[i][j] = new Explosion( w );
				
			}
			else 
			{ 
				if ( gameMap[i][j]->isDestructable() ) gameMap[i][j]->decreaseHP( damage ); 
				break; 
			}
		}
		i += x;
		j += y;
	}
}

pair<int, int> Bomb::getPos () const { return mapPos; }
void Bomb::setPos ( int i, int j ) { mapPos = make_pair ( i, j ); }

void Bomb::upgrade () 
{ 
	damage++;
	expRadius++;
	expWidth += 2;
	if ( expWidth > 3 ) expWidth = 3;
	detonationTime -= 0.2;
	if ( detonationTime < 1 ) detonationTime = 1;
	//update mapRep
}

Bomb * Bomb::clone () const { return new Bomb ( *this ); }