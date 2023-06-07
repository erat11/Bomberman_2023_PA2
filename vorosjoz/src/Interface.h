#ifndef INTERFACE_H
#define INTERFACE_H
#include <cstring>

#include <string>
#include <iostream>
#include <ncurses.h>
#include <vector>
#include <fstream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <regex>
#include <random>

using namespace std;

struct BuffChancesWrapper
{
	int gainHp, bombUpgrade, scoreMultiplier, scoreBonus, immortality, wallhack, explode;
};

class Interface
{
	public:
		Interface ();
		Interface ( const string & s );
		virtual void print();
	    virtual void handleInput();
	    ~Interface ();
	protected:
		class BoxPosition
		{
			public:
				BoxPosition ( int a, int b, int c, int d, int v, const string & s ) : 
				topLeftX ( a ), topLeftY ( b ), bottomRightX ( c ), 
				bottomRightY ( d ), value ( v ), jumpTo ( s ) {} 
				int topLeftX, topLeftY, bottomRightX, bottomRightY, value;
				string jumpTo;
		};
		string backgroundColour, fontColour, caption;
		int writeX, writeY, maxX, maxY;

		void boxPrint ( const char * s, int value );
		bool isInVector ( const BoxPosition & x );
		bool checkFormat ( const string & s, const char * c, int & value );

		void checkSize ();
		vector <string> boxNames;
		vector<BoxPosition> clickBoxPos;
};

#endif