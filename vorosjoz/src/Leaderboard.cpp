#include "Interface.h"

#define PATH_TO_FILE "./config/leaderboard"

class Leaderboard : public Interface
{
	public:
		Leaderboard () : Interface () {}
		Leaderboard ( const string & v ) : Interface ( v ) 
		{ 
			load();
			print();
			handleInput();
		}
		static bool scoreCmp(const pair<string, int>& pair1, const pair<string, int>& pair2) 
		{
			//sort function comparator
    		return !( pair1.second < pair2.second );
		}
		void load ()
		{
			ifstream file ( PATH_TO_FILE );
			string line, name;
			int score;
			while ( getline ( file, line ) )
			{
				if ( !checkFormat ( line, "\\s*(\\d+)\\s*=\\s*(\\S+)\\s*", score ) )//int=string format check
					return;
				size_t pos = line.find('=');
				name = line.substr(pos + 1, line.size() - 1);
				results.push_back( make_pair(name, score) );
			}
			sort ( results.begin(), results.end(), scoreCmp ) ;
			file.close();
		}
		void save ( string name, int score )
		{
			ofstream file( PATH_TO_FILE, ios::app );
			file <<  score << '=' << name << endl;
			file.close();
		}
		void print () override
		{
			checkSize();
			clear();
			writeX = maxX / 10;
			writeY = ( maxY - strlen("Leaderboard") ) / 2;
			mvprintw ( writeX, writeY, "Leaderboard" );
			writeX += 5;
			if ( !results.size() ) 
			{
				writeX = maxX / 10;
				writeY = ( maxY - strlen( "Leaderboard Empty! Play some games to change this!" ) ) / 2;
				mvprintw ( writeX, writeY, "Leaderboard Empty! Play some games to change this!" );
			}
			int i = 1;
			for ( const auto x : results )
			{
				writeX++;
				writeY = ( maxY - (maxY/6) ) / 2;
				mvprintw ( writeX, writeY, "%d", i++ );
				writeY += to_string(i).size();
				mvprintw ( writeX, writeY++, "%c", '.' );
				mvprintw ( writeX, writeY, x.first.c_str() );
				writeY += x.first.size() + 1;
				mvprintw ( writeX, writeY, "%d",  x.second );
			}
			writeX = maxX * 9 / 10;
			writeY = ( maxY - 4 ) / 2;
			boxPrint ( "Exit", -1 );
			refresh();
		}
		void handleInput () override
		{
			MEVENT event;
			int ch = '!';
			while (1) 
			{
				print();
				ch = getch();
				if ( ch == KEY_MOUSE && getmouse ( & event ) == OK ) 
					for ( const auto & z : clickBoxPos )
						if ( z.topLeftX <= event.y && z.topLeftY <= event.x &&
						 z.bottomRightX >= event.y && z.bottomRightY >= event.x )
						{
							if      ( z.jumpTo == "Exit"  ) return; 
						}
				ch = '!';
			}
		}
	private:
		vector<pair<string, int>> results; // storage structure
};