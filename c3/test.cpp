#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <array>
#include <cassert>
using namespace std;
#endif /* __PROGTEST__ */

class Person
{
	public:
		Person ( string name, string surname, string number ) : name(name), surname(surname), number(number){}
		string name, surname, number;
};

int readInput ( vector<Person> & v, ifstream & inFile )
{
	string str, na, su, nu;
	getline( inFile, str );
	if ( str.empty() ) return 1;
	if ( inFile.eof() ) return -4;
	int i = 0, prev = 1;
	for ( char x : str )
		if ( ! isspace(x) )
		{
			if ( i == 3 )
				return -1;
			prev = 0;
			switch(i)
			{
				case 1: { su.push_back(x); break; }
				case 2: { nu.push_back(x); break; }
				default: { na.push_back(x); break; }
			}
		}
		else
		{
			if ( prev == 0 )
				i++;
			prev = 1;
		}
	if ( ( i != 3 && i != 2 ) || nu.front() == '0' || nu.length() != 9 )
		return -2;
	for ( char x : nu )
		if ( !isdigit(x) )
			return -3;
	Person p( na, su, nu );
	v.push_back(p);
	return 0;
}

void check ( std::vector<Person> & v, string & t, ostream & out )
{
	int i = 0;
	for ( auto x : v )
	{
		if (  x.name == t  || x.surname == t  )
		{
			i++;
			out << x.name << " " << x.surname << " " << x.number << endl;
		}
	}
	out << "-> " << i << endl;
}

bool report ( const string & fileName, ostream & out )
{
  ifstream inFile;
  inFile.open( fileName, ios_base::in );
  if ( ! inFile . is_open() )
  	return false;
  string tmp;
  vector<Person> people;
  while	( ! inFile.eof() )
  {
  	int ret = readInput( people, inFile );
  	if ( ret == 1 )
  		break;
  	else if ( ret < 0 )
  		return false;
  }
  while ( ! inFile . eof () )
  {
  	tmp.clear();
  	getline ( inFile, tmp );
  	if ( ! tmp.empty() )
  		check ( people, tmp, out );
  }
  return true;
}

#ifndef __PROGTEST__
int main ()
{
  ostringstream oss;
  oss . str ( "" );
  assert ( report( "tests/test0_in.txt", oss )== true );
  assert ( oss . str () ==
    "John Christescu 258452362\n"
    "John Harmson 861647702\n"
    "-> 2\n"
    "-> 0\n"
    "Josh Dakhov 264112084\n"
    "Dakhov Speechley 865216101\n"
    "-> 2\n"
    "John Harmson 861647702\n"
    "-> 1\n" );
  oss . str ( "" );
  assert ( report( "tests/test1_in.txt", oss ) == false );
  oss.clear();
  assert ( report( "tests/test2_in.txt", oss ) == false );
  //cout << report( "tests/test3_in.txt", oss );
  return 0;
}
#endif /* __PROGTEST__ */
