#include <fstream>
#include "Interface.h"

Interface::Interface () {}

Interface::Interface ( const char * configFile )
{
	ofstream file ( configFile );
	cout << "interface\n";
	file.close();
}