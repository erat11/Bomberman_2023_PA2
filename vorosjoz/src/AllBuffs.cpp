#include "Buff.h"

class BombUpgrade : public Buff
{
	public:
		BombUpgrade ( bool x ) : Buff () 
		{ 
			if ( x )
			{
				name = "Bomb Upgrade"; 
				debuff = false;
			}
			else
			{
				name = "Bomb Downgrade";
				debuff = true;
			}

		}
		virtual void activate ( PlayerAttributes & plAttr, vector<vector<GameObject*>> & gameMap ) override
		{
			if ( ! debuff )
			{
				plAttr.defaultBomb.upgrade( 1 );
				plAttr.score += 1000 * plAttr.multiplier;
			}
			else plAttr.defaultBomb.upgrade( -1 );
		}
	private:
	bool debuff;
};

class ScoreMultiplier : public Buff
{
	public:
		ScoreMultiplier ( bool x ) : Buff () 
		{ 
			if ( x )
			{
				name = "Score Multiplier x2"; 
				debuff = false;
			}
			else
			{
				name = "Score Debuff"; 
				debuff = true;
			}
		}
		virtual void activate ( PlayerAttributes & plAttr, vector<vector<GameObject*>> & gameMap ) override
		{
			if ( !debuff ) plAttr.multiplier *= 2;
			else if ( plAttr.multiplier > 1 ) plAttr.multiplier /= 2;
		}
	private:
		bool debuff;
};

class ScoreBonus : public Buff
{
	public:
		ScoreBonus () : Buff () { name = "Score Bonus"; }
		virtual void activate ( PlayerAttributes & plAttr, vector<vector<GameObject*>> & gameMap ) override
		{
			plAttr.score += 25000 * plAttr.multiplier;
		}
};

class GainHP : public Buff
{
	public:
		GainHP ( bool x ) : Buff () 
		{ 
			if ( x )
			{
				debuff = false;
				name = "Gain HP"; 
			}
			else
			{
				debuff = true;
				name = "Loose HP Debuff"; 
			}
		}
		virtual void activate ( PlayerAttributes & plAttr, vector<vector<GameObject*>> & gameMap ) override
		{
			if ( !debuff )
			{
				gameMap[plAttr.mapPos.first][plAttr.mapPos.second]->decreaseHP( -1 );
				plAttr.score += 1000 * plAttr.multiplier;
			}
			else gameMap[plAttr.mapPos.first][plAttr.mapPos.second]->decreaseHP( 1 );
		}
	private:
		bool debuff;
};

class TimeBasedBuff : public Buff
{
	public:
		TimeBasedBuff ( int i ) : Buff () 
		{ 
			buffType = i;
			switch(i)
			{
				default:{ name = "Immortality"; duration = 4; break; }
				case(1):{ name = "Wall Hack"; duration = 2; break; }
			}
		}
		virtual void activate ( PlayerAttributes & plAttr, vector<vector<GameObject*>> & gameMap ) override
		{
			plAttr.score += 1000;
			plAttr.buffActive = true;
			start = chrono::high_resolution_clock::now();
			plAttr.destructable = false;
			switch(buffType)
			{
				default:{ plAttr.destructable = false; break; }
				case(1):{ plAttr.wallHack = true; break; }
			}
		}
		bool done () const override
		{
			return timer() > duration;
		}
		bool isActive() const { return !done(); }
		double timer () const override
		{
			auto now = chrono::high_resolution_clock::now();
			return chrono::duration_cast<chrono::seconds>(now - start).count();
		}
	private:
		chrono::system_clock::time_point start;
		int buffType;
};

class Explode : public Buff
{
	public:
		Explode () : Buff () { name = "Boom"; }
		virtual void activate ( PlayerAttributes & plAttr, vector<vector<GameObject*>> & gameMap ) override
		{
			plAttr.score += 5000;
			for ( int i = -3; i < 4; ++i  )
				for (int j = -3; j < 4; ++j)
					if ( plAttr.mapPos.first + i > 0 && abs(plAttr.mapPos.first + i) < gameMap.size() &&
						 plAttr.mapPos.second + j > 0 && abs(plAttr.mapPos.second  + j) < gameMap[0].size() &&
						 ( i != 0 || j != 0 ) )
					{
						char mR = gameMap[plAttr.mapPos.first + i][plAttr.mapPos.second + j]->getMapRep();
						if ( gameMap[plAttr.mapPos.first + i][plAttr.mapPos.second + j]->getType() == 2 )
							gameMap[plAttr.mapPos.first + i][plAttr.mapPos.second + j]->decreaseHP(1);
						else if ( mR != '|' && mR != '-' )
						{
							delete gameMap[plAttr.mapPos.first + i][plAttr.mapPos.second + j];
							if ( rand() % 10 == 1 && ( mR == '~' || mR == '$' || mR == '#' ) ) 
								gameMap[plAttr.mapPos.first + i][plAttr.mapPos.second + j] = new Explosion( '$' );
							else 
								gameMap[plAttr.mapPos.first + i][plAttr.mapPos.second + j] = new Explosion( ' ' );
						}
					}
				
		}
};