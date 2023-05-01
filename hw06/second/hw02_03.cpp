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

class Name
{
	public:
		Name () {}
		Name ( string name, string surname, unsigned int salary ) : n ( name ), s ( surname ), salary ( salary ) {}
		string n, s;
		unsigned int salary;
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

		bool operator!=( const Name & other ) const 
		{
			return !( other.n == n && other.s == s );
		}
		void print (  ) const
		{
			cout << s << " " << n << " : " << salary << endl;
		}
		int size () { return -1; }
		friend ostream & operator<<( ostream & os, const Name & n ) 
		{
			return os << n.n << " " << n.s << " " << n.salary;
		}
};


template <typename T, typename U>
struct Node
{
	T key;
	U value;
	Node<T, U> * left;
	Node<T, U> * right;
};

template <typename T, typename U>
class Map 
{
	public:
		Node<T, U> * root;
		Map( ) 
		{
			root = nullptr;
		}

		~Map( ) 
		{
			clear ( root );
		}

		Node<T, U> * addNode ( Node<T, U> * root, T key, U value ) 
		{
			return insert ( root, key, value );
		}

		string getEmail ( T key ) const
		{
			Node<T, U> * node = getHelper ( root, key );
			if ( node != nullptr ) return node->value;
			return "nullptrdefinitelyrandomlolxd"s;
		}

		Name getName ( T key ) const
		{
			Node<T, U> * node = getHelper ( root, key );
			if ( node != nullptr ) return node->value;
			return Name ( "helo", "vorld", 69420666 );
		}

		int getKey ( T key ) const
		{
			Node<T, U> * node = getHelper ( root, key );
			if ( node != nullptr ) return node->key.salary;
			return -1;
		}

		Node<T, U> * remove ( Node<T, U> * root, T key ) 
		{
			return removeNode  ( root, key );
		}

		Node<T, U> * set ( Node<T, U> * root, T key, unsigned int s )
		{
			return setHelper ( root, key, s );
		}

		Node<T, U> * set ( Node<T, U> * root, T key, U value )
		{
			return setHelper ( root, key, value );
		}

		void traverseN( const unsigned int s, int & mi, int & ma ) const
		{
			traverserN ( root, s, mi, ma );
		}

		void traverseS( const unsigned int s, int & mi, int & ma ) const
		{
			traverserS ( root, s, mi, ma );
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

	private:
		Node<T, U> * insert ( Node<T, U> * node, T key, U value )
		{
			if ( node == nullptr ) 
			{
				node = new Node<T, U>();
				node->key = key;
				node->value = value;
				node->left = nullptr;
				node->right = nullptr;
				return node;
			}
			if ( key < node->key ) node->left = insert ( node->left, key, value );
			else if ( key > node->key ) node->right = insert ( node->right, key, value );
			else node->value = value;
			return node;
		}

		Node<T, U> * setHelper ( Node<T, U> * node, T key, unsigned int s )
		{
			if ( key < node->key ) node->left = setHelper ( node->left, key, s );
			else if ( key > node->key ) node->right = setHelper ( node->right, key, s );
			else node->key.salary = s;
			return node;
		}

		Node<T, U> * setHelper ( Node<T, U> * node, T key, U value )
		{
			if ( key < node->key ) node->left = setHelper ( node->left, key, value );
			else if ( key > node->key ) node->right = setHelper ( node->right, key, value );
			else node->value = value;
			return node;
		}

		Node<T, U> * getHelper ( Node<T, U> * node, T key ) const
		{
			if ( node == nullptr ) return nullptr;
			if ( key < node->key ) return getHelper ( node->left, key );
			else if ( key > node->key ) return getHelper ( node->right, key );
			else return node;
			
		}

		Node<T, U> * removeNode ( Node<T, U> * node, T key ) 
		{
			if ( node == nullptr ) return nullptr;

			if ( key < node->key ) node->left = removeNode ( node->left, key );
			else if ( key > node->key ) node->right = removeNode (node->right, key );
			else 
			{
				if ( node->left == nullptr ) 
				{
				    Node<T, U> * tmp = node->right;
				    delete node;
				    return tmp;
				} 
				else if (node->right == nullptr) 
				{
				    Node<T, U> * tmp = node->left;
				    delete node;
				    return tmp;
				}

				Node<T, U> * tmp = minValueNode ( node->right );
				node->key = tmp->key;
				node->value = tmp->value;
				node->right = removeNode ( node->right, key );
			}

			return node;
		}

		void printer ( Node<T, U> * node ) const
		{
			if ( node != nullptr ) 
			{
				printer ( node->left );
				cout << node->key << ": " << node->value << endl;
				printer ( node->right );
			}
		}

		void traverserN ( Node<T, U> * node, const unsigned int s, int & mi, int & ma ) const
		{
			if ( node != nullptr ) 
			{
				if ( node->key.salary < s && node->key.salary > 0 ) mi++;
				else if ( node->key.salary == s ) ma++;
				traverserN ( node->left, s, mi, ma );
				traverserN ( node->right, s, mi, ma );
			}
		}

		void traverserS ( Node<T, U> * node, const unsigned int s, int & mi, int & ma ) const
		{
			if ( node != nullptr ) 
			{
				if ( node->key.salary < s && node->key.salary > 0 ) mi++;
				else if ( node->key.salary == s ) ma++;
				traverserS ( node->left, s, mi, ma );
				traverserS ( node->right, s, mi, ma );
			}
		}

		void clear( Node<T, U> * node ) 
		{
			if ( node != nullptr ) 
			{
				clear ( node->left );
				clear ( node->right );
				delete node;
			}
		}

		Node<T, U> * minValueNode ( Node<T, U> * node ) 
		{
			Node<T, U> * current = node;
			while (current->left != nullptr) current = current->left;
			return current;
		}
};

class CPersonalAgenda
{
	public:
		CPersonalAgenda  ( ) { size = 0; }
		~CPersonalAgenda ( ) { }
		bool add ( const string & name, const string & surname, const string & email, unsigned int salary )
		{
			Name wholeName ( name, surname, salary );
			if ( names.getKey( wholeName ) != -1 || emails.getName( email ) != Name ( "helo", "vorld", 69420666 ) ) return false;
			names.root = names.addNode ( names.root, wholeName, email );
			emails.root = emails.addNode ( emails.root, email, wholeName );
			push( n, wholeName );
			size++;
			return true;
		}
		bool del ( const string & name, const string & surname )
		{
			if ( size == 1 ) { emails.clear(); names.clear(); size = 0; return true; }
			string em = names.getEmail( Name ( name, surname, 666 ) );
			if ( em == "nullptrdefinitelyrandomlolxd"s ) return false;
			names.root = names.remove ( names.root, Name ( name, surname, 666 ) );
			emails.root = emails.remove ( emails.root, em );
			for (int i = 0; i < size; ++i)
				if ( n[i] == Name ( name, surname, 666 ) ) { n.erase( n.begin() + i ); break; }
			size--;
			return true;
		}
		bool del ( const string & email )
		{
			if ( size == 1 ) { emails.clear(); names.clear(); size = 0; return true; }
			Name na = emails.getName( email );
			if ( na == Name ( "helo", "vorld", 69420666 ) ) return false;
			emails.root = emails.remove ( emails.root, email );
			names.root = names.remove ( names.root, na );
			for (int i = 0; i < size; ++i )
				if ( n[i] == na ) { n.erase( n.begin() + i ); break; }
			size--;
			return true;
		}
		bool changeName ( const string & email, const string & newName, const string & newSurname )
		{
			Name na = emails.getName( email );
			unsigned int sa = na.salary;
			if ( na == Name ( "helo", "vorld", 69420666 ) || names.getEmail( Name ( newName, newSurname, 666 ) ) != "nullptrdefinitelyrandomlolxd"s  ) return false;
			emails.root = emails.remove ( emails.root, email );
			emails.root = emails.addNode ( emails.root, email, Name ( newName, newSurname, sa ) );
			names.root = names.remove ( names.root, na );
			names.root = names.addNode ( names.root, Name ( newName, newSurname, sa ), email );
			for (int i = 0; i < size; ++i )
				if ( n[i] == na ) { n.erase( n.begin() + i ); break; }
			push( n, Name ( newName, newSurname, sa ));;
			return true;
		}
		bool changeEmail ( const string & name, const string & surname, const string & newEmail )
		{
			string em = names.getEmail( Name ( name, surname, 666 ) );
			if ( em == "nullptrdefinitelyrandomlolxd"s || emails.getName ( newEmail ) != Name ( "helo", "vorld", 69420666 ) ) return false;
			unsigned int sa = emails.getName ( em ).salary;
			names.root = names.remove ( names.root, Name ( name, surname, 666 ) );
			names.root = names.addNode( names.root, Name ( name, surname, sa ), newEmail );
			emails.root = emails.remove ( emails.root, em );
			emails.root = emails.addNode( emails.root, newEmail, Name ( name, surname, sa ) );
			return true;
		}
		bool setSalary ( const string & name, const string & surname, unsigned int salary )
		{
			string em = names.getEmail( Name ( name, surname, 666 ) );
			if ( em == "nullptrdefinitelyrandomlolxd"s ) return false;
			names.root = names.set ( names.root, Name ( name, surname, 666 ), salary );
			emails.root = emails.set ( emails.root, em, Name ( name, surname, salary ) );
			return true;
		}
		bool setSalary ( const string & email, unsigned int salary )
		{
			Name na = emails.getName( email );
			if ( na ==  Name ( "helo", "vorld", 69420666 ) ) return false;
			names.root = names.set ( names.root, na, salary );
			emails.root = emails.set ( emails.root, email, Name ( na.n, na.s, salary ) );
			return true;
		}
		unsigned int getSalary ( const string & name, const string & surname ) const
		{	
			auto x = names.getKey( Name ( name, surname, 666 ) );
			if ( x == -1 ) return false;
			return x;
		}
		unsigned int getSalary ( const string & email ) const
		{
			Name x = emails.getName( email );
			if ( x == Name ( "helo", "vorld", 69420666 ) ) return false;
			return x.salary;
		}
		bool getRank ( const string & name, const string & surname, int & rankMin, int & rankMax ) const
		{
			int x = names.getKey( Name ( name, surname, 666 ) );
			if ( x == -1 ) return false;
			rankMin = 0, rankMax = 0;
			names.traverseN ( x, rankMin, rankMax );
			rankMax += rankMin - 1;
			return true;
		}
		bool getRank ( const string & email, int & rankMin, int & rankMax ) const
		{
			Name x = emails.getName( email );
			if ( x == Name ( "helo", "vorld", 69420666 ) ) return false;
			rankMin = 0, rankMax = 0;
			names.traverseS ( x.salary, rankMin, rankMax );
			rankMax += rankMin - 1;
			return true;
		}
		bool getFirst ( string & outName, string & outSurname ) const
		{
			if ( size == 0 ) return false;
			Node<Name, string> * current = names.root;
			while (current->left != nullptr) current = current->left;
			outName = current->key.n;
			outSurname = current->key.s;
			return true;
		}
		bool getNext ( const string & name, const string & surname, string & outName, string & outSurname ) const
		{
			int pos = binSearch ( n, Name ( name, surname, 666 ) );
			if ( pos == -1 || pos == size - 1 ) return false;
			outName = n[pos+1].n;
			outSurname = n[pos+1].s;
			return true;
		}
		void print ( ) const
		{
			names.print();
			cout << endl;
			emails.print();
		}
	private:
		Map<Name, string> names;
		Map<string, Name> emails;
		vector<Name> n;
		int size;
		void push( vector<Name> & n, const Name & na )
		{
			for (int i = 0; i < size; ++i)
				if ( n[i] > na ) { n.insert( n.begin() + i, na ); return; }
			n.push_back( na );
		}
		int binSearch ( const vector<Name> v, const Name & p ) const
		{
			size_t h = size, l = 0;
			while ( h > l )
			{
				size_t mid = l + ( h - l ) / 2;
				if ( p == v[mid] ) return mid;
				if ( p > v[mid] ) l = mid + 1;
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
