#include <cstring>
#include <cstdio>
#include <iostream>
using namespace std;

/* String implementation, counted references. The strings use shared holder objects (TData).
 * These shared data are copied only when needed - i.e. if the contents is to be modified or
 * if the last CStrAuto releases the string (copy-on-write).
 */

class CStrAuto
{
  public:
                             CStrAuto            ( const char * str = "" );
    explicit                 CStrAuto            ( double x );
    explicit                 CStrAuto            ( int x );
                             CStrAuto            ( const CStrAuto & x );
                            ~CStrAuto            ( void );
    CStrAuto               & operator =          ( const CStrAuto & x );
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
                             TData               ( const char * str );
                             TData               ( int Reserve );
                            ~TData               ( void );
      int                    m_RefCnt;
      int                    m_Len;
      int                    m_Max;
      char                 * m_Str;
    };

    TData                  * m_Data;


    void                     append              ( const char * src, int srclen );
    void                     detach              ( int newMaxSize );

    friend CStrAuto          operator +          ( const CStrAuto & a, const CStrAuto & b );
    friend std::ostream    & operator <<         ( std::ostream & os, const CStrAuto & x );
};

CStrAuto           operator +                    ( const CStrAuto & a, const CStrAuto & b );
std::ostream     & operator <<                   ( std::ostream & os, const CStrAuto & x );
//=================================================================================================
                   CStrAuto::TData::TData        ( const char * str )
                   : m_RefCnt ( 1 ),
                     m_Len ( strlen ( str ) ),
                     m_Max ( m_Len + 1 ),
                     m_Str ( new char[m_Max] )
{
  memcpy    ( m_Str, str, m_Len + 1 );
}
//-------------------------------------------------------------------------------------------------
                   CStrAuto::TData::TData        ( int reserve )
                   : m_RefCnt ( 1 ),
                     m_Len ( 0 ),
                     m_Max ( reserve ),
                     m_Str ( new char[m_Max] )

{
  m_Str[0] = 0;
}
//-------------------------------------------------------------------------------------------------
                   CStrAuto::TData::~TData       ( void )
{
  delete [] m_Str;
}
//=================================================================================================
                   CStrAuto::CStrAuto            ( const char * str )
                   : m_Data ( new TData ( str ) )
{
}
//-------------------------------------------------------------------------------------------------
                   CStrAuto::CStrAuto            ( double x )
                   : m_Data ( new TData ( 20 ) )
{
  operator += ( x );
}
//-------------------------------------------------------------------------------------------------
                   CStrAuto::CStrAuto            ( int x )
                   : m_Data ( new TData ( 20 ) )
{
  operator += ( x );
}
//-------------------------------------------------------------------------------------------------
                   CStrAuto::CStrAuto            ( const CStrAuto & x )
                   : m_Data ( x . m_Data )
{
  m_Data -> m_RefCnt ++;
}
//-------------------------------------------------------------------------------------------------
                   CStrAuto::~CStrAuto           ( void )
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
  char tmp [20];

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
  if ( idx < 0 || idx >= m_Data -> m_Len )
    throw "index out of range";

  return m_Data -> m_Str[idx];
}
//-------------------------------------------------------------------------------------------------
void               CStrAuto::SetCharAt           ( int idx, char x )
{
  if ( idx < 0 || idx >= m_Data -> m_Len )
    throw "index out of range";

  detach ( m_Data -> m_Max );
  m_Data -> m_Str[idx] = x;
}
//-------------------------------------------------------------------------------------------------
void               CStrAuto::append              ( const char * src, int srclen )
{
  detach ( m_Data -> m_Len + srclen + 1 );

  if ( m_Data -> m_Len + srclen + 1 > m_Data -> m_Max )
  {
    m_Data -> m_Max = m_Data -> m_Len + srclen + 1;
    char * tmp = new char [m_Data -> m_Max];
    memcpy ( tmp, m_Data -> m_Str, m_Data -> m_Len );
    memcpy ( tmp + m_Data -> m_Len, src, srclen );
    delete [] m_Data -> m_Str;  // safe even if m_Data == src
    m_Data -> m_Str = tmp;
  }
  else
    memcpy ( m_Data -> m_Str + m_Data -> m_Len, src, srclen );
  m_Data -> m_Len += srclen;
  m_Data -> m_Str [ m_Data -> m_Len ] = 0;
}
//-------------------------------------------------------------------------------------------------
void               CStrAuto::detach              ( int newMaxSize )
{
  if ( m_Data -> m_RefCnt > 1 )
  { // data block is shared among more than one instance of CStrAuto. create own block
    // that can be modified without affecting the other CStrAuto instances.

    TData * tmpD = new TData ( newMaxSize );
    memcpy ( tmpD -> m_Str, m_Data -> m_Str, m_Data -> m_Len + 1 ); // + terminating zero
       // tmpD is a separate data block for use in our instance. disconnect from the source

    m_Data -> m_RefCnt --;
    m_Data = tmpD;
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
std::ostream     & operator <<                   ( std::ostream & os, const CStrAuto & x )
{
  os << x . m_Data -> m_Str;
  return os;
}
//=================================================================================================
int                main                          ( int argc, char * argv [] )
{
  CStrAuto      a ( 5 ), b = "Long test string";
  int       i;

  a += " test ";

  CStrAuto      c ( a );                       // copy constructor

  a += 12.5;                               // op +=
  a += b;

  cout << "a = " << a << endl <<
          "b = " << b << endl <<
          "c = " << c << endl;

  for ( i = a . Length  () - 1; i >= 0; i -- )  // test op []
   cout << a[i];
  cout << endl;

  c = b;

  const char * pokus = (const char*)  c;       // typecast op test

  cout << pokus << endl;

  c = a + b;                                // concatenation

  cout << "a = " << a << endl <<
          "b = " << b << endl <<
          "c = " << c << endl;


  c = a + "pokus";                               // user conversion constructor

  b = a + CStrAuto ( 5 ); // ok
  b = a + CStrAuto ( 5.0 ); // ok

  return 0;
}
//-------------------------------------------------------------------------------------------------
