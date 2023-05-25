#include "GameObject.h"

GameObject::GameObject () {}
GameObject::GameObject ( int hp, char r ) : hp ( hp ), mapRep( r ) {}
char GameObject::getMapRep  () const { return mapRep;     }
void GameObject::setMapRep  (char c) { mapRep = c;        }
int  GameObject::getHP      () const { return hp; 	    }
void GameObject::mapDestroy () {}