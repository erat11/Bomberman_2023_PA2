#include "StartScreen.h"

#define PATH_TO_SETTINGS "./config/settings"
#define PATH_TO_DEFAULT "./config/default_settings"
#define PATH_TO_BUFF_DROP_CHANCE "./config/buff_drops"

class SettingsScreen : public StartScreen
{
	public:
		SettingsScreen() : StartScreen () { mapChoice = false; }
		SettingsScreen ( const vector<string> & v ) : StartScreen ( v ) { mapChoice = false; }
		int                getDropChance  ()       { return dropChance;   }
		int                getDollarChance()       { return dollarChance; }
		int                getBaloons     ()       { return baloons;      }
		int                getDebuffs     ()       { return debuffs;      }
		int                getHP          ()       { return hp;           }
		bool               getChoice      () const { return mapChoice;    }
		BuffChancesWrapper getBuffChances () const { return bC;           }
		bool load()
		{
			// loading into variables from config files
			ifstream settingsFile ( PATH_TO_SETTINGS );
			ifstream buffDropFile ( PATH_TO_BUFF_DROP_CHANCE );
			if ( settingsFile.is_open () && buffDropFile.is_open() ) 
			{
				string line;
				if ( ! getline ( settingsFile, line ) || !checkFormat ( line, "DROP_CHANCE=(\\d+)", dropChance     ) || 
				     ! getline ( settingsFile, line ) || !checkFormat ( line, "DOLLAR_CHANCE=(\\d+)", dollarChance ) ||
				     ! getline ( settingsFile, line ) || !checkFormat ( line, "BALOONS=(\\d+)", baloons            ) || 
				     ! getline ( settingsFile, line ) || !checkFormat ( line, "DEBUFFS=(\\d+)", debuffs            ) || 
				     ! getline ( settingsFile, line ) || !checkFormat ( line, "HP=(\\d+)", hp ) )
				{
					cout << "Error reading config settingsFile, exiting...\n" << endl;
					return false;
				}
				settingsFile.close();

				if ( ! getline ( buffDropFile, line ) || !checkFormat ( line, "GAIN_HP=(\\d+)", bC.gainHp                   ) || 
				     ! getline ( buffDropFile, line ) || !checkFormat ( line, "BOMB_UPGRADE=(\\d+)", bC.bombUpgrade         ) ||
				     ! getline ( buffDropFile, line ) || !checkFormat ( line, "SCORE_MULTIPLIER=(\\d+)", bC.scoreMultiplier ) || 
				     ! getline ( buffDropFile, line ) || !checkFormat ( line, "SCORE_BONUS=(\\d+)", bC.scoreBonus           ) || 
				     ! getline ( buffDropFile, line ) || !checkFormat ( line, "IMMORTALITY=(\\d+)", bC.immortality          ) ||
					 ! getline ( buffDropFile, line ) || !checkFormat ( line, "WALLHACK=(\\d+)", bC.wallhack                ) ||
					 ! getline ( buffDropFile, line ) || !checkFormat ( line, "EXPLODE=(\\d+)", bC.explode ) )
				{
					cout << "Error reading config buffDropFile, exiting...\n" << endl;
					return false;
				}
				buffDropFile.close();
				return true;
			}
			return false;
		}
		void print() override
		{
			checkSize();
			clear();
			writeX = 10;
			writeY = ( maxY - strlen("Settings") ) / 2;
			mvprintw ( writeX, writeY, "Settings" );
			for ( unsigned int i = 1; i < boxNames.size(); ++i )
			{
				//boxprint's second argument is set to -1, if there is no additional parameter
				if      (  boxNames[i] == "Drop Chance Percent"s  ) boxPrint ( boxNames[i].c_str(), dropChance   );
				else if (  boxNames[i] == "Dollar Chance Percent"s) boxPrint ( boxNames[i].c_str(), dollarChance );
				else if (  boxNames[i] == "Debuffs On/Off"s       ) boxPrint ( boxNames[i].c_str(), debuffs      );
				else if (  boxNames[i] == "HP count"s             ) boxPrint ( boxNames[i].c_str(), hp           );
				else                                                boxPrint ( boxNames[i].c_str(), -1           );
				writeX += 1;
			}
			refresh();
		}
		void handleInput() override
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
							//click event happened in the button dimensions creaded and stored by boxprint
							if      ( z.jumpTo == "Reset Default & Exit"  ) { reset(); return;          }
							else if ( z.jumpTo == "Save & Exit"           ) { save(); return;           }
							else if ( z.jumpTo == "Drop Chance Percent"   ) { increaseDC();             }
							else if ( z.jumpTo == "Dollar Chance Percent" ) { increaseD();              }
							else if ( z.jumpTo == "HP count"              ) { increaseHP();             }
							else if ( z.jumpTo == "Debuffs On/Off"        ) { debuffs = ! debuffs;      }
							else if ( z.jumpTo == "Choose Map"            ) { mapChoice = true; return; }
						}
				ch = '!';
			}
		}
	private:
		int dropChance, dollarChance, baloons, debuffs, hp;
		BuffChancesWrapper bC;
		bool mapChoice;
		void save ()
		{
			ofstream settings ( PATH_TO_SETTINGS );
			settings << "DROP_CHANCE="   << dropChance   << endl;
			settings << "DOLLAR_CHANCE=" << dollarChance << endl;
			settings << "BALOONS="       << baloons      << endl;
			settings << "DEBUFFS="       << debuffs      << endl;
			settings << "HP="            << hp           << endl;
		}
		void reset ()
		{
			//restores defaults from "default" file
			ifstream defaultSettings ( PATH_TO_DEFAULT );
			ofstream settings ( PATH_TO_SETTINGS );
			if ( ! defaultSettings.is_open() || ! settings.is_open()  ) { cout << "error loading default"; return; }
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