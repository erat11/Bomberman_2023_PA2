#include "Interface.h"

class GetNameScreen : public Interface
{
	public:
		GetNameScreen () : Interface () {}
		GetNameScreen ( const char * s ) : Interface ( s )
		{
			print();
			handleInput();
		}
		void print () override
		{
			clear();
			refresh();
			writeY = ( maxY - caption.size() ) / 2;
			writeX = 10;
			mvprintw ( writeX, writeY, caption.c_str() );
			refresh();
			writeX += 6;
			writeY = ( maxY - playerName.size() ) / 2;
			mvprintw ( writeX, writeY, playerName.c_str() );
		}
		void handleInput () override
		{
			echo();
			int ch = ' ';
			while ( 1 ) 
			{
				print();
				ch = getch();
				print();
				if ( ( ch == '\n' && playerName.size() != 0 ) || playerName.size() > 32 ) break;
				if ( ch == '\n' && playerName.size() == 0 ) continue;
				playerName += ch;
			}
			noecho();
		}
		string get () { return playerName; }
	private:
		string playerName;
};