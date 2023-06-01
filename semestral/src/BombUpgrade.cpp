#include "Buff.h"

class BombUpgrade : public Buff
{
	public:
		BombUpgrade () : Buff () { name = "Bomb Upgrade"; }
		virtual void activate ( PlayerAttributes & plAttr ) override
		{
			plAttr.defaultBomb.upgrade();
		}
};
