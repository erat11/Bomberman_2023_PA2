#include "StartScreen.h"
#include "GetNameScreen.cpp"
#include "Player.h"
#include "GameMap.h"
#include "SettingsScreen.cpp"
#include "MapChooseScreen.cpp"

class Game
{
	/* 
	* Initiates the start screen, handles mouse actions ( button clicking ), 
	* initiates other interface screens on button press
	*/
	public:
	    Game() { Run(); }
	    void Run ()
		{
			//define start screen button captions
			vector<string> boxNames = { "BOMBERMAN 2023", "Start New Game", "Settings", "Leaderboard", "Exit" };

			auto stScreen = StartScreen ( boxNames );
			stScreen.print();
			stScreen.handleInput();
			//stScreen returns from handleInput after a button is pressed, the result is saved, accessible by the get method

			if ( stScreen.get() == "Exit"           ) return;
			if ( stScreen.get() == "Start New Game" ) StartGame();
			else if ( stScreen.get() == "Settings" ) 
			{
				//define settings button captions
				boxNames = { "Settings", "Drop Chance Percent", "Dollar Chance Percent",
				             "Debuffs On/Off", "HP count", "Choose Map", "Save & Exit", "Reset Default & Exit" };
				auto seScreen = SettingsScreen ( boxNames );
				if ( !seScreen.load() ) { cout << "Error handling the Settings file"; return; }
				seScreen.handleInput();
				if ( seScreen.getChoice() ) auto x = MapChooseScreen(); //checking if choose map was clicked
				Run();//return back to the beginning 
			}
			else if ( stScreen.get() == "Leaderboard" )
			{
				auto leaderboard = Leaderboard( " Leaderboard ");
				Run();
			}
		}
	private:
	    void StartGame()
		{
			//choosing between game modes
			vector<string> boxNames = { "STARTING A NEW GAME OF BOMBERMAN 2023", "Player vs Player", "Player vs Bot", "Back" };

			auto gnScreen = StartScreen ( boxNames );
			gnScreen.print();
			gnScreen.handleInput();
			if ( gnScreen.get() == "Back" ) Run();//back to start screen

			else if ( gnScreen.get() == "Player vs Player" ) GameLoop ( 1 ); // argument is false if botgame 

			else if ( gnScreen.get() == "Player vs Bot"    ) GameLoop ( 0 );
		}
	    void GameLoop( bool p )
		{
			auto seScreen = SettingsScreen ();
			//load from settings config file "settings"
			if ( !seScreen.load() ) { cout << "Error handling the Settings file"; return; }
			auto NameScreen1 = GetNameScreen ( "TYPE NAME FOR PLAYER1" );
			playMap = GameMap ( "GAME" );

			//wrappers for all the config file info
			GameSettingsWrapper gsw ( seScreen.getHP(), seScreen.getDebuffs(), seScreen.getBaloons() );
			BuffChancesWrapper bcw = seScreen.getBuffChances();

			if ( p )
			{
				auto NameScreen2 = GetNameScreen ( "TYPE NAME FOR PLAYER2" );
				if ( !playMap.load( NameScreen1.get(), NameScreen2.get(), gsw, bcw ) ) return;
			}
			else { if ( !playMap.load( NameScreen1.get(), "BOT"s, gsw, bcw) ) return; }
			
			playMap.addDollars( seScreen.getDollarChance() );
			playMap.setDropChance ( seScreen.getDropChance() );//defines the static variable in all walls

			//loop
			playMap.handleInput();
			//end loop

			auto leaderboard = Leaderboard( "Leaderboard" );
		}
	    GameMap playMap;//this variable handles all of the game actions in its handleInput method
};