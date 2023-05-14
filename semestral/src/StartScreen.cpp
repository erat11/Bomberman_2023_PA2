#include "Interface.h"
#include <cstring>

using namespace std;

class StartScreen : public Interface
{
	public:
		StartScreen () : Interface () {}
		StartScreen ( const vector<string> & v ) : Interface ( v[0] ) 
		{ 
			/*bkgd ( COLOR_PAIR ( 1 ) );
			start_color();
			init_pair ( 1, COLOR_BLUE, COLOR_BLACK );
			backgroundColour = "blue";
			fontColour = "black";*/

			boxNames = v;
			writeY = ( maxY - caption.size() ) / 2;
			writeX = 5;
			print();
			handleInput();
		}
		void print () override
		{
			clear();
			refresh();
			mvprintw ( writeX, writeY, caption.c_str() );
			refresh();
			writeX += 6;
			for ( unsigned int i = 1; i < boxNames.size(); ++i )
			{
				boxPrint ( boxNames[i].c_str() );
				writeX += 3;
			}
		}
		void handleInput () override
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
		string get () { return jumpTo; }
	private:
		string jumpTo;
		vector <string> boxNames;
};