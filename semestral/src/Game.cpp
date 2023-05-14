#include "Game.h"

using namespace std;

Game::Game ()
{
	Run();
}

void Game::Run ()
{
	vector<string> boxNames = { "BOMBERMAN 2023", "Start New Game", "Settings", "Leaderboard", "Quit" };
	auto sScreen = StartScreen ( boxNames );
	if ( sScreen.get() == "Quit"           ) return;
	if ( sScreen.get() == "Start New Game" ) StartGame();
	else cout << sScreen.get() << endl;
}

void Game::StartGame()
{
	vector<string> boxNames = { "STARTING A NEW GAME OF BOMBERMAN 2023", "Player vs Player", "Player vs Bot", "Back" };
	auto startGameScreen = StartScreen ( boxNames );
	if ( startGameScreen.get() == "Back"             ) Run();
	if ( startGameScreen.get() == "Player vs Player" ) GameLoop ( 1 );
	if ( startGameScreen.get() == "Player vs Bot"    ) GameLoop ( 0 );
}

int Game::GameLoop( bool p )
{
	auto NameScreen1 = GetNameScreen ( "TYPE NAME FOR PLAYER1" );
	player1.setName( NameScreen1.get() );
	if ( p )
	{
		auto NameScreen2 = GetNameScreen ( "TYPE NAME FOR PLAYER2" );
		player2.setName( NameScreen2.get() );
	}
	else player2.initBot();
}

Game::~Game () {}