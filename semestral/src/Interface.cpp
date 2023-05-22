#include "Interface.h"

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

void Interface:: boxPrint ( const char * s, int value )
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
	writeY += 3;
	if ( value != -1 ) mvprintw ( writeX, writeY, to_string(value).c_str() );

	writeX++;
	writeY = oldY;
	mvprintw ( writeX, writeY++, "|" );
	for ( int i = 0; i < l + 2; ++i ) mvprintw ( writeX, writeY++, " " );
	mvprintw ( writeX, writeY++, "|" );

	writeX++;
	writeY = oldY;
	for ( int i = 0; i < l + 4; ++i ) mvprintw ( writeX, writeY++, "-" );

	auto u = BoxPosition ( tlx, tly, writeX, writeY, value, string ( s ) );
	if ( !isInVector( u ) )
		clickBoxPos.push_back( BoxPosition ( tlx, tly, writeX, writeY, value, string ( s ) ) );
}

void Interface::print() { cout << caption; }

bool Interface::isInVector ( const BoxPosition & z )
{
	for ( const auto & x : clickBoxPos )
		if ( z.jumpTo == x.jumpTo ) return true;
	return false;
}

void Interface::handleInput () { getch(); }

void Interface::checkSize ()
{
	getmaxyx ( stdscr, maxX, maxY );
}

Interface::~Interface() 
{
	clear();
	refresh();
	endwin();
}