#include "GameMap.h"

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
		player1->setMoveset( 'w', 's', 'a', 'd', ' ' );
		if ( s2 == "BOT" ) player2 = new AI ( sizeX - 2, sizeY - 2, hp, 'Q', s2 );
		else player2 = new Player ( sizeX - 2, sizeY - 2, hp, 'Q', s2 );
		player2->setMoveset( '8', '5', '4', '6', '+' );
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
	playerInfoPrint( player1, 10, ( maxY - sizeY ) / 10);
	playerInfoPrint( player2, maxX - 10, ( ( maxY ) / 10 ) * 8 );
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
		player1->setHP( gameMap[player1->getPos().first][player1->getPos().second]->getHP() );
		if ( key == '\n' ) break;
		if ( player1->getHP() < 0 || player2->getHP() < 0 ) break;//winning screen
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
				Bomb * z = dynamic_cast<Bomb *> ( gameMap[i][j] );
				if ( z->ready() ) 
				{
					z->detonate( gameMap );
					delete gameMap[z->getPos().first][z->getPos().second];
					gameMap[z->getPos().first][z->getPos().second] = new Explosion();
				}
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
	writeY = wY;
	writeX++;
	mvprintw( writeX, writeY, "Bombstack " );
	writeY += 10;
	queue<Bomb*> stackTmp = p->getBombStack();
	while ( !stackTmp.empty() )
	{
		mvprintw( writeX, writeY, "%c ", stackTmp.front()->getMapRep() );
		stackTmp.pop();
		writeY += 2;
	}
	writeY = wY;
	writeX++;
	mvprintw( writeX, writeY, "Score: " );
	writeY += 10;
	mvprintw( writeX, writeY, "%d", p->getScore() );
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
	player->setPos( c, d );
	delete gameMap[c][d];
	gameMap[c][d] = player;
	gameMap[a][b] = new Wall ( ' ' );
}

void GameMap::handlePlayer ( char key, Player * player )
{
	int i = player->getPos().first, j = player->getPos().second;
	if      ( player->getMoveset().left() == key  && isPosEmpty( i, j - 1 ) ) swapper ( i, j, i, j - 1, player );
	else if ( player->getMoveset().right() == key && isPosEmpty( i, j + 1 ) ) swapper ( i, j, i, j + 1, player );
	else if ( player->getMoveset().down() == key  && isPosEmpty( i + 1, j ) ) swapper ( i, j, i + 1, j, player );
	else if ( player->getMoveset().up() == key    && isPosEmpty( i - 1, j ) ) swapper ( i, j, i - 1, j, player );
	else if ( player->getMoveset().bomb() == key ) player->placeBomb( gameMap );
}

bool GameMap::isPosEmpty( int i, int j )
{
	return gameMap[i][j]->getMapRep() == ' ';
}
