#ifndef BUFF_H
#define BUFF_H

#include "GameObject.h"
#include <string>
#include "Bomb.h"

struct PlayerAttributes
{
	//encapsulates player attributes, so the modification through buffs is easier
	pair<int, int> mapPos;
	int speed, hp;
	long long score;
	Bomb defaultBomb;
	bool buffActive = false;
	bool destructable = true;
	bool wallHack = false;
	int multiplier = 1;
};


class Buff : public GameObject
{
	public:
		Buff ();
		virtual void activate ( PlayerAttributes & plAttr, vector<vector<GameObject*>> & gameMap );
		virtual ~Buff() override;
		string getName() const;
		Buff * clone () const override;
		virtual bool done() const;
		virtual bool isActive() const;
		virtual double timer() const;
		double getDuration() const;
	protected:
		int duration;
		string name;
};

#endif