#include "GameMap.h"

int Wall::dropChance = 0;

GameMap::GameMap () : Interface () {}

GameMap::GameMap ( const char * caption ) : Interface ( caption )
{

}

GameMap::~GameMap()
{
	for ( auto & x : gameMap )
		for ( GameObject * y : x )
			delete y;
}

bool GameMap::load ( int hp, const string & s1, const string & s2 )
{
	ifstream file ( PATH_TO_MAP_FILE );
	if ( file.is_open () ) 
	{
		string line;
		if ( ! getline ( file, line ) || !checkFormat ( line, "X=(\\d+)", sizeX ) || 
		   ( ! getline ( file, line ) || !checkFormat ( line, "Y=(\\d+)", sizeY ) ) )
		{
			cout << "Error reading config file, exiting...\n" << endl;
			return false;
		}
		startY = ( maxY - sizeY ) / 2;
		startX = ( maxX - sizeX ) / 2;
		int i = 0, j;
		while ( getline ( file, line ) )
		{
			j = 0;
			vector<GameObject*> tmp;
			for ( auto & z : line )
			{
				Wall * x = new Wall ( z );
				tmp.push_back( x );
				j++;
			}
			i++;
			gameMap.push_back ( tmp );
		}
		file.close();
		//todo verify map integrity
		player1 = new Player ( 1, 1, hp, 'P', s1 );
		player1->setMoveset( 'w', 's', 'a', 'd', ' ', 'b' );
		if ( s2 == "BOT" ) player2 = new AI ( sizeX - 2, sizeY - 2, hp, 'Q', s2 );
		else player2 = new Player ( sizeX - 2, sizeY - 2, hp, 'Q', s2 );
		player2->setMoveset( '8', '5', '4', '6', '+', '-' );
		delete gameMap[1][1];
		delete gameMap[sizeX - 2][sizeY - 2];
		gameMap[1][1] = player1;
		gameMap[sizeX - 2][sizeY - 2] = player2;
		nodelay(stdscr, true);
		return true;
	}
	cout << "Error while opening config file, exiting...\n";
	return false;
}

void GameMap::addDollars ( int chance )
{
	for ( unsigned int i = 0; i < gameMap.size(); ++i )
	{
		for ( unsigned int j = 0; j < gameMap[i].size(); ++j )
			if ( gameMap[i][j]->getMapRep() == '~' && ( rand() % 100 ) <= chance )
			{
				delete gameMap[i][j];
				gameMap[i][j] = new Wall ( '$' );
			}
	} 
}

void GameMap::print ()
{
	clear();
	mapPrint();
	playerInfoPrint( player1, maxX/10, ( maxY - sizeY ) / 10) ;
	playerInfoPrint( player2, maxX - maxX*3/10, ( ( maxY ) / 10 ) * 8 );
	refresh();
}

void GameMap::handleInput () 
{
	char key = '!';
	while ( 1 )
	{
		key = getch();
		if ( player1->hasInMoveset( key ) )                   handlePlayer ( key, player1 );
		else if ( player2->hasInMoveset( key ) && player2->bot() ) handlePlayer ( key, player2 );
		print();
		player1->sync( gameMap );
		player2->sync( gameMap );
		if ( key == '\n' ) break;
		if ( player1->getHP() <= 0 || player2->getHP() <= 0 )
		{
			napms(1000);
			string winner;
			long long winnerScore = 0;
			if ( player1->getHP() <= 0 ) { winner = player2->getName(); winnerScore = player2->getScore(); }
			else { winner = player1->getName();	winnerScore = player1->getScore(); }		
			clear();
			refresh();
			writeX = maxX / 2;
			writeY = ( maxY - winner.size() - 5 ) / 2;
			mvprintw( writeX, writeY, winner.c_str() );
			writeY += winner.size() + 1;
			mvprintw( writeX, writeY, "WINS!" );
			napms(2500);
			refresh();
			Leaderboard l = Leaderboard ();
			l.save( winner, winnerScore );
			return;
		}
	}
}

int GameMap::getSizeX() { return sizeX; }

int GameMap::getSizeY() { return sizeY; }

void GameMap::mapPrint ()	
{
	writeY = ( maxY - sizeY ) / 2;
	writeX = ( maxX - sizeX ) / 2;
	for ( unsigned int i = 0; i < gameMap.size(); ++i )
	{
		for ( unsigned int j = 0; j < gameMap[0].size(); ++j )
		{
			if ( gameMap[i][j]->getType() == 1 )
			{
				Bomb * z = dynamic_cast<Bomb *> ( gameMap[i][j] )->clone();
				if ( z->ready() ) 
				{
					z->detonate( gameMap );

					if ( !z->getPlayer() ) player1->addScore( z->getScore() * player1->getMultiplier() );
					else player2->addScore( z->getScore() * player2->getMultiplier() );

					delete gameMap[z->getPos().first][z->getPos().second];
					gameMap[z->getPos().first][z->getPos().second] = new Explosion( ' ' );
				}
				delete z;
			}
			else if ( gameMap[i][j]->getType() == 10 )
			{
				if ( gameMap[i][j]->getMapRep() == ' ' ) { delete gameMap[i][j]; gameMap[i][j] = new Wall(); }
				else if ( gameMap[i][j]->getMapRep() == '!' ) { delete gameMap[i][j]; gameMap[i][j] = new Buff(); }
			}
			mvprintw( writeX, writeY++, "%c", gameMap[i][j]->getMapRep() );
		}
		
		
		
		writeY = ( maxY - sizeY ) / 2;
		writeX++;
	} 
}

void GameMap::playerInfoPrint( Player * p, int writeX, int writeY )
{
	int wY = writeY;
	mvprintw( writeX, writeY, "Player: (" );
	writeY += 9;
	mvprintw( writeX, writeY++, "%c", p->getMapRep() );
	mvprintw( writeX++, writeY, ") " );
	writeY = wY;
	mvprintw( writeX, writeY, p->getName().c_str() );
	writeY += p->getName().size() + 2;
	printHearts ( writeX, writeY, p->getHP() );
	writeY = wY;writeX++;
	mvprintw( writeX, writeY, "Bombstack " );
	writeY += 10;
	queue<Bomb*> stackTmp = p->getBombStack();
	while ( !stackTmp.empty() )
	{
		mvprintw( writeX, writeY, "%c ", stackTmp.front()->getMapRep() );
		stackTmp.pop();
		writeY += 2;
	}
	writeY = wY;writeX++;
	mvprintw( writeX, writeY, "Score: " );
	writeY += 10;
	mvprintw( writeX, writeY, "%d", p->getScore() );
	writeX++;writeY = wY;
	mvprintw( writeX, writeY, "CURRENT BUFF: " );
	writeY += 16;
	if ( !p->getBuffStack().size() ) mvprintw( writeX, writeY, "none" );
	else mvprintw( writeX, writeY, p->getBuffStack().front()->getName().c_str() );
	writeX++;writeY = wY;
	mvprintw( writeX, writeY, "BUFFS: " );
	writeY += 10;
	mvprintw( writeX, writeY, "%d", p->getBuffStack().size() );
	queue<Buff*> stackTmp2 = p->getBuffStack();
	writeY = wY;
	while ( !stackTmp2.empty() )
	{
		writeX++;
		mvprintw( writeX, writeY, stackTmp2.front()->getName().c_str() );
		stackTmp2.pop();
	}
	writeX++;writeY = wY;
	if ( p->getBuffActive() ) mvprintw( writeX, writeY, "%.1f", p->getBuffStack().front()->getDuration() - 
		p->getBuffStack().front()->timer()) ;

}

void GameMap::printHearts ( int wx, int wy, int hp )
{
	for (int i = 0; i < hp; ++i)
	{
		 mvaddwstr(wx, wy, L"\u2665 ");
		 wy += 2;
	}

	
}

void GameMap::swapper ( int a, int b, int c, int d, Player * player )
{
	if ( gameMap[c][d]->getMapRep() != '|' && gameMap[c][d]->getMapRep() != '-' && gameMap[c][d]->getType() != 2 )
	{
		player->setPos( c, d );
		if ( gameMap[c][d]->getType() == 8 ) player->addBuff( pickRandomBuff() );
		delete gameMap[c][d];
		gameMap[c][d] = player;
		gameMap[a][b] = new Wall ( ' ' );
	}
}

void GameMap::handlePlayer ( char key, Player * player )
{
	int i = player->getPos().first, j = player->getPos().second;
	if      ( player->getMoveset().left() == key  && ( isPosEmpty( i, j - 1 ) || player->wallHack() ) ) swapper ( i, j, i, j - 1, player );
	else if ( player->getMoveset().right() == key && ( isPosEmpty( i, j + 1 ) || player->wallHack() ) ) swapper ( i, j, i, j + 1, player );
	else if ( player->getMoveset().down() == key  && ( isPosEmpty( i + 1, j ) || player->wallHack() ) ) swapper ( i, j, i + 1, j, player );
	else if ( player->getMoveset().up() == key    && ( isPosEmpty( i - 1, j ) || player->wallHack() ) ) swapper ( i, j, i - 1, j, player );
	else if ( player->getMoveset().bomb() == key ) player->placeBomb( gameMap );
	else if ( player->getMoveset().buff() == key && !player->getBuffActive() ) player->activateBuff( gameMap );
}

bool GameMap::isPosEmpty( int i, int j )
{
	return gameMap[i][j]->getMapRep() == ' ' || gameMap[i][j]->getType() == 8;
}

void GameMap::setDropChance( int chance )
{
	Wall w;
	w.setDropChance( chance );
}

Buff * GameMap::pickRandomBuff()
{
	int q = rand() % 8;
	switch(q)
	{
		case(0): return new GainHP();
		case(1): return new TimeBasedBuff(0);
		case(2): return new TimeBasedBuff(1);
		case(3): return new Explode();
		case(4): return new ScoreMultiplier();
		case(5): return new BombUpgrade();
		default: return new ScoreBonus();
	}
}