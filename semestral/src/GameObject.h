#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <utility>

using namespace std;

class GameObject
{
	public: 
		GameObject ();
		GameObject ( int hp, char r );
		char getMapRep        () const;
		void setMapRep  (char c);	
		int getHP             () const;
	protected:
		int hp;
		char mapRep;
};

#endif