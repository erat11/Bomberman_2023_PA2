#include "Interface.h"

class GetNameScreen : public Interface
{
	/*
	* Name query screen
	*/
	public:
		GetNameScreen () : Interface () {}
		GetNameScreen ( const char * s ) : Interface ( s )
		{
			print();
			handleInput();
		}
		void print () override
		{
			checkSize();
			clear();
			refresh();
			writeY = ( maxY - caption.size() ) / 2;
			writeX = 10;
			mvprintw ( writeX, writeY, caption.c_str() );
			refresh();
			writeX += 12;
			writeY = ( maxY - 18 ) / 2;
			mvprintw ( writeX, writeY, "( 1 to 16 letters )" );
			writeX -= 6;
			writeY = ( maxY - playerName.size() ) / 2;
			mvprintw ( writeX, writeY, playerName.c_str() );
		}
		void handleInput () override
		{
			echo(); // enables letter input to be displayed on screen
			int ch = ' ';
			while ( 1 ) 
			{
				print();
				ch = getch();
				print();
				if ( ( ch == '\n' && playerName.size() != 0 ) || playerName.size() > 16 ) break;
				if ( ch == '\n' && playerName.size() == 0 ) continue;
				playerName += ch;
			}
			noecho();
		}
		string get () { return playerName; }
	private:
		string playerName;
};