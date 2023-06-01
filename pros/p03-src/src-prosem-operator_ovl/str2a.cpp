#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include <sys/time.h>
using namespace std;

/* String implementation, counted references. The strings use shared holder objects (TData).
 * These shared data are copied only when needed - i.e. if the contents is to be modified or
 * if the last CStrAuto releases the string (copy-on-write).
 *
 * This modification adds move semantic (c++11). Note the implementation of move and copy
 * constructor is the same - the cost of integer increment/decrement is comparable to the
 * cost of setting a member to nullptr. Move semantic of c++11 does not help much in this
 * implementation.
 */

class CStrAuto
{
  public:
                             CStrAuto            ( const char * str = "" );
    explicit                 CStrAuto            ( double x );
                             CStrAuto            ( int x );
                             CStrAuto            ( const CStrAuto & x );
                             CStrAuto            ( CStrAuto && x ) noexcept;
                            ~CStrAuto            ( void ) noexcept;
    CStrAuto               & operator =          ( const CStrAuto & x );
    CStrAuto               & operator =          ( CStrAuto && x );
    int                      Length              ( void ) const;
    CStrAuto               & operator +=         ( const CStrAuto & x );
    CStrAuto               & operator +=         ( const char * x );
    CStrAuto               & operator +=         ( int x );
    CStrAuto               & operator +=         ( double x );
                             operator const char * ( void ) const;
    char                     operator []         ( int idx ) const;
    void                     SetCharAt           ( int idx, char X );

  private:
    struct TData
    {
                             TData               ( const char * str,
                                                   int reserve );
                             TData               ( const TData & src,
                                                   int reserve );
                            ~TData               ( void );
      void                   Append              ( const char * src,
                                                   int srcLen );

      int                    m_RefCnt;
      int                    m_Len;
      int                    m_Max;
      char                 * m_Str;
    };

    TData                  * m_Data;


    void                     append              ( const char * src, int srclen );
    void                     detach              ( int newMaxSize );

    friend CStrAuto          operator +          ( const CStrAuto & a, const CStrAuto & b );
    friend ostream         & operator <<         ( ostream & os, const CStrAuto & x );
};

CStrAuto           operator +                    ( const CStrAuto & a, const CStrAuto & b );
ostream          & operator <<                   ( ostream & os, const CStrAuto & x );

//=================================================================================================
                   CStrAuto::TData::TData        ( const char * str,
                                                   int reserve )
                   : m_RefCnt ( 1 ),
                     m_Len ( strlen ( str ) ),
                     m_Max ( reserve ),
                     m_Str ( new char[m_Max] )
{
  memcpy    ( m_Str, str, m_Len + 1 ); // + 1 = zero terminating character
}
//-------------------------------------------------------------------------------------------------
                   CStrAuto::TData::TData        ( const TData & src,
                                                   int reserve )
                   : m_RefCnt ( 1 ),
                     m_Len ( src . m_Len ),
                     m_Max ( reserve ),
                     m_Str ( new char[m_Max] )
{
  memcpy ( m_Str, src . m_Str, m_Len + 1 );
}
//-------------------------------------------------------------------------------------------------
                   CStrAuto::TData::~TData       ( void )
{
  delete [] m_Str;
}
//-------------------------------------------------------------------------------------------------
void              CStrAuto::TData::Append        ( const char * src,
                                                   int srcLen )
{
  if ( m_Len + srcLen + 1 > m_Max )
  {
    // increase the size. If m_Max is small, just increase by 10 chars
    // if m_Max is big enough, increase the size by a factor of 1.5
    while (  m_Len + srcLen + 1 > m_Max )
      m_Max += m_Max < 100 ? 10 : m_Max / 2;

    char * tmp = new char [m_Max];
    memcpy ( tmp, m_Str, m_Len );
    memcpy ( tmp + m_Len, src, srcLen );
    delete [] m_Str;  // safe even if m_Str == src (can be the case of a += a)
    m_Str = tmp;
  }
  else
    memcpy ( m_Str + m_Len, src, srcLen );
  m_Len += srcLen;
  m_Str[m_Len] = 0;
}
//=================================================================================================
                   CStrAuto::CStrAuto            ( const char * str )
                   : m_Data ( new TData ( str, strlen ( str ) + 1 ) )
{
}
//-------------------------------------------------------------------------------------------------
                   CStrAuto::CStrAuto            ( double X )
                   : m_Data ( new TData ( "", 30 ) )
{
  operator += ( X );
}
//-------------------------------------------------------------------------------------------------
                   CStrAuto::CStrAuto            ( int X )
                   : m_Data ( new TData ( "", 20 ) )
{
  operator += ( X );
}
//-------------------------------------------------------------------------------------------------
                   CStrAuto::CStrAuto            ( const CStrAuto & x )
                   : m_Data ( x . m_Data )
{
  m_Data -> m_RefCnt ++;
}
//-------------------------------------------------------------------------------------------------
                   CStrAuto::CStrAuto            ( CStrAuto && x ) noexcept
                   : m_Data ( x . m_Data )
{
  m_Data -> m_RefCnt ++;
}
//-------------------------------------------------------------------------------------------------
                   CStrAuto::~CStrAuto           ( void ) noexcept
{
  if ( -- m_Data -> m_RefCnt == 0 ) 
    delete m_Data;
}
//-------------------------------------------------------------------------------------------------
CStrAuto         & CStrAuto::operator =          ( const CStrAuto & x )
{
  if ( &x != this )
  {
    if ( -- m_Data -> m_RefCnt == 0 )
      delete m_Data;
    m_Data = x . m_Data;
    m_Data -> m_RefCnt ++;
  }
  return *this;
}
//-------------------------------------------------------------------------------------------------
CStrAuto         & CStrAuto::operator =          ( CStrAuto && x )
{
  swap ( m_Data, x . m_Data );
  return *this;
}
//-------------------------------------------------------------------------------------------------
int                CStrAuto::Length              ( void ) const
{
  return m_Data -> m_Len;
}
//-------------------------------------------------------------------------------------------------
CStrAuto         & CStrAuto::operator +=         ( const CStrAuto & x )
{
  append ( x . m_Data -> m_Str, x . m_Data -> m_Len );
  return *this;
}
//-------------------------------------------------------------------------------------------------
CStrAuto         & CStrAuto::operator +=         ( const char * x )
{
  append ( x, strlen ( x ) );
  return *this;
}
//-------------------------------------------------------------------------------------------------
CStrAuto         & CStrAuto::operator +=         ( int x )
{
  char tmp [20];

  snprintf ( tmp, sizeof ( tmp ), "%d", x );
  append ( tmp, strlen ( tmp ) );
  return *this;
}
//-------------------------------------------------------------------------------------------------
CStrAuto         & CStrAuto::operator +=         ( double x )
{
  char tmp [30];

  snprintf ( tmp, sizeof ( tmp ), "%f", x );
  append ( tmp, strlen ( tmp ) );
  return *this;
}
//-------------------------------------------------------------------------------------------------
                   CStrAuto::operator const char * ( void ) const
{
  return m_Data -> m_Str;
}
//-------------------------------------------------------------------------------------------------
char               CStrAuto::operator []         ( int idx ) const
{
  // This operator no longer returns char &. I.e. whis operator result cannot be the left-hand side of
  // an assignment. Thus the operator can be used here:
  //  char x = str[1];
  // whereas this is forbidden:
  //  str[1] = 'A';
  // to allow the latter use, we would have to copy the content (if shared). Since we cannot distinguish
  // the intended read/write use inside the operator and we do not want to do a preemptive copy of the contents
  // when reading, we simply create an extra method to support the writing and leave op [] just for reading.
  if ( idx < 0 || idx >= m_Data -> m_Len )
   throw "index out of range";

  return m_Data -> m_Str[idx];
}
//-------------------------------------------------------------------------------------------------
void               CStrAuto::SetCharAt           ( int idx, char X )
{
  if ( idx < 0 || idx >= m_Data -> m_Len )
    throw "index out of range";

  detach ( m_Data -> m_Max );
  m_Data -> m_Str[idx] = X;
}
//-------------------------------------------------------------------------------------------------
void               CStrAuto::append              ( const char * src,
                                                   int srcLen )
{
  detach ( m_Data -> m_Len + srcLen + 1 );
  m_Data -> Append ( src, srcLen );
}
//-------------------------------------------------------------------------------------------------
void               CStrAuto::detach              ( int newMaxSize )
{
  if ( m_Data -> m_RefCnt > 1 )
  { // data block is shared among more than one instance of CStrAuto. Create own block
    // that can be modified without affecting the other CStrAuto instances.

    TData * tmp = new TData ( *m_Data, newMaxSize );

    // tmp is a separate data block for use in our instance. Disconnect from the source
    m_Data -> m_RefCnt --;
    m_Data = tmp;
  }
}
//-------------------------------------------------------------------------------------------------
CStrAuto            operator +                   ( const CStrAuto & a, const CStrAuto & b )
{
  CStrAuto res ( a );
  res += b;
  return res;
}
//-------------------------------------------------------------------------------------------------
ostream          & operator <<                   ( ostream & os, const CStrAuto & x )
{
  return os << x . m_Data -> m_Str;
}
//=================================================================================================
double             getTimeStamp                  ( void )
{
  struct timeval tv;
  gettimeofday ( &tv, nullptr );
  return tv . tv_sec + tv . tv_usec / 1000000.0;
}
//-------------------------------------------------------------------------------------------------
CStrAuto           randomString                  ( const char * randStr, int len )
{
  int x = (int)(0.5 * rand () * len / RAND_MAX);
  return CStrAuto ( randStr + x );
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
  vector<CStrAuto> vec ( ARRAY_SIZE ) ;

  double timeSt = getTimeStamp ();
  for ( int i = 0; i < ARRAY_SIZE; i ++ )
    vec[i] = randomString ( randSeed, RAND_LEN );

  for ( int i = 0, j = ARRAY_SIZE - 1; i < j; i ++, j -- )
    swap ( vec[i], vec[j] );

  cout << "size: " <<  ARRAY_SIZE / 2 << " swaps: " << ( getTimeStamp () - timeSt ) << " sec" << endl;
  return 0;
}
//-------------------------------------------------------------------------------------------------
