#ifndef BUFF_H
#define BUFF_H

#include "GameObject.h"
#include <string>
#include "Bomb.h"

struct PlayerAttributes
{
	pair<int, int> mapPos;
	int speed, hp, score;
	Bomb defaultBomb;
};


class Buff : public GameObject
{
	public:
		Buff ();
		virtual void activate ( PlayerAttributes & plAttr );
		virtual ~Buff() override;
		string getName() const;
		Buff * clone () const override;
	protected:
		int duration;
		string name;
};

#endif