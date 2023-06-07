#include "GameMap.h"

int Wall::dropChance = 0;

GameMap::GameMap () : Interface () {}

GameMap::GameMap ( const char * caption ) : Interface ( caption ) {}

GameMap::~GameMap()
{
	for ( auto & x : gameMap )
		for ( GameObject * y : x )
			delete y;
}

bool GameMap::load ( const string & s1, const string & s2, GameSettingsWrapper gsw, BuffChancesWrapper bcw )
{
	this->gsw = gsw;
	this->bcw = bcw;
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
				tmp.push_back( new Wall ( z ) );
				j++;
			}
			i++;
			gameMap.push_back ( tmp );
		}
		file.close();
		player1 = new Player ( 1, 1, gsw.hp, 'P', s1 );
		player1->setMoveset( 'w', 's', 'a', 'd', ' ', 'b' );
		if ( s2 == "BOT" ) player2 = new AI ( sizeX - 2, sizeY - 2, gsw.hp, 'Q', s2 );
		else player2 = new Player ( sizeX - 2, sizeY - 2, gsw.hp, 'Q', s2 );
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
	//adds dolar walls randomly to the map
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
		else if ( player2->hasInMoveset( key ) && !player2->bot() ) handlePlayer ( key, player2 );
		print();
		player1->sync( gameMap );
		if ( player2->bot() && player2->getMove() ) handleAI();
		player2->sync( gameMap );
		if ( key == '\n' ) break;
		if ( player1->getHP() <= 0 || player2->getHP() <= 0 )
		{
			winScreen();
			return;
		}
	}
}

void GameMap::winScreen( )
{
	//happens after a player wins the game
	napms(1500);
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
	refresh();
	napms(2500);//show winners name for 2.5s
	Leaderboard l = Leaderboard ();
	l.save( winner, winnerScore );
}

void GameMap::handleAI()
{
	player2->activateBuff( gameMap ); 
	player2->generateMove( gameMap );
	if ( player2->mover != -1 ) 
	{ 
		if ( player2->mover <= 3 )
		{
			int a = 0, b = 0;
			if ( player2->mover == 2 ) player2->setDirection( 3 );
			else if (  player2->mover == 3 )player2->setDirection( 2 );
			else player2->setDirection( player2->mover );
			switch( player2->mover )
			{
				// 0 = NORTH, 1 = SOUTH, 2 = EAST, 3 = WEST
				case(0):{ a = -1; break; }
				case(1):{ a =  1; break; }
				case(2):{ b =  1; break; }
				default:{ b = -1; break; }
			}
			int i = player2->getPos().first, j = player2->getPos().second;
			if ( isPosEmpty ( i + a, j + b ) )
				swapper( i, j, i + a, j + b, player2 ); 
		}
		else if ( player2->mover == 4 ) { if ( !player2->getBuffActive() ) player2->activateBuff( gameMap ); }
		else if ( player2->mover == 5 ) player2->placeBomb( gameMap );
		else if ( player2->mover == 6 ) player2->freeze ( );
		player2->mover = -1;
	}
	player2->resetMove(); 
}

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
				if ( z->ready( gameMap ) ) 
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
	mvprintw( writeX, writeY, "Score: " );
	writeY += 10;
	mvprintw( writeX, writeY, "%d", p->getScore() );
	writeY = wY;writeX++;
	if ( !p->bot() )
	{
		mvprintw( writeX, writeY, "Bomb Level: " );
		writeY += 18;
		mvprintw( writeX, writeY, "%d", p->getBombLevel() );
		writeX++;writeY = wY;
		mvprintw( writeX, writeY, "CURRENT BUFF: " );
		writeY += 16;
		if ( !p->getBuffStack().size() ) mvprintw( writeX, writeY, "none" );
		else mvprintw( writeX, writeY, p->getBuffStack().front()->getName().c_str() );
		writeX++;writeY = wY;
		if ( p->getBuffActive() ) mvprintw( writeX, writeY, "%.1f", p->getBuffStack().front()->getDuration() - 
			p->getBuffStack().front()->timer()) ;
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
	}
	else mvprintw( writeX, writeY, "%d", player2->p );


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
	bool buff = true;
	if ( gsw.debuffs && rand() % 5 == 0 ) buff = false;
	int sum = bcw.gainHp + bcw.bombUpgrade + bcw.scoreMultiplier + bcw.scoreBonus + bcw.immortality + bcw.wallhack + bcw.explode;
	BuffChancesWrapper bcwTMP = bcw;
	bcwTMP.bombUpgrade += bcwTMP.gainHp;
	bcwTMP.scoreMultiplier += bcwTMP.bombUpgrade;
	bcwTMP.scoreBonus += bcwTMP.scoreMultiplier;
	bcwTMP.immortality += bcwTMP.scoreBonus;
	bcwTMP.wallhack += bcwTMP.immortality;
	bcwTMP.explode += bcwTMP.wallhack;
	int q = rand() % sum;
	if ( q < bcwTMP.gainHp ) return new GainHP ( buff );
	if ( q < bcwTMP.bombUpgrade ) return new BombUpgrade ( buff );
	if ( q < bcwTMP.scoreMultiplier ) return new ScoreMultiplier ( buff );
	if ( q < bcwTMP.scoreBonus ) return new ScoreBonus ();
	if ( q < bcwTMP.immortality ) return new TimeBasedBuff ( 0 );
	if ( q < bcwTMP.wallhack ) return new TimeBasedBuff ( 1 );
	else return new Explode ();
}