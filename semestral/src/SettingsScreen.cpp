#include "StartScreen.h"

#define PATH_TO_SETTINGS "./config/settings"
#define PATH_TO_DEFAULT "./config/default_settings"

class SettingsScreen : public StartScreen
{
	public:
		SettingsScreen () : StartScreen () { mapChoice = false; }
		SettingsScreen ( const vector<string> & v ) : StartScreen ( v ) { mapChoice = false; }
		int getDropChance() { return dropChance; }
		int getDollarChance() { return dollarChance; }
		int getBaloons() { return baloons; }
		int getDebuffs() { return debuffs; }
		int getHP() { return hp; }
		bool load ( )
		{
			ifstream file ( PATH_TO_SETTINGS );
			if ( file.is_open () ) 
			{
				string line;
				if ( ! getline ( file, line ) || !checkFormat ( line, "DROP_CHANCE=(\\d+)", dropChance     ) || 
				     ! getline ( file, line ) || !checkFormat ( line, "DOLLAR_CHANCE=(\\d+)", dollarChance ) ||
				     ! getline ( file, line ) || !checkFormat ( line, "BALOONS=(\\d+)", baloons            ) || 
				     ! getline ( file, line ) || !checkFormat ( line, "DEBUFFS=(\\d+)", debuffs            ) || 
				     ! getline ( file, line ) || !checkFormat ( line, "HP=(\\d+)", hp ) )
				{
					cout << "Error reading config file, exiting...\n" << endl;
					return false;
				}
				file.close();
				return true;
			}
			return false;
		}
		void print () override
		{
			checkSize();
			clear();
			writeX = 2;
			writeY = ( maxY - strlen("Settings") ) / 2;
			mvprintw ( writeX, writeY, "Settings" );
			for ( unsigned int i = 1; i < boxNames.size(); ++i )
			{
				if      (  boxNames[i] == "Drop Chance Percent"s  ) boxPrint ( boxNames[i].c_str(), dropChance );
				else if (  boxNames[i] == "Dollar Chance Percent"s) boxPrint ( boxNames[i].c_str(), dollarChance );
				else if (  boxNames[i] == "Debuffs On/Off"s       ) boxPrint ( boxNames[i].c_str(), debuffs );
				else if (  boxNames[i] == "Baloons On/Off"s       ) boxPrint ( boxNames[i].c_str(), baloons );
				else if (  boxNames[i] == "HP count"s) boxPrint ( boxNames[i].c_str(), hp );
				else boxPrint ( boxNames[i].c_str(), -1 );
				writeX += 1;
			}
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
							if      ( z.jumpTo == "Reset Default & Exit"  ) { reset(); return; }
							else if ( z.jumpTo == "Save & Exit"           ) { save(); return; }
							else if ( z.jumpTo == "Drop Chance Percent"   ) { increaseDC(); }
							else if ( z.jumpTo == "Dollar Chance Percent" ) { increaseD(); }
							else if ( z.jumpTo == "HP count"              ) { increaseHP(); }
							else if ( z.jumpTo == "Debuffs On/Off"        ) { debuffs = ! debuffs; }
							else if ( z.jumpTo == "Baloons On/Off"        ) { baloons = ! baloons; }
							else if ( z.jumpTo == "Choose Map"            ) { mapChoice = true; return; }
						}
				ch = '!';
			}
		}
		bool getChoice () const { return mapChoice; }
	private:
		int dropChance, dollarChance, baloons, debuffs, hp;
		bool mapChoice;
		void save ()
		{
			ofstream settings( PATH_TO_SETTINGS );
			settings << "DROP_CHANCE=" << dropChance << endl;
			settings << "DOLLAR_CHANCE=" << dollarChance << endl;
			settings << "BALOONS=" << baloons << endl;
			settings << "DEBUFFS=" << debuffs << endl;
			settings << "HP=" << hp << endl;
		}
		void reset ()
		{
			ifstream defaultSettings( PATH_TO_DEFAULT );
			ofstream settings( PATH_TO_SETTINGS );
			if ( ! defaultSettings.is_open() || ! settings.is_open()  ) cout << "error loading default";
			string line;
			while ( getline ( defaultSettings, line ) ) settings << line << endl;
		}
		void increaseDC () 
		{
			dropChance += 5;
			dropChance %= 55;
		}
		void increaseD () 
		{
			dollarChance += 2;
			dollarChance %= 22;
		}
		void increaseHP () 
		{
			hp += 1;
			hp %= 11;
			if ( !hp ) hp++;
		}

};