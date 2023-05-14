#ifndef INTERFACE_H
#define INTERFACE_H

#include <string>
#include <iostream>
#include <ncurses.h>
#include <vector>

using namespace std;

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
				BoxPosition ( int a, int b, int c, int d, const string & s ) : 
				topLeftX ( a ), topLeftY ( b ), bottomRightX ( c ), 
				bottomRightY ( d ), jumpTo ( s ) {} 
				int topLeftX, topLeftY, bottomRightX, bottomRightY;
				string jumpTo;
		};
		string backgroundColour, fontColour, caption;
		int writeX, writeY, maxX, maxY;
		vector<BoxPosition> clickBoxPos;

		void boxPrint ( const char * s );
};

#endif