#ifndef INTERFACE_H
#define INTERFACE_H

#include <string>
#include <iostream>

using namespace std;

class Interface
{
	public:
		Interface ();
		Interface ( const char * configFile );
	protected:
		int width, heigth;
		bool visibility;
		string caption;
		//backgroud color, font size ...
};

#endif