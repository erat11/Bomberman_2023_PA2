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
		int getHP             () const;
		int getType			  () const;
		virtual void mapDestroy ();
		void decreaseHP ( int amount );
		virtual GameObject * clone() const;
	protected:
		int hp, type; // type: bomb = 0
		char mapRep;
};

#endif