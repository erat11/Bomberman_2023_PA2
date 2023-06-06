#include "Interface.h"

#define PATH_TO_SELECTED "./config/maps/selected"
#define PATH_TO_MAP_NAMES "./config/maps/names"

class MapChooseScreen : public Interface
{
	public:
		MapChooseScreen() : Interface ( "Choose your Map" )
		{
			currentlySelected = 0;
			load();
			print();
			handleInput();
			sizeX = 0; sizeY = 0;
		}
		void load ()
		{
			ifstream file ( PATH_TO_MAP_NAMES );
			string line;
			while ( getline ( file, line ) ) 
			{ 
				if ( line.back() == '\n' ) line.pop_back();
				mapNames.push_back( line );
			}
			if ( mapNames.size() ) readMap();
		}
		void print () override
		{
			clear();
			refresh();
			writeY = ( maxY - caption.size() ) / 2;
			writeX = 1;
			mvprintw ( writeX, writeY, caption.c_str() );

			writeX += 2;
			writeY = ( maxY - sizeY ) / 2;
			for ( const auto x : currentMap )
				mvprintw ( writeX++, writeY, x.c_str() );

			writeX = ( maxX * 8 ) / 10;
			boxPrint ( "Next Map", -1 );
			writeX++;
			boxPrint ( "Save & Exit", -1 );
			refresh();
		}
		void handleInput () override
		{
			MEVENT event;
			int ch;
			while (1) 
			{
				print();
				ch = getch();
				if ( ch == KEY_MOUSE && getmouse ( & event ) == OK ) 
					for ( const auto & z : clickBoxPos )
						if ( z.topLeftX <= event.y && z.topLeftY <= event.x &&
						 z.bottomRightX >= event.y && z.bottomRightY >= event.x )
						{
							if ( z.jumpTo == "Next Map" ) nextMap();
							else if ( z.jumpTo == "Save & Exit" ) { save(); return; }
						}
			}
		}
	private:
		vector<string> mapNames;
		int currentlySelected, sizeX, sizeY;
		vector<string> currentMap;
		bool readMap()
		{
			string name = "./config/maps/" + mapNames[currentlySelected];
			ifstream file ( name );
			string line;
			if ( ! getline ( file, line ) || !checkFormat ( line, "X=(\\d+)", sizeX ) || 
			   ( ! getline ( file, line ) || !checkFormat ( line, "Y=(\\d+)", sizeY ) ) )
			{
				cout << "Error reading map file, exiting...\n" << endl;
				return false;
			}
			currentMap.clear();
			while ( getline ( file, line ) ) { line.push_back('\n'); currentMap.push_back(line); line.clear(); }
			file.close();
			return true;
		}
		void nextMap ()
		{
			currentlySelected ++;
			currentlySelected %= mapNames.size();
			if ( mapNames.size() ) readMap();
		}
		void save ()
		{
			if ( currentMap.size() )
			{
				ofstream file(PATH_TO_SELECTED, ios::trunc);
				file << "X=" << sizeX << endl;
				file << "Y=" << sizeY << endl;
				for ( auto x : currentMap ) file << x;
				file.close();
			}
		}
};