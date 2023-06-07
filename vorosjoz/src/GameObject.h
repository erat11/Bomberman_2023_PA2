#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <utility>

using namespace std;

class GameObject
{
	public: 
		GameObject ();
		GameObject ( int hp, char r );
		virtual ~GameObject ();
		char getMapRep        ();
		virtual void update ();
		void setMapRep  (char c);	
		virtual int getHP     () const;
		int getType			  () const;
		virtual long long getScore () const;
		virtual void mapDestroy ();
		virtual void decreaseHP ( int amount );
		virtual GameObject * clone() const;
		virtual void setHP ( int nhp );
		virtual bool isDestructable() const;
	protected:
		int hp, type;
		long long score; // type: bomb = 0, player = 2, wall = {6, 7}, buff = 8, 
		char mapRep;
};

#endif