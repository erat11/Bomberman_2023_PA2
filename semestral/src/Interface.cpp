#include <fstream>
#include "Interface.h"
#include <iostream>
#include <string>
#include <regex>
#include <sys/ioctl.h>
#include <unistd.h>

using namespace std;

/*
	initscr();

	// set the background color to blue
	bkgd(COLOR_PAIR(1));
	start_color();
	init_pair(1, COLOR_BLUE, COLOR_BLACK);

	// print a message on the blue background
	printw("Hello, world!");

	refresh();
	getch();

	endwin();
	return 0;
*/

Interface::Interface () {}

Interface::Interface ( const string & caption )
{
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	mousemask(ALL_MOUSE_EVENTS, NULL);

	this->caption = caption;
	getmaxyx ( stdscr, maxX, maxY );
}

void Interface::print() { cout << caption; }

void Interface::handleInput () { getch(); }

void Interface::boxPrint ( const char * s )
{
	int l = strlen ( s );
	writeY = ( maxY - l - 4 ) / 2;
	int oldY = writeY;

	int tlx = writeX, tly = writeY;
	for ( int i = 0; i < l + 4; ++i ) mvprintw ( writeX, writeY++, "-" );

	writeX++;
	writeY = oldY;
	mvprintw ( writeX, writeY++, "|" );
	for ( int i = 0; i < l + 2; ++i ) mvprintw ( writeX, writeY++, " " );
	mvprintw ( writeX, writeY++, "|" );

	writeX++;
	writeY = oldY;
	mvprintw ( writeX, writeY, "| " );
	writeY += 2;
	mvprintw ( writeX, writeY, s );
	writeY += l;
	mvprintw ( writeX, writeY, " |" );
	writeY += 2;

	writeX++;
	writeY = oldY;
	mvprintw ( writeX, writeY++, "|" );
	for ( int i = 0; i < l + 2; ++i ) mvprintw ( writeX, writeY++, " " );
	mvprintw ( writeX, writeY++, "|" );

	writeX++;
	writeY = oldY;
	for ( int i = 0; i < l + 4; ++i ) mvprintw ( writeX, writeY++, "-" );

	clickBoxPos.push_back( BoxPosition ( tlx, tly, writeX, writeY, string ( s ) ) );
}

Interface::~Interface() 
{
	clear();
	refresh();
	endwin();
}

/*bool Interface::readConfig ( const char * configFile )
{
	cout << "interface\n";
	ifstream file ( configFile );
	if ( file.is_open () ) 
	{
		string line;
		if ( ! getline ( file, line ) || !checkFormat ( line, "resX=(\\d+)", width ) )  //reading resX
			cout << "Error reading config file, exiting...\n";
		if ( ! getline ( file, line ) || !checkFormat ( line, "resY=(\\d+)", height ) )  //reading resY
			cout << "Error reading config file, exiting...\n";
		
		file.close(); // Close file
	}
	else cout << "Failed to open config file, exiting...\n";
	return true;
}

bool Interface::checkFormat ( const string & s, const char * c, int & value )
{
	regex pattern ( c );
	smatch match;
	if ( regex_search ( s, match, pattern ) ) value = stoi(match[1].str());
	else return false;
	return true;
}*/