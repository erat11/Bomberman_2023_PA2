#include "StartScreen.h"

StartScreen::StartScreen () : Interface () {}
StartScreen::StartScreen ( const vector<string> & v ) : Interface ( v[0] ) 
{ 
	boxNames = v;
}

void StartScreen::print ()
{
	checkSize();
	clear();
	refresh();
	writeY = ( maxY - caption.size() ) / 2;
	writeX = 5;
	mvprintw ( writeX, writeY, caption.c_str() );
	refresh();
	writeX += 6;
	for ( unsigned int i = 1; i < boxNames.size(); ++i )
	{
		boxPrint ( boxNames[i].c_str(), -1 );
		writeX += 3;
	}
}
void StartScreen::handleInput ()
{
	MEVENT event;
	int ch;
	while (1) 
	{
		ch = getch();
		if ( ch == KEY_MOUSE && getmouse ( & event ) == OK ) 
			for ( const auto & z : clickBoxPos )
				if ( z.topLeftX <= event.y && z.topLeftY <= event.x &&
				 z.bottomRightX >= event.y && z.bottomRightY >= event.x )
				{
					jumpTo = z.jumpTo;
					return;
				}
	}
}
string StartScreen::get () { return jumpTo; }