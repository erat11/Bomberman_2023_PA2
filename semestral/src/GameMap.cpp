#include "Interface.h"

#define PATH_TO_MAP_FILE "./config/maps/selected"

class GameMap : public Interface
{
	public:
		GameMap () : Interface () {}
		GameMap ( const char * caption ) : Interface ( caption )
		{
			if ( ! load() ) { cout << "error whilst loading\n"; return; }
			print();
		}
		bool load ( )
		{
			ifstream file ( PATH_TO_MAP_FILE );
			if ( file.is_open () ) 
			{
				string line;
				if ( ! getline ( file, line ) || !checkFormat ( line, "X=(\\d+)", sizeX ) || 
				   ( ! getline ( file, line ) || !checkFormat ( line, "Y=(\\d+)", sizeY ) ) )
				{
					cout << "Error reading config file, exiting...\n" << endl;
					return false;
				}
				while ( getline ( file, line ) ) 
					gameMap.push_back( line );
				file.close();
				//todo verify map integrity
				return true;
			}
			cout << "Error while opening config file, exiting...\n";
			return false;
		}
		bool checkFormat ( const string & s, const char * c, int & value )
		{
			regex pattern ( c );
			smatch match;
			if ( regex_search ( s, match, pattern ) ) value = stoi(match[1].str());
			else return false;
			return true;
		}
		void print () override
		{
			checkSize();
			clear();
			refresh();
			writeX = ( maxX - sizeX ) / 2;
			writeY = ( maxY - sizeY ) / 2;
			for ( auto & x : gameMap )
			{
				
				mvprintw( writeX++, writeY, x.c_str() );
				writeY = ( maxY - sizeY ) / 2;
			} 
			getch();
		}
	private:
		int sizeX, sizeY;
		vector<string> gameMap;
};
