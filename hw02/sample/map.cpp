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

		U get ( T key ) const
		{
			Node<T, U> * node = getHelper ( root, key );
			if ( node != nullptr ) return node->value;
			return -1;
		}

		Node<T, U> * remove ( T key ) 
		{
			return removeNode  ( root, key );
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
		Node<T, U> * getRoot ( )
		{
			return root;
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

		Node<T, U> * minValueNode ( Node<T, U> * node ) 
		{
			Node<T, U> * current = node;
			while (current->left != nullptr) current = current->left;
			return current;
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

		void clear( Node<T, U> * node ) 
		{
			if ( node != nullptr ) 
			{
				clear ( node->left );
				clear ( node->right );
				delete node;
			}
		}
};

int main (  )
{
	Map<int, string> m;
	m.root = m.addNode ( m.root, 5, "klfjslkfjs"s );
	m.print();
	return 0;
}