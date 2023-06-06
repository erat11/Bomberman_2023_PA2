#ifndef STARTSCREEN_H
#define STARTSCREEN_H
#include "Interface.h"

using namespace std;

class StartScreen : public Interface
{
	public:
		StartScreen ();
		StartScreen ( const vector<string> & v );
		virtual void print () override;
		virtual void handleInput () override;
		virtual string get () const;
	private:
		string jumpTo;
};

#endif