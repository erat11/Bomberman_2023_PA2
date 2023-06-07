#include "StartScreen.h"
#include "GetNameScreen.cpp"
#include "Player.h"
#include "GameMap.h"
#include "SettingsScreen.cpp"
#include "MapChooseScreen.cpp"

class Game
{
public:
    Game() { Run(); }
    void Run ()
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
			             "Debuffs On/Off", "HP count", "Choose Map", "Save & Exit", "Reset Default & Exit" };
			auto seScreen = SettingsScreen ( boxNames );
			seScreen.load();
			seScreen.handleInput();
			if ( seScreen.getChoice() ) auto x = MapChooseScreen();
			Run();
		}
		else if ( stScreen.get() == "Leaderboard" )
		{
			auto leaderboard = Leaderboard( " Leaderboard ");
			Run();
		}
	}
    ~Game() {}
private:
    void StartGame()
	{
		vector<string> boxNames = { "STARTING A NEW GAME OF BOMBERMAN 2023", "Player vs Player", "Player vs Bot", "Back" };
		auto gnScreen = StartScreen ( boxNames );
		gnScreen.print();
		gnScreen.handleInput();
		if ( gnScreen.get() == "Back" ) Run();
		else if ( gnScreen.get() == "Player vs Player" ) GameLoop ( 1 );
		else if ( gnScreen.get() == "Player vs Bot"    ) GameLoop ( 0 );
	}
    void GameLoop( bool p )
	{
		auto seScreen = SettingsScreen ();
		seScreen.load();
		auto NameScreen1 = GetNameScreen ( "TYPE NAME FOR PLAYER1" );
		playMap = GameMap ( "GAME" );
		GameSettingsWrapper gsw ( seScreen.getHP(), seScreen.getDebuffs(), seScreen.getBaloons() );
		BuffChancesWrapper bcw = seScreen.getBuffChances();
		if ( p )
		{
			auto NameScreen2 = GetNameScreen ( "TYPE NAME FOR PLAYER2" );
			if ( !playMap.load( NameScreen1.get(), NameScreen2.get(), gsw, bcw ) ) return;
		}
		else { if ( !playMap.load( NameScreen1.get(), "BOT"s, gsw, bcw) ) return; }
		
		playMap.addDollars( seScreen.getDollarChance() );
		playMap.setDropChance ( seScreen.getDropChance() );

		//loop
		playMap.handleInput();
		auto leaderboard = Leaderboard( "Leaderboard" );
	}
    GameMap playMap;
};