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
		if ( s2 == "BOT" ) player2 = AI ( sizeX - 2, sizeY - 2, hp, 'Q', s2 );
		else player2 = Player ( sizeX - 2, sizeY - 2, hp, 'Q', s2 );
		gameMap[1][1] = player1;
		gameMap[sizeX - 2][sizeY - 2] = player2;
		cbreak();
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
	checkSize();
	clear();
	
	mapPrint();
	playerPrint();

	refresh();
	getch();
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

void GameMap::playerPrint( )
{
	writeX = 10;
	writeY = ( maxY - sizeY ) / 10;
	mvprintw( writeX, writeY, "Player1: (" );
	writeY += 10;
	mvprintw( writeX, writeY++, "%c", player1.getMapRep() );
	mvprintw( writeX++, writeY, ") " );
	writeY = ( maxY - sizeY ) / 10;
	mvprintw( writeX, writeY, player1.getName().c_str() );
	writeY += player1.getName().size() + 2;
	printHearts ( writeX, writeY, player1.getHP() );
	//todo bombstack
	//todo score

	writeX = maxX - 10;
	writeY = ( ( maxY ) / 10 ) * 8;
	mvprintw( writeX, writeY, "Player2: (" );
	writeY += 10;
	mvprintw( writeX, writeY++, "%c", player2.getMapRep() );
	mvprintw( writeX++, writeY, ") " );
	writeY = ( ( maxY ) / 10 ) * 8;
	mvprintw( writeX, writeY, player2.getName().c_str() );
	writeY += player2.getName().size() + 2;
	printHearts ( writeX, writeY, player2.getHP() );
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
	char key = getch();

    // Check if 'a', 'w', 's', or 'd' were pressed
    int i = player1.getPos().first, j = player1.getPos().second;
    if ( key == 'a' && !gameMap[i][j - 1].getHP() )
    {
    	player1.setPos( i, j - 1 );
    	swapper ( i, j, i, j - 1, 1 );
    }
    if ( key == 'd' && !gameMap[i][j + 1].getHP() )
    {
    	player1.setPos( i, j + 1 );
    	swapper ( i, j, i, j + 1, 1 );
    }
    if ( key == 's' && !gameMap[i + 1][j].getHP() )
    {
    	player1.setPos( i + 1, j );
    	swapper ( i + 1, j, i, j, 1 );
    }
    if ( key == 'w' && !gameMap[i - 1][j].getHP() )
    {
    	player1.setPos( i - 1, j );
    	swapper ( i - 1, j, i, j, 1 );
    }
}

void GameMap::swapper ( int a, int b, int c, int d, bool player )
{
	if ( player ) gameMap[a][b] = player1;
	else 	      gameMap[a][b] = player2;
	gameMap[c][d] = Wall ( Wall ( 0, ' ' ) );
}