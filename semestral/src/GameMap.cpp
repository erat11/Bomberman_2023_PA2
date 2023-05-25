#include "GameMap.h"

GameMap::GameMap () : Interface () {}

GameMap::GameMap ( const char * caption ) : Interface ( caption )
{

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
			vector<GameObject> tmp;
			for ( auto & z : line )
			{
				tmp.push_back( Wall ( 1, z ) );
				j++;
			}
			i++;
			gameMap.push_back ( tmp );
		}
		file.close();
		//todo verify map integrity
		player1 = Player ( 1, 1, hp, 'P', s1 );
		player1.setMoveset( 'w', 's', 'a', 'd', ' ' );
		if ( s2 == "BOT" ) player2 = AI ( sizeX - 2, sizeY - 2, hp, 'Q', s2 );
		else player2 = Player ( sizeX - 2, sizeY - 2, hp, 'Q', s2 );
		player2.setMoveset( '8', '5', '4', '6', '+' );
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
			if ( gameMap[i][j].getMapRep() == '~' && ( rand() % 100 ) <= chance )
				gameMap[i][j].setMapRep('$');
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

int GameMap::getSizeX() { return sizeX; }

int GameMap::getSizeY() { return sizeY; }

void GameMap::mapPrint ()	
{
	writeY = ( maxY - sizeY ) / 2;
	writeX = ( maxX - sizeX ) / 2;
	for ( const auto & x : gameMap )
	{
		for ( const auto & y : x )
			mvprintw( writeX, writeY++, "%c", y.getMapRep() );
		
		writeY = ( maxY - sizeY ) / 2;
		writeX++;
	} 
}

void GameMap::playerInfoPrint( const Player & p, int writeX, int writeY )
{
	int wY = writeY;
	mvprintw( writeX, writeY, "Player: (" );
	writeY += 9;
	mvprintw( writeX, writeY++, "%c", p.getMapRep() );
	mvprintw( writeX++, writeY, ") " );
	writeY = wY;
	mvprintw( writeX, writeY, p.getName().c_str() );
	writeY += p.getName().size() + 2;
	printHearts ( writeX, writeY, p.getHP() );
	writeY = wY;
	writeX++;
	mvprintw( writeX, writeY, "Bombstack " );
	writeY += 10;
	queue<Bomb> stackTmp = p.getBombStack();
	while ( !stackTmp.empty() )
	{
		mvprintw( writeX, writeY, "%c ", stackTmp.front().getMapRep() );
		stackTmp.pop();
		writeY += 2;
	}
	writeY = wY;
	writeX++;
	mvprintw( writeX, writeY, "Score: " );
	writeY += 10;
	mvprintw( writeX, writeY, "%d", p.getScore() );
}

void GameMap::printHearts ( int wx, int wy, int hp )
{
	for (int i = 0; i < hp; ++i)
	{
		 mvaddwstr(wx, wy, L"\u2665 ");
		 wy += 2;
	}
	
}

void GameMap::handleInput () 
{
	char key = '!';
	while ( 1 )
	{
		print();
		key = getch();
		if ( player1.hasInMoveset( key ) )                   handlePlayer ( key, player1 );
		else if ( player2.hasInMoveset( key ) && !player2.bot() ) handlePlayer ( key, player2 );
		print();
		if ( key == '\n' ) break;
	}
}

void GameMap::swapper ( int a, int b, int c, int d, Player & player )
{
	player.setPos( c, d );
	gameMap[c][d] = player;
	gameMap[a][b] = Wall ( Wall ( 0, ' ' ) );
}

void GameMap::handlePlayer ( char key, Player & player )
{
	int i = player.getPos().first, j = player.getPos().second;
	if      ( player.getMoveset().left() == key  && gameMap[i][j - 1].getMapRep() == ' ' ) swapper ( i, j, i, j - 1, player );
	else if ( player.getMoveset().right() == key && gameMap[i][j + 1].getMapRep() == ' ' ) swapper ( i, j, i, j + 1, player );
	else if ( player.getMoveset().down() == key  && gameMap[i + 1][j].getMapRep() == ' ' ) swapper ( i, j, i + 1, j, player );
	else if ( player.getMoveset().up() == key    && gameMap[i - 1][j].getMapRep() == ' ' ) swapper ( i, j, i - 1, j, player );
	else if ( player.getMoveset().bomb() == key ) handleBombPlacement( player );
}

void GameMap::handleBombPlacement ( Player & player )
{
	
}

