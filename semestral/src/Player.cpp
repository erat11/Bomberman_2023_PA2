#include "Player.h"

Player::Player () : GameObject() { }
Player::Player ( int x, int y, int hp, char r, const string & name ) : GameObject ( hp, r ) 
{
	type = 2;
	mapPos = make_pair ( x, y );
	this->name = name;
	this->hp = hp;
	isBot = false;
	initBombStack();
	score = 0;
}
Player::~Player ()
{
	while ( ! bombs.empty() )
	{
		auto b = bombs.front();
		delete b;
		bombs.pop();
	}
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
queue<Bomb*> Player::getBombStack() const { return bombs; }

void Player::placeBomb( vector<vector<GameObject*>> & gameMap )
{
	Bomb * b = bombs.front();
	auto x = getDirectedPos();
	if ( gameMap[x.first][x.second]->getMapRep() == ' ' )
	{
		delete gameMap[x.first][x.second];
		gameMap[x.first][x.second] = b;
	}
	b->setPos( getDirectedPos().first, getDirectedPos().second );
	b->lightBomb();

	bombs.pop();
	bombs.push ( new Bomb () );
}

void Player::initBombStack ()
{
	for ( int i = 0; i < 5; ++i )
		bombs.push ( new Bomb () );
}

pair<int, int> Player::getDirectedPos() const
{
	switch( move.direction() )
	{
		case( 0 ): return make_pair( mapPos.first - 1, mapPos.second );
		case( 1 ): return make_pair( mapPos.first + 1, mapPos.second );
		case( 2 ): return make_pair( mapPos.first, mapPos.second - 1 );
		default: return make_pair( mapPos.first, mapPos.second + 1 );
	}
}

void Player::setHP ( int nhp ) { hp = nhp; }

