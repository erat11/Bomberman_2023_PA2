
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <numeric>
#include <vector>
#include <array>
#include <set>
#include <map>
#include <deque>
#include <queue>
#include <stack>
#include <cassert>
using namespace std;

class A
 {
   public:
            A ( int x ) { m_X = x; cout << x; }
   private:
    int m_X;
 };

class B
 {
   public:
            B ( int x ) : m_A ( new A ( x ) ) { }
   private:
    A * m_A;
 };
int main ( void)
{
B test( 98 );
  return 0;
}