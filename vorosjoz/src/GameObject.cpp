#include "GameObject.h"

GameObject::GameObject () { score = 0;}
GameObject :: ~GameObject () {}
GameObject::GameObject ( int hp, char r ) : hp ( hp ), mapRep( r ) {}
char GameObject::getMapRep  () { update(); return mapRep; }
void GameObject::setMapRep  (char c) { mapRep = c;        }
int  GameObject::getHP      () const { return hp; }
void GameObject::mapDestroy () { mapRep = ' '; }
int  GameObject::getType    () const { return type; }
void GameObject::update () {}
void GameObject::decreaseHP ( int amount ) { hp -= amount; }
GameObject* GameObject::clone() const { return new GameObject ( *this ); }
void GameObject::setHP ( int nhp ) { hp = nhp; }
bool GameObject::isDestructable() const { return true; }
long long GameObject::getScore () const { return score; }