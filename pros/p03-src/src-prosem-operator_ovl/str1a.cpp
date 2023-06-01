#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <sys/time.h>
using namespace std;


// a demonstration of the speed of CStr

class CStr
{
  public:
                             CStr                ( const char * Str = "" );
    explicit                 CStr                ( double X );
    explicit                 CStr                ( int X );
                             CStr                ( const CStr & x );
                            ~CStr                ( void );
    CStr                   & operator =          ( const CStr & x );
    int                      Length              ( void ) const;
    CStr                   & operator +=         ( const CStr & x );
    CStr                   & operator +=         ( const char * x );
    CStr                   & operator +=         ( int x );
    CStr                   & operator +=         ( double x );
                             operator const char * ( void ) const; // consider explicit in C++11
    char                   & operator []         ( int idx );

  protected:
    int                      m_Len;
    int                      m_Max;
    char                   * m_Data;

    void                     append              ( const char * src, int srclen );

    friend CStr              operator +          ( const CStr & a, const CStr & b );
    friend std::ostream    & operator <<         ( std::ostream & os, const CStr & x );
};

CStr               operator +                    ( const CStr & a, const CStr & b );
std::ostream     & operator <<                   ( std::ostream & os, const CStr & x );



//-------------------------------------------------------------------------------------------------
                   CStr::CStr                    ( const char * Str )
                     : m_Len ( strlen ( Str ) ),
                       m_Max ( m_Len + 1 ),
                       m_Data ( new char[m_Max] )
{
  strncpy ( m_Data, Str, m_Max );
}
//-------------------------------------------------------------------------------------------------
                   CStr::CStr                    ( double X )
                     : m_Len ( 0 ),
                       m_Max ( 20 ),
                       m_Data ( new char [m_Max] )
{
 *m_Data    = 0;
  operator += ( X );
}
//-------------------------------------------------------------------------------------------------
                   CStr::CStr                    ( int X )
                     : m_Len ( 0 ),
                       m_Max ( 20 ),
                       m_Data ( new char [m_Max] )
{
 *m_Data    = 0;
  operator += ( X );
}
//-------------------------------------------------------------------------------------------------
                   CStr::CStr                    ( const CStr & src )
                     : m_Len ( src . m_Len ),
                       m_Max ( src . m_Max ),
                       m_Data ( new char [m_Max] )
{
  memcpy    ( m_Data, src . m_Data, m_Len + 1 );
}
//-------------------------------------------------------------------------------------------------
                   CStr::~CStr                   ( void )
{
  delete [] m_Data;
}
//-------------------------------------------------------------------------------------------------
CStr             & CStr::operator =              ( const CStr & x )
{
  if ( &x != this )
  {
    delete [] m_Data;
    m_Max   = x . m_Max;
    m_Len   = x . m_Len;
    m_Data  = new char [m_Max];
    memcpy  ( m_Data, x . m_Data, m_Len + 1 );
  }
  return *this;
}
//-------------------------------------------------------------------------------------------------
int                CStr::Length                  ( void ) const
{
  return m_Len;
}
//-------------------------------------------------------------------------------------------------
CStr             & CStr::operator +=             ( const CStr & x )
{
  append ( x . m_Data, x . m_Len );
  return *this;
}
//-------------------------------------------------------------------------------------------------
CStr             & CStr::operator +=             ( const char * x )
{
  append ( x, strlen ( x ) );
  return *this;
}
//-------------------------------------------------------------------------------------------------
CStr             & CStr::operator +=             ( int x )
{
  char tmp [20];

  snprintf  ( tmp, sizeof ( tmp ), "%d", x );   // will not use ostringstream
  append    ( tmp, strlen ( tmp ) );
  return    *this;
}
//-------------------------------------------------------------------------------------------------
CStr             & CStr::operator +=             ( double x )
{
  char tmp [20];

  snprintf ( tmp, sizeof ( tmp ), "%f", x );
  append ( tmp, strlen ( tmp ) );
  return *this;
}
//-------------------------------------------------------------------------------------------------
                   CStr::operator const char *   ( void ) const
{
  return m_Data;
}
//-------------------------------------------------------------------------------------------------
char             & CStr::operator []             ( int idx )
{
  if ( idx < 0 || idx >= m_Len )
    throw "index out of range";

  return m_Data[idx];
}
//-------------------------------------------------------------------------------------------------
void              CStr::append                   ( const char * src, int srclen )
{
  if ( m_Len + srclen + 1 >= m_Max )
  {
    m_Max = m_Len + srclen + 1;
    char * tmp = new char [m_Max];
    memcpy ( tmp, m_Data, m_Len );
    memcpy ( tmp + m_Len, src, srclen );
    delete [] m_Data; // safe even when m_Data == src
    m_Data = tmp;
  }
  else
    memcpy         ( m_Data + m_Len, src, srclen );
  m_Len         += srclen;
  m_Data[m_Len]  = 0;
}
//-------------------------------------------------------------------------------------------------
CStr              operator +                     ( const CStr & a, const CStr & b )
{
  CStr res ( a );
  res += b;
  return ( res );
}
//-------------------------------------------------------------------------------------------------
std::ostream    & operator <<                    ( std::ostream & os, const CStr & x )
{
  os << x . m_Data;
  return os;
}
//=================================================================================================
double             getTimeStamp                  ( void )
{
  struct timeval tv;
  gettimeofday ( &tv, nullptr );
  return tv . tv_sec + tv . tv_usec / 1000000.0;
}
//-------------------------------------------------------------------------------------------------
CStr               randomString                  ( const char * randStr, int len )
{
  int x = (int)(0.5 * rand () * len / RAND_MAX);
  return CStr ( randStr + x );
}
//=================================================================================================
int                main                          ( int argc, char * argv [] )
{
  const int RAND_LEN = 30000;
  char  randSeed [RAND_LEN];

  // create an array of random bytes
  for ( int i = 0; i < RAND_LEN - 1; i ++ )
    randSeed[i] = 'a' + (int)(rand () * 26.0 / RAND_MAX );
  randSeed[RAND_LEN - 1] = 0;


  const int ARRAY_SIZE = 50000;
  // fill vector with some random strings,
  // initial capacity 50000
  vector<CStr> vec ( ARRAY_SIZE );

  double timeSt = getTimeStamp ();
  for ( int i = 0; i < ARRAY_SIZE; i ++ )
    vec[i] = randomString ( randSeed, RAND_LEN );

  for ( int i = 0, j = ARRAY_SIZE - 1; i < j; i ++, j -- )
    swap ( vec[i], vec[j] );


  cout << "vector: " <<  ARRAY_SIZE / 2 << " swaps: " << ( getTimeStamp () - timeSt ) << " sec" << endl;

  CStr * arr = new CStr [ARRAY_SIZE];

  // the same with array of CStr
  timeSt = getTimeStamp ();
  for ( int i = 0; i < ARRAY_SIZE; i ++ )
    arr[i] = randomString ( randSeed, RAND_LEN );

  for ( int i = 0, j = ARRAY_SIZE - 1; i < j; i ++, j -- )
    swap ( arr[i], arr[j] );


  cout << "raw array: " << ARRAY_SIZE / 2 << " swaps: " << ( getTimeStamp () - timeSt ) << " sec" << endl;
  delete [] arr;

  return 0;
}
//-------------------------------------------------------------------------------------------------
