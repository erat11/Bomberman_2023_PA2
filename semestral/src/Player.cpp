#include "Player.h"

Player::Player () : GameObject() { }
Player::Player ( int x, int y, int hp, char r, const string & name ) : GameObject ( hp, r ) 
{
	mapPos = make_pair ( x, y );
	this->name = name;
	this->hp = hp;
	isBot = false;
	initBombStack();
	score = 0;
}
/*Player::Player ( const Player & p )
{
	Moveset m = p.getMoveset();
	move = Moveset ( m );
	mapPos = make_pair ( p.getPos().first, p.getPos().second );
	speed = p.getSpeed(); hp = p.getHP(); score = p.getScore(); 
	mapRep = p.getMapRep();
	name = p.getName();
	isBot = p.bot();
}
Player & Player::operator = ( const Player & p )
{
	Moveset m = p.getMoveset();
	move = Moveset ( m );
	mapPos = make_pair ( p.getPos().first, p.getPos().second );
	speed = p.getSpeed(); hp = p.getHP(); score = p.getScore(); 
	mapRep = p.getMapRep();
	name = p.getName();
	isBot = p.bot();
	return *this;
}*/
int    Player::getSpeed () const { return speed; }
int    Player::getHP    () const { return hp;    }
int    Player::getScore () const { return score; }
string Player::getName  () const { return name;  }
void Player::setPos ( int i, int j ) { mapPos = make_pair ( i, j ); }
pair<int, int> Player::getPos () const { return mapPos; }
bool Player::bot() const { return isBot; }
void Player::setMoveset( char a, char b, char c, char d, char e ) { move = Moveset ( a, b, c, d, e ); }
bool Player::hasInMoveset ( char k ) { return move.contain( k ); }
Moveset Player::getMoveset () const { return move; }
queue<Bomb> Player::getBombStack() const { return bombs; }

void Player::PlaceBomb()
{
	bombs.pop();
	bombs.push ( Bomb () );
}

void Player::initBombStack ()
{
	for ( int i = 0; i < 5; ++i )
		bombs.push ( Bomb () );
}