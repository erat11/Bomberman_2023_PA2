#include "GameObject.h"

GameObject::GameObject () {}
GameObject :: ~GameObject () {}
GameObject::GameObject ( int hp, char r ) : hp ( hp ), mapRep( r ) {}
char GameObject::getMapRep  () { update(); return mapRep; }
void GameObject::setMapRep  (char c) { mapRep = c;        }
int  GameObject::getHP      () const { return hp; }
void GameObject::mapDestroy () { mapRep = ' '; }
int  GameObject::getType    () const { return type; }
void GameObject::update () {}
void GameObject::decreaseHP ( int amount ) { hp -= amount; }