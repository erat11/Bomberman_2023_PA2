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
	//wraps info from "buff_drops"
	int gainHp, bombUpgrade, scoreMultiplier, scoreBonus, immortality, wallhack, explode;
};

class Interface
{
	/*
	* This class handles all the ncurses micromanagement, initializations ...
	* It is not directly used, it is merely a template for all the other screen implementations
	*/
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
		string caption;
		// maxX, maxY are the values of the right bottom edge of the screen
		int writeX, writeY, maxX, maxY; // writeX, writeY is the current cursor position
		void boxPrint ( const char * s, int value ); //prints a button made of - and |, saves the position into clickBoxPos
		bool isInVector ( const BoxPosition & x );
		bool checkFormat ( const string & s, const char * c, int & value ); // uses regexp to check config file structure 
		void checkSize ();
		vector <string> boxNames; //button captions
		vector<BoxPosition> clickBoxPos; //stores button positions
};

#endif