#ifndef BOMB_H
#define BOMB_H
#include "GameObject.h"

class Bomb : public GameObject
{
	public:
		Bomb ();

	protected:
		int expRadius, detonationTime, damage;
};

#endif