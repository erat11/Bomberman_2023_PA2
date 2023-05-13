#define CONFIG_START_SCREEN "./config/startScreen.ini"
#include "StartScreen.h"
#include "Game.h"

using namespace std;

Game::Game ()
{
	Run();
}

void Game::Run ()
{
	cout << "game\n";
	screen = StartScreen( CONFIG_START_SCREEN );
}

Game::~Game () {}