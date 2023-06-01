#include "Player.h"

Player::Player () : GameObject() { }
Player::Player ( int x, int y, int hp, char r, const string & name ) : GameObject ( hp, r ) 
{
	type = 2;
	this->name = name;
	isBot = false;
	initBombStack();
	attributes.hp = hp;
	attributes.mapPos = make_pair ( x, y );
	attributes.score = 0;
	attributes.defaultBomb = Bomb();
}
Player::~Player ()
{
	while ( ! bombs.empty() )
	{
		auto b = bombs.front();
		delete b;
		bombs.pop();
	}
	while ( ! buffs.empty() )
	{
		auto b = buffs.front();
		delete b;
		buffs.pop();
	}
}

int    Player::getSpeed () const { return attributes.speed; }
int    Player::getHP    () const { return attributes.hp;    }
int    Player::getScore () const { return attributes.score; }
string Player::getName  () const { return name;  }
void Player::setPos ( int i, int j ) { attributes.mapPos = make_pair ( i, j ); }
pair<int, int> Player::getPos () const { return attributes.mapPos; }
bool Player::bot() const { return isBot; }
void Player::setMoveset( char a, char b, char c, char d, char e, char f ) { move = Moveset ( a, b, c, d, e, f ); }
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
	bombs.push ( attributes.defaultBomb.clone() );
}

void Player::initBombStack ()
{
	for ( int i = 0; i < 5; ++i )
		bombs.push ( attributes.defaultBomb.clone() );
}

pair<int, int> Player::getDirectedPos() const
{
	switch( move.direction() )
	{
		case( 0 ): return make_pair( attributes.mapPos.first - 1, attributes.mapPos.second );
		case( 1 ): return make_pair( attributes.mapPos.first + 1, attributes.mapPos.second );
		case( 2 ): return make_pair( attributes.mapPos.first, attributes.mapPos.second - 1 );
		default: return make_pair( attributes.mapPos.first, attributes.mapPos.second + 1 );
	}
}

void Player::setHP ( int nhp ) { attributes.hp = nhp; }

queue<Buff*> Player::getBuffStack() const { return buffs; }

void Player::addBuff ( Buff * b )
{
	buffs.push( b );
}

void Player::activateBuff()
{
	if ( buffs.size() )
	{
		buffs.front()->activate( attributes );
		reloadBombStack();
		delete buffs.front();
		buffs.pop();
	}
}

Player * Player::clone () const { return new Player ( *this ); }

void Player::reloadBombStack ()
{
	while ( ! bombs.empty() )
	{
		auto b = bombs.front();
		delete b;
		bombs.pop();
	}
	initBombStack();
}