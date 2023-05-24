#include "Player.h"

Player::Player () : GameObject() { }
Player::Player ( int x, int y, int hp, char r, const string & name ) : GameObject ( hp, r ) 
{
	mapPos = make_pair ( x, y );
	this->name = name;
	this->hp = hp;
}
int    Player::getSpeed () { return speed; }
int    Player::getHP    () { return hp;    }
int    Player::getScore () { return score; }
string Player::getName  () { return name;  }
void Player::setPos ( int x, int y ) { mapPos = make_pair( x, y ); }
pair<int, int> Player::getPos () const { return mapPos; }