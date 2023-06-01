#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>
using namespace std;

class CTimeTester;
#endif /* __PROGTEST__ */

class CTime
{
private:
    int m_Hour;
    int m_Minute;
    int m_Second;
    void round ( )
    {
    	if ( m_Second >= 60 )
    	{
    		m_Minute += ( m_Second / 60 );
			m_Second %= 60;
			m_Hour += ( m_Minute / 60 );
			m_Minute %= 60;
			m_Hour %= 24;
    	}
    	else if ( m_Second < 0 )
    	{  
            int q = 0;
			while ( m_Second < 0 ) { q++; m_Second += 60; }
            m_Minute -= q;
            q = 0;
            while ( m_Minute < 0 ) { q++; m_Minute += 60; }
            m_Hour -= q;
            while ( m_Hour < 0 ) { m_Hour += 24; }
    	}
    }
public:
	CTime ()
    {
    	m_Hour = 0;
    	m_Minute = 0;
    	m_Second = 0;
    }
    CTime ( int h, int m )
    {
    	if ( h > 23 || m > 59 || h < 0 || m < 0 )
    		throw invalid_argument ( "Error : Invalid argument" );
    	m_Hour = h;
    	m_Minute = m;
    	m_Second = 0;
    }

    CTime ( int h, int m, int s )
    {
    	if ( h > 23 || m > 59 || s > 59 || h < 0 || m < 0 || s < 0 )
    		throw invalid_argument ( "Error : Invalid argument" );
    	m_Hour = h;
    	m_Minute = m;
    	m_Second = s;
    }
    CTime ( const CTime & t )
    {
    	m_Hour = t.m_Hour;
    	m_Minute = t.m_Minute;
    	m_Second = t.m_Second;
    }
    CTime & operator = ( const CTime & t )
    {
    	m_Hour = t.m_Hour;
    	m_Minute = t.m_Minute;
    	m_Second = t.m_Second;
    	return *this;
    }
    CTime operator + ( int s ) const
    {
        CTime r ( m_Hour, m_Minute, m_Second );
        r.m_Second += s;
        r.round();
        return r;
    }
    CTime & operator += ( int s )
    {
    	m_Second += s;
    	this->round();
    	return *this;
    }
    CTime operator - ( int s ) const
    {
    	CTime r ( m_Hour, m_Minute, m_Second );
    	r.m_Second -= s;
		r.round();
		return r;
    }
    CTime & operator -= ( int s )
    {
    	m_Second -= s;
    	this->round();
    	return *this;
    }
    int operator - ( const CTime & t ) const
    {
    	if ( *this < t )
    		return t.m_Hour*3600 + t.m_Minute*60 +t.m_Second - ( m_Hour*3600 + m_Minute*60 + m_Second );
    	return m_Hour*3600 + m_Minute*60 + m_Second - ( t.m_Hour*3600 + t.m_Minute*60 +t.m_Second );
    }

    
    CTime & operator ++ () { m_Second++; this->round (  ); return *this; }//prefix
    CTime & operator -- () { m_Second--; this->round (  ); return *this; }//prefix
    CTime operator ++ ( int ) 
    { 
        CTime t ( *this );
        m_Second++; 
        this->round(); 
        return t;
    }
    CTime operator -- ( int )
    { 
        CTime t ( *this );
        m_Second--; 
        this->round(); 
        return t;
    }
    bool operator < ( const CTime & t ) const
    {  
    	if ( m_Hour < t.m_Hour ) return true;
    	if ( m_Hour > t.m_Hour ) return false;
    	if ( m_Minute < t.m_Minute ) return true;
    	if ( m_Minute > t.m_Minute ) return false;
    	if ( m_Second < t.m_Second ) return true;
    	if ( m_Second > t.m_Second ) return false;
    	return false;
    }
    bool operator == ( const CTime & t ) const
    {  
    	return ( t.m_Hour == m_Hour && t.m_Minute == m_Minute && t.m_Second == m_Second );
    }
    bool operator != ( const CTime & t ) const
    {  
    	return !( *this == t );
    }
    bool operator <= ( const CTime & t ) const
    {  
    	if ( t == *this ) return true;
    	return ( *this < t );
    }
    bool operator > ( const CTime & t ) const
    {  
    	return ! ( *this <= t ); 
    }
    bool operator >= ( const CTime & t ) const
    {  
    	return ! ( *this < t );
    }
    friend ostream& operator<<(ostream & os, const CTime & t);
    friend ostringstream& operator<<(ostringstream& oss, const CTime& t);
    friend class ::CTimeTester;
    ~CTime () {}
};

ostream & operator<< (ostream& oss, const CTime& t)
{
    oss <<setfill(' ')<<setw(2)<< t.m_Hour << ':' <<
          setfill('0')<<setw(2)<< t.m_Minute << ':' <<
          setfill('0')<<setw(2)<< t.m_Second;
    return oss;
}

ostringstream& operator<<(ostringstream& oss, const CTime& t)
{
	oss <<setfill(' ')<<setw(2)<< t.m_Hour << ':' <<
		  setfill('0')<<setw(2)<< t.m_Minute << ':' <<
		  setfill('0')<<setw(2)<< t.m_Second;
	return oss;
}

#ifndef __PROGTEST__
struct CTimeTester
{
    static bool test ( const CTime & time, int hour, int minute, int second )
    {
        return time.m_Hour == hour 
            && time.m_Minute == minute
            && time.m_Second == second;
    }
};

#include <cassert>
#include <sstream>

int main ()
{
    CTime a { 12, 30 };
    assert( CTimeTester::test( a, 12, 30, 0 ) );

    CTime b { 13, 30 };
    assert( CTimeTester::test( b, 13, 30, 0 ) );

    assert( b - a == 3600 );

    assert( CTimeTester::test( a + 60, 12, 31, 0 ) );
    assert( CTimeTester::test( a - 60, 12, 29, 0 ) );

    assert( a < b );
    assert( a <= b );
    assert( a != b );
    assert( !( a > b ) );
    assert( !( a >= b ) );
    assert( !( a == b ) );

    while ( ++a != b );
    assert( a == b );

    std::ostringstream output;
    assert( static_cast<bool>( output << a ) );
    assert( output.str() == "13:30:00" );

    assert( a++ == b++ );
    assert( a == b );

    assert( --a == --b );
    assert( a == b );

    assert( a-- == b-- );
    assert( a == b );

    return 0;
}
#endif /* __PROGTEST__ */
