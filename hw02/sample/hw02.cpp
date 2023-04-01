#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream> 
#include <iomanip> 
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */ 

class Name
{
	public:
		Name () {}
		Name ( string name, string surname ) : n ( name ), s ( surname ) {}
		string n, s;
		bool operator==( const Name & other ) const 
		{
			return ( other.n == n && other.s == s );
		}
		bool operator<( const Name & other ) const 
		{
			if ( s < other.s ) return true;
			if ( s > other.s ) return false;
			if ( n < other.n ) return true;
			return false;
		}
		bool operator>( const Name & other ) const 
		{
			if ( s > other.s ) return true;
			if ( s < other.s ) return false;
			if ( n > other.n ) return true;
			return false;
		}
		void print (  ) const
		{
			cout << s << " " << n << endl;
		}
};

class Person
{
	public:
		Person ( Name wholeName, string email, unsigned int salary ) : wholeName( wholeName ), email ( email ), salary ( salary ){}
		Name wholeName;
		string email;
		unsigned int salary;
};

class CPersonalAgenda
{
	public:
		CPersonalAgenda  ( ) { size = 0; }
		~CPersonalAgenda ( ) { }
		bool add ( const string & name, const string & surname, const string & email, unsigned int salary )
		{
			Name wholeName ( name, surname );
			for ( auto & x : nameSort )
				if ( wholeName == x.wholeName || x.email == email ) return false;
			Person p ( wholeName, email, salary );
			push ( p );
			size++;
			return true;
		}
		bool del ( const string & name, const string & surname )
		{
			if ( size == 1 ) { emailSort.clear(); nameSort.clear(); size = 0; return true; }
			int pos = binSearch ( nameSort, Person ( Name ( name, surname ), "email", 100 ), true );
			if ( pos == -1 ) return false;
			string e = nameSort[pos].email;
			nameSort.erase( nameSort.begin() + pos );
			int pos2 = binSearch ( emailSort, Person ( Name ( "hue", "jazz" ), e, 100 ), false );
			emailSort.erase( emailSort.begin() + pos2 );
			size--;
			return true;
		}
		bool del ( const string & email )
		{
			if ( size == 1 ) { emailSort.clear(); nameSort.clear(); size = 0; return true; }
			int pos = binSearch ( emailSort, Person ( Name ( "hue", "jazz" ), email, 100 ), false );
			if ( pos == -1 ) return false;
			Name n = emailSort[pos].wholeName;
			emailSort.erase( emailSort.begin() + pos );
			int pos2 = binSearch ( nameSort, Person ( n, "e", 100 ), true );
			nameSort.erase( nameSort.begin() + pos2 );
			size--;
			return true;
		}
		bool changeName ( const string & email, const string & newName, const string & newSurname )
		{
			int q = binSearch ( nameSort, Person ( Name ( newName, newSurname ), "email", 100 ), true );
			int pos = binSearch ( emailSort, Person ( Name ( "hue", "jazz" ), email, 100 ), false );
			if ( pos == -1 || q != -1 ) return false;
			string oldName = emailSort[pos].wholeName.n, oldSurname = emailSort[pos].wholeName.s;
			emailSort[pos].wholeName = Name ( newName, newSurname );
			int pos2 = binSearch ( nameSort, Person ( Name ( oldName, oldSurname ), email, 100 ), true );
			nameSort.erase( nameSort.begin() + pos2 );
			Person p ( Name ( newName, newSurname), email, emailSort[pos].salary );
			for ( int i = 0; i < size - 1; ++i ) if ( p.wholeName < nameSort[i].wholeName  ) { nameSort.insert( nameSort.begin() + i, p ); break; }
			return true;
		}
		bool changeEmail ( const string & name, const string & surname, const string & newEmail )
		{
			if ( binSearch ( emailSort, Person ( Name ( "MY", "GOD" ), newEmail, 666 ), false ) != -1 ) return false;
			int pos = binSearch ( nameSort, Person ( Name ( name, surname ), "email", 100 ), true );
			if ( pos == -1 ) return false;
			string oldEmail = nameSort[pos].email;
			nameSort[pos].email = newEmail;
			int pos2 = binSearch ( emailSort, Person ( Name ( "oldName, oldSurnam", "e" ), oldEmail, 100 ), false );
			emailSort.erase( emailSort.begin() + pos2 );
			Person p ( Name ( name, surname), newEmail, nameSort[pos].salary );
			for ( int i = 0; i < size - 1; ++i ) if ( p.email < emailSort[i].email  ) { emailSort.insert( emailSort.begin() + i, p ); break; }
			return true;
		}
		bool setSalary ( const string & name, const string & surname, unsigned int salary )
		{
			int pos = binSearch ( nameSort, Person ( Name ( name, surname ), "df", 100 ), true );
			if ( pos == -1 ) return false;
			nameSort[pos].salary = salary;
			int pos2 = binSearch ( emailSort, Person ( Name ( "dsf", "AAA" ), nameSort[pos].email, 100 ), false );
			emailSort[pos2].salary = salary;
			return true;
		}
		bool setSalary ( const string & email, unsigned int salary )
		{
			int pos = binSearch ( emailSort, Person ( Name ( "hue", "jazz" ), email, 100 ), false );
			if ( pos == -1 ) return false;
			emailSort[pos].salary = salary;
			int pos2 = binSearch ( nameSort, Person ( Name ( emailSort[pos].wholeName.n, emailSort[pos].wholeName.s ), "fsdf", 100 ), true );
			nameSort[pos2].salary = salary;
			return true;
		}
		unsigned int getSalary ( const string & name, const string & surname ) const
		{	
			int pos = binSearch ( nameSort, Person ( Name ( name, surname ), "meme", 100 ), true );
			if ( pos == -1 ) return false;
			return nameSort[pos].salary;
		}
		unsigned int getSalary ( const string & email ) const
		{
			int pos = binSearch ( emailSort, Person ( Name ( "hue", "jazz" ), email, 100 ), false );
			if ( pos == -1 ) return false;
			return emailSort[pos].salary;
		}
		bool getRank ( const string & name, const string & surname, int & rankMin, int & rankMax ) const
		{
			rankMin = 0, rankMax = 0;
			int pos = binSearch ( nameSort, Person ( Name ( name, surname ), "meme", 100 ), true );
			if ( pos == -1 ) return false;
			for ( auto & x : nameSort )
				if ( x.salary < nameSort[pos].salary )
					rankMin++;
				else if ( x.salary == nameSort[pos].salary && x.email != nameSort[pos].email )
					rankMax ++;
			rankMax += rankMin;
			return true;
		}
		bool getRank ( const string & email, int & rankMin, int & rankMax ) const
		{
			rankMin = 0, rankMax = 0;
			int pos = binSearch ( emailSort, Person ( Name ( "hue", "jazz" ), email, 100 ), false );
			if ( pos == -1 ) return false;
			for ( auto & x : emailSort )
				if ( x.salary < emailSort[pos].salary )
					rankMin++;
				else if ( x.salary == emailSort[pos].salary && x.email != emailSort[pos].email )
					rankMax ++;
				
			rankMax += rankMin;
			return true;
		}
		bool getFirst ( string & outName, string & outSurname ) const
		{
			if ( size == 0 ) return false;
			outName = nameSort[0].wholeName.n;
			outSurname = nameSort[0].wholeName.s;
			return true;
		}
		bool getNext ( const string & name, const string & surname, string & outName, string & outSurname ) const
		{
			int pos = binSearch ( nameSort, Person ( Name ( name, surname ), "huejazz@lol.troll", 100 ), true );
			if ( pos == -1 || pos + 1 == size ) return false;
			outName = nameSort[pos+1].wholeName.n;
			outSurname = nameSort[pos+1].wholeName.s;
			return true;
		}
		void print ( ) const
		{
			for ( auto & x : nameSort ) cout << x.wholeName.s << " " << x.wholeName.n << " " << x.email << " " << x.salary << endl;
			cout << endl;
			for ( auto & x : emailSort ) cout << x.wholeName.s << " " << x.wholeName.n << " " << x.email << " " << x.salary << endl;
			cout << endl;
		}
	private:
		vector<Person> nameSort, emailSort;
		int size;
		void push ( const Person p )
		{
			bool n = false, e = false;
			auto iten = nameSort.begin(), itee = emailSort.begin();
			for ( int i = 0; i < size; ++i )
			{
				if ( p.wholeName < nameSort[i].wholeName && !n ) { nameSort.insert( iten + i, p ); n = true; }
				if ( p.email < emailSort[i].email && !e ) { emailSort.insert( itee + i, p ); e = true; }
			}
			if ( ! n ) nameSort.push_back( p );
			if ( ! e ) emailSort.push_back( p );
		}
		int binSearch ( const vector<Person> v, const Person p, const bool name ) const
		{
			size_t h = size, l = 0;
			while ( h > l )
			{
				size_t mid = l + ( h - l ) / 2;
				if ( name )
				{
					if ( p.wholeName == v[mid].wholeName ) return mid;
					if ( p.wholeName > v[mid].wholeName ) l = mid + 1;
					else h = mid;
				}
				else
				{
					if ( p.email == v[mid].email ) return mid;
					if ( p.email > v[mid].email ) l = mid + 1;
					else h = mid;
				}
			}
			return -1;
		}
};

#ifndef __PROGTEST__
int main ( void )
{
  string outName, outSurname;
  int lo, hi;

  CPersonalAgenda b1;
  assert ( b1 . add ( "John", "Smith", "john", 30000 ) );
  assert ( b1 . add ( "John", "Miller", "johnm", 35000 ) );
  assert ( b1 . add ( "Peter", "Smith", "peter", 23000 ) );
  assert ( b1 . getFirst ( outName, outSurname )
           && outName == "John"
           && outSurname == "Miller" );
  assert ( b1 . getNext ( "John", "Miller", outName, outSurname )
           && outName == "John"
           && outSurname == "Smith" );
  assert ( b1 . getNext ( "John", "Smith", outName, outSurname )
           && outName == "Peter"
           && outSurname == "Smith" );
  assert ( ! b1 . getNext ( "Peter", "Smith", outName, outSurname ) );
  assert ( b1 . setSalary ( "john", 32000 ) );
  assert ( b1 . getSalary ( "john" ) ==  32000 );
  assert ( b1 . getSalary ( "John", "Smith" ) ==  32000 );
  assert ( b1 . getRank ( "John", "Smith", lo, hi )
           && lo == 1
           && hi == 1 );
  assert ( b1 . getRank ( "john", lo, hi )
           && lo == 1
           && hi == 1 );
  assert ( b1 . getRank ( "peter", lo, hi )
           && lo == 0
           && hi == 0 );
  assert ( b1 . getRank ( "johnm", lo, hi )
           && lo == 2
           && hi == 2 );
  assert ( b1 . setSalary ( "John", "Smith", 35000 ) );
  assert ( b1 . getSalary ( "John", "Smith" ) ==  35000 );
  assert ( b1 . getSalary ( "john" ) ==  35000 );
  assert ( b1 . getRank ( "John", "Smith", lo, hi )
           && lo == 1
           && hi == 2 );
  assert ( b1 . getRank ( "john", lo, hi )
           && lo == 1
           && hi == 2 );
  assert ( b1 . getRank ( "peter", lo, hi )
           && lo == 0
           && hi == 0 );
  assert ( b1 . getRank ( "johnm", lo, hi )
           && lo == 1
           && hi == 2 );
  assert ( b1 . changeName ( "peter", "James", "Bond" ) );
  assert ( b1 . getSalary ( "peter" ) ==  23000 );
  assert ( b1 . getSalary ( "James", "Bond" ) ==  23000 );
  assert ( b1 . getSalary ( "Peter", "Smith" ) ==  0 );
  assert ( b1 . getFirst ( outName, outSurname )
           && outName == "James"
           && outSurname == "Bond" );
  assert ( b1 . getNext ( "James", "Bond", outName, outSurname )
           && outName == "John"
           && outSurname == "Miller" );
  assert ( b1 . getNext ( "John", "Miller", outName, outSurname )
           && outName == "John"
           && outSurname == "Smith" );
  assert ( ! b1 . getNext ( "John", "Smith", outName, outSurname ) );
  assert ( b1 . changeEmail ( "James", "Bond", "james" ) );
  assert ( b1 . getSalary ( "James", "Bond" ) ==  23000 );
  assert ( b1 . getSalary ( "james" ) ==  23000 );
  assert ( b1 . getSalary ( "peter" ) ==  0 );
  assert ( b1 . del ( "james" ) );
  assert ( b1 . getRank ( "john", lo, hi )
           && lo == 0
           && hi == 1 );
  assert ( b1 . del ( "John", "Miller" ) );
  assert ( b1 . getRank ( "john", lo, hi )
           && lo == 0
           && hi == 0 );
  assert ( b1 . getFirst ( outName, outSurname )
           && outName == "John"
           && outSurname == "Smith" );
  assert ( ! b1 . getNext ( "John", "Smith", outName, outSurname ) );
  assert ( b1 . del ( "john" ) );
  assert ( ! b1 . getFirst ( outName, outSurname ) );
  assert ( b1 . add ( "John", "Smith", "john", 31000 ) );
  assert ( b1 . add ( "john", "Smith", "joHn", 31000 ) );
  assert ( b1 . add ( "John", "smith", "jOhn", 31000 ) );

  CPersonalAgenda b2;
  assert ( ! b2 . getFirst ( outName, outSurname ) );
  assert ( b2 . add ( "James", "Bond", "james", 70000 ) );
  assert ( b2 . add ( "James", "Smith", "james2", 30000 ) );
  assert ( b2 . add ( "Peter", "Smith", "peter", 40000 ) );
  assert ( ! b2 . add ( "James", "Bond", "james3", 60000 ) );
  assert ( ! b2 . add ( "Peter", "Bond", "peter", 50000 ) );
  assert ( ! b2 . changeName ( "joe", "Joe", "Black" ) );
  assert ( ! b2 . changeEmail ( "Joe", "Black", "joe" ) );
  assert ( ! b2 . setSalary ( "Joe", "Black", 90000 ) );
  assert ( ! b2 . setSalary ( "joe", 90000 ) );
  assert ( b2 . getSalary ( "Joe", "Black" ) ==  0 );
  assert ( b2 . getSalary ( "joe" ) ==  0 );
  assert ( ! b2 . getRank ( "Joe", "Black", lo, hi ) );
  assert ( ! b2 . getRank ( "joe", lo, hi ) );
  assert ( ! b2 . changeName ( "joe", "Joe", "Black" ) );
  assert ( ! b2 . changeEmail ( "Joe", "Black", "joe" ) );
  assert ( ! b2 . del ( "Joe", "Black" ) );
  assert ( ! b2 . del ( "joe" ) );
  assert ( ! b2 . changeName ( "james2", "James", "Bond" ) );
  assert ( ! b2 . changeEmail ( "Peter", "Smith", "james" ) );
  assert ( ! b2 . changeName ( "peter", "Peter", "Smith" ) );
  assert ( ! b2 . changeEmail ( "Peter", "Smith", "peter" ) );
  assert ( b2 . del ( "Peter", "Smith" ) );
  assert ( ! b2 . changeEmail ( "Peter", "Smith", "peter2" ) );
  assert ( ! b2 . setSalary ( "Peter", "Smith", 35000 ) );
  assert ( b2 . getSalary ( "Peter", "Smith" ) ==  0 );
  assert ( ! b2 . getRank ( "Peter", "Smith", lo, hi ) );
  assert ( ! b2 . changeName ( "peter", "Peter", "Falcon" ) );
  assert ( ! b2 . setSalary ( "peter", 37000 ) );
  assert ( b2 . getSalary ( "peter" ) ==  0 );
  assert ( ! b2 . getRank ( "peter", lo, hi ) );
  assert ( ! b2 . del ( "Peter", "Smith" ) );
  assert ( ! b2 . del ( "peter" ) );
  assert ( b2 . add ( "Peter", "Smith", "peter", 40000 ) );
  assert ( b2 . getSalary ( "peter" ) ==  40000 );

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
