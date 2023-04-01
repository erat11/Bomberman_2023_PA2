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


class Person2
{
	public:
		Person2 () {}
		Person2 ( Name wholeName, string email )
		{
			this->wholeName.s = wholeName.s;
			this->wholeName.n = wholeName.n;
			this->email= email;
		}
		Name wholeName;
		string email;
};

struct Node 
{
	unsigned int amount;
	Person2 p;
	Node * left;
	Node * right;
};

class Map 
{
	public:
		Node * root;
		Map( ) 
		{
			root = nullptr;
		}

		~Map( ) 
		{
			clear ( root );
		}

		Node * addNode ( Node * root, Person2 p, unsigned int salary ) 
		{
			return insert ( root, p, salary );
		}

		int get ( string email ) const
		{
			Node * node = getHelper ( root, email );
			if ( node != nullptr ) return node->amount;
			return -1;
		}

		Name getName ( string email ) const
		{
			Node * node = getHelper ( root, email );
			if ( node != nullptr ) return node->p.wholeName;
			return Name ( "NUL", "EROR" );
		}

		Node * remove ( string p ) 
		{
			return removeNode  ( root, p );
		}

		void print() const
		{
			printer ( root );
		}

		void clear() 
		{
			clear ( root );
			root = nullptr;
		}
		Node * getRoot ( )
		{
			return root;
		}

	private:
		Node * insert ( Node * node, const Person2 & p, unsigned int salary )
		{
			if ( node == nullptr ) 
			{
				node = new Node();
				node->p = p;
				node->amount = salary;
				node->left = nullptr;
				node->right = nullptr;
				return node;
			}
			if ( p.email < node->p.email) node->left = insert ( node->left, p, salary );
			else if ( p.email > node->p.email ) node->right = insert ( node->right, p, salary );
			else node->amount = salary;
			return node;
		}

		Node * getHelper ( Node * node, string email ) const
		{
			if ( node == nullptr ) return nullptr;
			if ( email < node->p.email ) return getHelper ( node->left, email );
			else if ( email > node->p.email ) return getHelper ( node->right, email );
			else return node;
			
		}

		Node * removeNode ( Node * node, string email ) 
		{
			if ( node == nullptr ) return nullptr;

			if ( email < node->p.email ) node->left = removeNode ( node->left, email );
			else if ( email > node->p.email ) node->right = removeNode (node->right, email );
			else 
			{
				if ( node->left == nullptr ) 
				{
				    Node * tmp = node->right;
				    delete node;
				    return tmp;
				} 
				else if (node->right == nullptr) 
				{
				    Node * tmp = node->left;
				    delete node;
				    return tmp;
				}

				Node* tmp = minValueNode ( node->right );
				node->p = tmp->p;
				node->amount = tmp->amount;
				node->right = removeNode ( node->right, email );
			}

			return node;
		}

		Node * minValueNode ( Node * node ) 
		{
			Node * current = node;
			while (current->left != nullptr) current = current->left;
			return current;
		}

		void printer ( Node * node ) const
		{
			if ( node != nullptr ) 
			{
				printer ( node->left );
				cout << node->p.wholeName.n << " " << node->p.wholeName.s << " " << node->p.email << ": " << node->amount << endl;
				printer ( node->right );
			}
		}

		void clear( Node* node ) 
		{
			if ( node != nullptr ) 
			{
				clear ( node->left );
				clear ( node->right );
				delete node;
			}
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
			Person2 p2 ( wholeName, email );
			push ( p );
			emailTree.root = emailTree.addNode ( emailTree.root, p2, salary );
			size++;
			return true;
		}
		bool del ( const string & name, const string & surname )
		{
			if ( size == 1 ) { emailTree.clear(); nameSort.clear(); size = 0; return true; }
			int pos = binSearch ( nameSort, Name ( name, surname ) );
			if ( pos == -1 ) return false;
			string e = nameSort[pos].email;
			nameSort.erase( nameSort.begin() + pos );
			emailTree.root = emailTree.remove( e );
			size--;
			return true;
		}
		bool del ( const string & email )
		{
			if ( size == 1 ) { emailTree.clear(); nameSort.clear(); size = 0; return true; }
			if ( emailTree.get ( email ) == -1 ) return false;
			emailTree.root = emailTree.remove( email );
			for ( int i = 0; i < size; ++i)
				if ( nameSort[i].email == email )
				{
					nameSort.erase( nameSort.begin() + i );
					break;
				}
			size--;
			return true;
		}
		bool changeName ( const string & email, const string & newName, const string & newSurname )
		{
			unsigned int salary;
			int i;
			if ( emailTree.get ( email ) == -1 || binSearch ( nameSort, Name ( newName, newSurname ) ) != -1 ) return false;
			for ( i = 0; i < size; ++i )
				if ( email == nameSort[i].email )
				{
					nameSort[i].wholeName.n = newName;
					nameSort[i].wholeName.s = newSurname;
					salary = nameSort[i].salary;
					break;
				}
			Person2 p2( Name ( newName, newSurname ), email );
			emailTree.root = emailTree.remove( email );
			emailTree.root = emailTree.addNode ( emailTree.root, p2, salary );
			nameSort.erase( nameSort.begin() + i );
			Person p ( Name ( newName, newSurname), email, salary );
			for ( int i = 0; i < size - 1; ++i ) if ( p.wholeName < nameSort[i].wholeName  ) { nameSort.insert( nameSort.begin() + i, p ); break; }
			return true;
		}
		bool changeEmail ( const string & name, const string & surname, const string & newEmail )
		{
			if ( emailTree.get( newEmail ) != -1 ) return false;
			int pos = binSearch ( nameSort, Name ( name, surname ) );
			if ( pos == -1 ) return false;
			string oldEmail = nameSort[pos].email;
			nameSort[pos].email = newEmail;
			emailTree.remove ( oldEmail );
			Person2 p2 ( Name ( name, surname ), newEmail );
			emailTree.root = emailTree.addNode ( emailTree.root, p2, nameSort[pos].salary );
			return true;
		}
		bool setSalary ( const string & name, const string & surname, unsigned int salary )
		{
			int pos = binSearch ( nameSort, Name ( name, surname ) );
			if ( pos == -1 ) return false;
			nameSort[pos].salary = salary;
			emailTree.root = emailTree.remove( nameSort[pos].email );
			emailTree.root = emailTree.addNode( emailTree.root, Person2 ( Name ( name, surname ), nameSort[pos].email ), salary );
			return true;
		}
		bool setSalary ( const string & email, unsigned int salary )
		{
			Name n = emailTree.getName( email );
			if ( n.n == "NUL" && n.s == "EROR" ) return false;
			return setSalary ( n.n, n.s, salary );
		}
		unsigned int getSalary ( const string & name, const string & surname ) const
		{	
			int pos = binSearch ( nameSort, Name ( name, surname ) );
			if ( pos == -1 ) return false;
			return nameSort[pos].salary;
		}
		unsigned int getSalary ( const string & email ) const
		{
			int salary = emailTree.get ( email );
			if ( salary == -1 ) return false;
			return salary;
		}
		bool getRank ( const string & name, const string & surname, int & rankMin, int & rankMax ) const
		{
			rankMin = 0, rankMax = 0;
			int pos = binSearch ( nameSort, Name ( name, surname ) );
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
			int salary = emailTree.get ( email );
			if ( salary == -1 ) return false;
			for ( auto & x : nameSort )
				if ( x.salary < abs(salary) )
					rankMin++;
				else if ( x.salary == abs(salary) && x.email != email )
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
			int pos = binSearch ( nameSort, Name ( name, surname ) );
			if ( pos == -1 || pos + 1 == size ) return false;
			outName = nameSort[pos+1].wholeName.n;
			outSurname = nameSort[pos+1].wholeName.s;
			return true;
		}
		void print ( ) const
		{
			for ( auto & x : nameSort ) cout << x.wholeName.s << " " << x.wholeName.n << " " << x.email << " " << x.salary << endl;
			cout << endl;
			emailTree.print();
		}
	private:
		vector<Person> nameSort;
		int size;
		Map emailTree;
		void push ( const Person p )
		{
			int i;
			for ( i = 0; i < size; ++i )
				if ( p.wholeName < nameSort[i].wholeName ) { nameSort.insert( nameSort.begin() + i, p ); break; }
			
			if ( i == size ) nameSort.push_back( p );
		}
		int binSearch ( const vector<Person> v, const Name & p ) const
		{
			size_t h = size, l = 0;
			while ( h > l )
			{
				size_t mid = l + ( h - l ) / 2;
				if ( p == v[mid].wholeName ) return mid;
				if ( p > v[mid].wholeName ) l = mid + 1;
				else h = mid;
				
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
