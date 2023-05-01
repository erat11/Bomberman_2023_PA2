#include <iostream>
#include <cstdlib>
using namespace std;

class CInt
 {
   public:
          CInt ( int x ) : m_X ( new int ( x ) ) {}
          CInt ( const CInt & src ) { m_X = new int ( *src . m_X ); }
         ~CInt ( void ) { delete m_X; }
    void  operator = ( int x ) { *m_X = x; }
    void  print ( void ) const { cout << *m_X; }
   private:
    int * m_X;
 };
int main ( void )
 {
   CInt a ( 93 ), b ( 21 ), c = a;
   a = b;
   c . print ();
   return 0;
 }

