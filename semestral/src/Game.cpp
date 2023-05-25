#include "Game.h"

using namespace std;

Game::Game ()
{
	Run();
}

void Game::Run ()
{
	vector<string> boxNames = { "BOMBERMAN 2023", "Start New Game", "Settings", "Leaderboard", "Quit" };
	auto stScreen = StartScreen ( boxNames );
	stScreen.print();
	stScreen.handleInput();
	if ( stScreen.get() == "Quit"           ) return;
	if ( stScreen.get() == "Start New Game" ) StartGame();
	else if ( stScreen.get() == "Settings" ) 
	{
		boxNames = { "Settings", "Drop Chance Percent", "Dollar Chance Percent",
		             "Debuffs On/Off", "Baloons On/Off", "HP count", "Choose Map", "Save & Exit", "Reset Default & Exit" };
		auto seScreen = SettingsScreen ( boxNames );
		seScreen.load();
		seScreen.handleInput();
		Run();
	}
}

void Game::StartGame()
{
	vector<string> boxNames = { "STARTING A NEW GAME OF BOMBERMAN 2023", "Player vs Player", "Player vs Bot", "Back" };
	auto gnScreen = StartScreen ( boxNames );
	gnScreen.print();
	gnScreen.handleInput();
	if ( gnScreen.get() == "Back" ) Run();
	else if ( gnScreen.get() == "Player vs Player" ) GameLoop ( 1 );
	else if ( gnScreen.get() == "Player vs Bot"    ) GameLoop ( 0 );
}

void Game::GameLoop( bool p )
{
	auto seScreen = SettingsScreen ();
	seScreen.load();
	auto NameScreen1 = GetNameScreen ( "TYPE NAME FOR PLAYER1" );
	playMap = GameMap ( "GAME" );
	if ( p )
	{
		auto NameScreen2 = GetNameScreen ( "TYPE NAME FOR PLAYER2" );
		if ( !playMap.load( seScreen.getHP(), NameScreen1.get(), NameScreen2.get() ) ) return;
	}
	else { if ( !playMap.load( seScreen.getHP(), NameScreen1.get(), "BOT"s ) ) return; }
	
	playMap.addDollars( seScreen.getDollarChance() );

	//loop
	playMap.handleInput();
}

Game::~Game () {}