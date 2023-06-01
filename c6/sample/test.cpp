#ifndef __PROGTEST__
#include <cstring>
using namespace std;

class CLinkedSetTester;
#endif /* __PROGTEST__ */

#include <iostream>


class CLinkedSet
{
private:
    struct CNode
    {
        CNode * m_Next;
        const char * Value () const
        {
        	return val;
        }
        char * val;
    };
    CNode * m_Begin;
    size_t size;
    void destroy ()
    {
    	CNode * tmp = m_Begin;
    	while ( tmp != nullptr )
    	{
    		CNode * tmp2 = tmp->m_Next;
    		delete tmp;
    		tmp = tmp2;
    	}
    }

public:
    CLinkedSet ()
    {
    	m_Begin = nullptr;
    	size = 0;
    }
    CLinkedSet ( CNode * s )
    {
    	m_Begin = s;
    	size = 0;
    }
    CLinkedSet ( const CLinkedSet & s )
    {
    	CNode * tmp = s.first();
    	CLinkedSet r;
    	memcpy ( m_Begin->val, s.first()->Value(), strlen(s.first()->Value())+1 );
    	m_Begin->m_Next = nullptr;
    	while ( tmp != nullptr )
    	{
    		tmp = tmp->m_Next;
    		r.Insert ( tmp->Value() );
    	}
    	size = 0;

    }
    CLinkedSet operator = ( const CLinkedSet & s )
    {
    	return CLinkedSet ( s );
    }
    CNode * first() const
    {
    	return m_Begin;
    }
    ~CLinkedSet () 
    {
    	destroy();
    }

    bool Insert ( const char * value )
    { 
    	if ( Contains ( value ) ) return false;
    	CNode * x = new CNode;
    	x->val = (char*)value;
    	x->m_Next = nullptr;
    	if ( this->Size() == 0 )
    	{
    		m_Begin = x;
    		size++;
    		return true;
    	}
    	CNode * tmp = m_Begin->m_Next, *tmpLast = m_Begin;
    	while ( tmp->m_Next != nullptr ) 
    	{
    		if ( strcmp( tmpLast->Value(), value ) > 0 && strcmp( tmp->Value(), value ) < 0 ) 
			{
				tmpLast->m_Next = x;
				x->m_Next = tmp;
				size++;
				return true;
			}
    		tmp = tmp->m_Next; 
    		tmpLast = tmpLast->m_Next; 
    	}
    	tmp->m_Next = x;
    	size++;
    	return true;
    }
    
    bool Remove ( const char * value )
    {
    	if ( this->Size() == 0 ) return false;
    	if ( strcmp( m_Begin->Value(), value ) == 0 ) 
    	{ 
    		CNode * tmp = m_Begin->m_Next; 
    		delete m_Begin; 
    		m_Begin = tmp;
    		size--; 
    		return true; 
    	}
    	CNode * tmp = m_Begin, *tmpLast = m_Begin;
    	while ( tmp != nullptr )
    	{
    		tmp = tmp->m_Next;
    		if ( strcmp( tmp->Value(), value ) == 0 )
    		{
    			tmpLast->m_Next = tmp->m_Next;
    			delete tmp;
    			size--;
    			return true;
    		}
    		tmpLast = tmpLast->m_Next;
    	}
    	return false;
    }

    bool Empty () const
    {
    	return ( size == 0 );
    }

    size_t Size () const
    {
    	return size;
    }

    bool Contains ( const char * value ) const
    {
    	CNode * tmp = m_Begin;
    	while ( tmp != nullptr )
    	{
    		if ( strcmp( tmp->Value(), value ) == 0 ) return true;
    		tmp = tmp->m_Next;
    	}
    	return false;
    }
    void print () const 
    {
    	CNode * tmp = m_Begin;
    	while ( tmp != nullptr )
    	{
    		cout << tmp->Value() << endl;
    		tmp = tmp->m_Next;
    	}
    }
    friend class ::CLinkedSetTester;
};

#ifndef __PROGTEST__
#include <cassert>

struct CLinkedSetTester
{
    static void test0()
    {
        CLinkedSet x0;
        assert( x0 . Insert( "Jerabek Michal" ) );
        assert( x0 . Insert( "Pavlik Ales" ) );
        /*assert( x0 . Insert( "Dusek Zikmund" ) );
        assert( x0 . Size() == 3 );
        assert( x0 . Contains( "Dusek Zikmund" ) );
        assert( !x0 . Contains( "Pavlik Jan" ) );
        x0.print();*/
        /*assert( x0 . Remove( "Jerabek Michal" ) );
        assert( !x0 . Remove( "Pavlik Jan" ) );
        assert( !x0 . Empty() );*/
    }

    /*static void test1()
    {
        CLinkedSet x0;
        assert( x0 . Insert( "Jerabek Michal" ) );
        assert( x0 . Insert( "Pavlik Ales" ) );
        assert( x0 . Insert( "Dusek Zikmund" ) );
        assert( x0 . Size() == 3 );
        assert( x0 . Contains( "Dusek Zikmund" ) );
        assert( !x0 . Contains( "Pavlik Jan" ) );
        assert( x0 . Remove( "Jerabek Michal" ) );
        assert( !x0 . Remove( "Pavlik Jan" ) );
        CLinkedSet x1 ( x0 );
        assert( x0 . Insert( "Vodickova Saskie" ) );
        assert( x1 . Insert( "Kadlecova Kvetslava" ) );
        assert( x0 . Size() == 3 );
        assert( x1 . Size() == 3 );
        assert( x0 . Contains( "Vodickova Saskie" ) );
        assert( !x1 . Contains( "Vodickova Saskie" ) );
        assert( !x0 . Contains( "Kadlecova Kvetslava" ) );
        assert( x1 . Contains( "Kadlecova Kvetslava" ) );
    }

    static void test2()
    {
        CLinkedSet x0;
        CLinkedSet x1;
        assert( x0 . Insert( "Jerabek Michal" ) );
        assert( x0 . Insert( "Pavlik Ales" ) );
        assert( x0 . Insert( "Dusek Zikmund" ) );
        assert( x0 . Size() == 3 );
        assert( x0 . Contains( "Dusek Zikmund" ) );
        assert( !x0 . Contains( "Pavlik Jan" ) );
        assert( x0 . Remove( "Jerabek Michal" ) );
        assert( !x0 . Remove( "Pavlik Jan" ) );
        x1 = x0;
        assert( x0 . Insert( "Vodickova Saskie" ) );
        assert( x1 . Insert( "Kadlecova Kvetslava" ) );
        assert( x0 . Size() == 3 );
        assert( x1 . Size() == 3 );
        assert( x0 . Contains( "Vodickova Saskie" ) );
        assert( !x1 . Contains( "Vodickova Saskie" ) );
        assert( !x0 . Contains( "Kadlecova Kvetslava" ) );
        assert( x1 . Contains( "Kadlecova Kvetslava" ) );
    }*/

};

int main ()
{
    CLinkedSetTester::test0();
    //CLinkedSetTester::test1();
    //CLinkedSetTester::test2();
    return 0;
}
#endif /* __PROGTEST__ */
