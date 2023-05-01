#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;
#endif /* __PROGTEST__ */

  class str
{
  public:
    int len;
    size_t size;
    char * data;
    str () { size = 0; }
    str ( const char * Str ) :len ( strlen ( Str ) ), size ( len + 1 ), data ( new char[size] ) { memcpy ( data, Str, size ); }
    str ( const str & x ) : len ( x.len ), size ( len + 1 ), data ( new char[size] ) { memcpy ( data, x.data, size ); }
    str & operator = ( const str & x )
    {
      if ( &x != this )
      {
        if ( size != 0 )
          delete [] data;
        size   = x . size;
        len   = x . len;
        data  = new char [size];
        memcpy  ( data, x . data, len + 1 );
        size = x.size;
      }
      return *this;
    }
    bool operator == ( const str & x ) const
    {
      if ( len != x.len ) return false;
      for ( int i = 0; i < len; ++i )
        if ( data[i] != x.data[i] )
          return false;
      return true;
    }
    bool operator != ( const str & x ) const{ return !(x == *this); }
    bool operator > ( const str & x ) const
    {
      for ( int i = 0; i < len; ++i )
        if ( data[i] < x.data[i] ) return false;
        else if ( data[i] > x.data[i] ) return true;
      return false;
    }
    bool operator < ( const str & x ) const
    {
      for ( int i = 0; i < len; ++i )
        if ( data[i] > x.data[i] ) return false;
        else if ( data[i] < x.data[i] ) return true;
      return false;
    }
    ~str () { if (size != 0 ) delete[] data; }
    void destroy() { delete [] data; size = 0; }
    void print () const 
    {
      for (int i = 0; i < len; ++i)
        cout << data[i];
      cout << endl;
    }
    char * toString () const { return data; }
};


template <typename T>
class Vector 
{
  public:
      Vector(Vector&& other)
      {
        data = other.data;
        size = other.size;
        capacity = other.capacity;
        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;
      }
      Vector& operator=(Vector&& other)
      {
        if (this != &other)
        {
          delete[] data;
          data = other.data;
          size = other.size;
          capacity = other.capacity;
          other.data = nullptr;
          other.size = 0;
          other.capacity = 0;
        }
        return *this;
      }
      Vector() 
      {
        capacity = 16;
        size = 0;
        data = new T[capacity];
      }
      Vector( const T & value ) 
      {
        capacity = 16;
        size = 1;
        data = new T[capacity];
        data[0] = value;
      }
      Vector ( const  Vector<T> & v )
      {
        capacity = v.getCapacity();
        size = v.getSize();
        data = new T [ capacity ];
        for ( int i = 0; i < size; i++ ) data[i] = v.getData()[i];      
      } 
      Vector & operator = ( const Vector<T> & v )
      {
        destroy();
        capacity = v.getCapacity() + 1;
        size = v.getSize();
        data = new T [ capacity + 1];
        for ( int i = 0; i < size; i++ ) data[i] = v.getData()[i];
        return *this;
      }
      ~Vector() 
      { 
        if ( capacity != 0 ) 
        { 
          for (int i = 0; i < size; ++i) data[i].destroy(); 
          destroy(); 
        }  
      }
      bool operator == ( const Vector<T> & v ) const
      {
        for ( int i = 0; i < size; i++ ) if ( data[i] != v.getData()[i] ) return false;
        return true;
      }
      bool operator != ( const Vector<T> & v ) const
      {
        for ( int i = 0; i < size; i++ ) if ( data[i] == v.getData()[i] ) return false;
        return true;
      }
      bool operator < ( const Vector<T> & v ) const
      {
        for (int i = 0; i < size; ++i)
          if ( data[i] >= v.getData()[i] ) return false;
          else if ( data[i] < v.getData()[i] ) return true;
        return false;
      }
      bool operator > ( const Vector<T> & v ) const
      {
        for (int i = 0; i < size; ++i)
          if ( data[i] <= v.getData()[i] ) return false;
          else if ( data[i] > v.getData()[i] ) return true;
        return false;
      }
      T & operator[]( int index ) { return data[index]; } //index must be >= 0, no safeguards
      T & operator[]( int index ) const { return data[index]; } //index must be >= 0, no safeguards
      void push_back( const T & value ) 
      {
          if (size >= capacity) resize();
          data[size] = value;
          size++;
      }
      int getSize() const { return size; }
      int getCapacity() const { return capacity; }
      T * getData() const { return data; }
      void destroy() { delete [] data; capacity = 0; }
      void print() const { for (int i = 0; i < size; ++i) cout << data[i]; cout << endl; }
  private:
      int capacity;
      int size;
      T* data;
      void resize() 
      {
          capacity *= 2;
          T * newData = new T[capacity];
          for ( int i = 0; i < size; i++ ) newData[i] = data[i];
          delete[] data;
          data = newData;
      }
};

class CMail
{
  public:
    CMail () { }
    CMail ( const char * from, const char * to, const char * body ) : f ( from ), t ( to ), b ( body ) {}
    bool operator == ( const CMail & x ) const { return f == x.f && t == x.t && b == x.b; }
    CMail & operator = ( const CMail & x )
    {
      f = x.f;
      t = x.t;
      b = x.b;
      return *this;
    }
    ~CMail () {}
    str Sender() const { return f; }
    str Recipient() const { return t; }
    str Body() const { return b; }
    void destroy (){ f.destroy(); t.destroy(); b.destroy(); }
    void print() const { f.print(); t.print(); b.print(); cout << endl; }
    friend ostream & operator << ( ostream & os, const CMail & m );
  protected:
    str f, t, b;
};

ostream & operator << ( ostream & os, const CMail & m )
{
  os << "From: " << m.Sender().toString() << ", To: " << m.Recipient().toString() <<
  ", Body: " <<  m.Body().toString();
  return os;
}

class Person
{
  public:
    Person () {}
    Person ( const str & e, const Vector<CMail> & o, const Vector<CMail> & i )
    {
      email = e;
      out = o;
      in = i;
    }
    ~Person () { }
    Person & operator = ( const Person & p )
    {
      email = p.email;
      out = p.out;
      in = p.in;
      return *this;
    }
    bool operator == ( const Person & p )const { return p.email == email; }
    bool operator != ( const Person & p )const{ return p.email != email; }
    bool operator > ( const Person & p )const{ return p.email > email; }
    bool operator < ( const Person & p )const{ return p.email < email; }
    str email;
    Vector<CMail> out, in;
    void destroy () { out.destroy(); in.destroy(); email.destroy();}
    void print() const { email.print(); for (int i = 0; i < in.getSize(); ++i) in[0].print(); }
};

class CMailIterator 
{
  public:
    CMailIterator ( const Vector<CMail> & mails, int idx )
    {
      this->mails = mails;
      this->idx = idx;
    }
    explicit operator bool ( void ) const{ return ( idx >= 0 && idx < mails.getSize() ); } 
    bool operator ! ( void ) const{ return ( idx < 0 || idx >= mails.getSize() ); }
    const CMail & operator * ( void ) const{ return mails[idx]; }
    CMailIterator & operator ++ ( void )
    {
      ++idx;
      return *this;
    }
  private:
    Vector<CMail> mails;
    int idx;
};


class CMailServer 
{
  public:
    CMailServer ( void ) { }
    CMailServer ( const CMailServer & src ){ people = src.get(); }
    CMailServer & operator = ( const CMailServer & src )
    {
      people = src.get();
      return *this;
    }
    ~CMailServer ( void ) {  }
    void sendMail ( const CMail & m )
    {
      int q = binSearch ( people,  m.Sender() );
      if ( q == -1 ) insert ( people, m.Sender(), 1, m ); //out = 1, in = 0
      else people[q].out.push_back( m );
      q = binSearch ( people,  m.Recipient() );
      if ( q == -1 ) insert ( people, m.Recipient(), 0, m ); //out = 1, in = 0
      else people[q].in.push_back( m );
    }
    CMailIterator outbox ( const char * email ) const
    {
      int q = binSearch ( people,  email );
      if ( q == -1 ) return CMailIterator( Vector<CMail>(), -1 );
      return CMailIterator ( people[q].out, 0 );
    }
    CMailIterator inbox ( const char * email ) const
    {
      int q = binSearch ( people,  email );
      if ( q == -1 ) return CMailIterator( Vector<CMail>(), -1 );
      return CMailIterator ( people[q].in, 0 );
    }
    Vector<Person> get() const { return people; }
  private:
    Vector<Person> people;
    //in case of slowness, there is always binsearch
    int binSearch ( const Vector<Person> & v, const str & p ) const
    {
      size_t h = v.getSize(), l = 0;
      while ( h > l )
      {
        size_t mid = l + ( h - l ) / 2;
        if ( v[mid].email == p ) return mid;
        if ( p > v[mid].email ) l = mid + 1;
        else h = mid;
      }
      return -1;
    }
    void insert ( Vector<Person> & p, const str n, bool out, const CMail & m )
    {
      Person newP;
      if (!out) newP = Person( n, Vector<CMail>(), Vector<CMail>(m) ) ; 
      else newP = Person ( n, Vector<CMail>(m), Vector<CMail>() ); 
      int i, j;
      for (i = 0; i < p.getSize() && p[i].email < n; i++);
      if ( p.getSize() == i )
      {
        p.push_back( newP ); 
        return;
      }

      Person tmp = p[i];
      p[i] = newP;
      j = i+1;
      while ( j < p.getSize() )
      {
        Person tmp2 = p[j];
        p[j] = tmp;
        tmp = tmp2;
        j++;
      }
      p.push_back(tmp);
    }
};

#ifndef __PROGTEST__
bool                         matchOutput                   ( const CMail     & m,
                                                             const char      * str )
{
  ostringstream oss;
  oss << m;
  return oss . str () == str;
}

int main ( void )
{
  char from[100], to[100], body[1024];

  assert ( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "john", "peter", "progtest deadline" ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "john", "progtest deadline", "peter" ) ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "peter", "john", "progtest deadline" ) ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "peter", "progtest deadline", "john" ) ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "progtest deadline", "john", "peter" ) ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "progtest deadline", "peter", "john" ) ) );
  CMailServer s0;
  s0 . sendMail ( CMail ( "john", "peter", "some important mail" ) );
  strncpy ( from, "john", sizeof ( from ) );
  strncpy ( to, "thomas", sizeof ( to ) );
  strncpy ( body, "another important mail", sizeof ( body ) );
  s0 . sendMail ( CMail ( from, to, body ) );
  strncpy ( from, "john", sizeof ( from ) );
  strncpy ( to, "alice", sizeof ( to ) );
  strncpy ( body, "deadline notice", sizeof ( body ) );
  s0 . sendMail ( CMail ( from, to, body ) );
  s0 . sendMail ( CMail ( "alice", "john", "deadline confirmation" ) );
  s0 . sendMail ( CMail ( "peter", "alice", "PR bullshit" ) );
  CMailIterator i0 = s0 . inbox ( "alice" );
  assert ( i0 && *i0 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( matchOutput ( *i0,  "From: john, To: alice, Body: deadline notice" ) );
  assert ( ++i0 && *i0 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( matchOutput ( *i0,  "From: peter, To: alice, Body: PR bullshit" ) );
  assert ( ! ++i0 );

  CMailIterator i1 = s0 . inbox ( "john" );
  assert ( i1 && *i1 == CMail ( "alice", "john", "deadline confirmation" ) );
  assert ( matchOutput ( *i1,  "From: alice, To: john, Body: deadline confirmation" ) );
  assert ( ! ++i1 );

  CMailIterator i2 = s0 . outbox ( "john" );
  assert ( i2 && *i2 == CMail ( "john", "peter", "some important mail" ) );
  assert ( matchOutput ( *i2,  "From: john, To: peter, Body: some important mail" ) );
  assert ( ++i2 && *i2 == CMail ( "john", "thomas", "another important mail" ) );
  assert ( matchOutput ( *i2,  "From: john, To: thomas, Body: another important mail" ) );
  assert ( ++i2 && *i2 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( matchOutput ( *i2,  "From: john, To: alice, Body: deadline notice" ) );
  assert ( ! ++i2 );

  CMailIterator i3 = s0 . outbox ( "thomas" );
  assert ( ! i3 );

  CMailIterator i4 = s0 . outbox ( "steve" );
  assert ( ! i4 );

  CMailIterator i5 = s0 . outbox ( "thomas" );
  s0 . sendMail ( CMail ( "thomas", "boss", "daily report" ) );
  assert ( ! i5 );

  CMailIterator i6 = s0 . outbox ( "thomas" );
  assert ( i6 && *i6 == CMail ( "thomas", "boss", "daily report" ) );
  assert ( matchOutput ( *i6,  "From: thomas, To: boss, Body: daily report" ) );
  assert ( ! ++i6 );

  CMailIterator i7 = s0 . inbox ( "alice" );
  s0 . sendMail ( CMail ( "thomas", "alice", "meeting details" ) );
  assert ( i7 && *i7 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( matchOutput ( *i7,  "From: john, To: alice, Body: deadline notice" ) );
  assert ( ++i7 && *i7 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( matchOutput ( *i7,  "From: peter, To: alice, Body: PR bullshit" ) );
  assert ( ! ++i7 );

  CMailIterator i8 = s0 . inbox ( "alice" );
  assert ( i8 && *i8 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( matchOutput ( *i8,  "From: john, To: alice, Body: deadline notice" ) );
  assert ( ++i8 && *i8 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( matchOutput ( *i8,  "From: peter, To: alice, Body: PR bullshit" ) );
  assert ( ++i8 && *i8 == CMail ( "thomas", "alice", "meeting details" ) );
  assert ( matchOutput ( *i8,  "From: thomas, To: alice, Body: meeting details" ) );
  assert ( ! ++i8 );

  CMailServer s1 ( s0 );
  s0 . sendMail ( CMail ( "joe", "alice", "delivery details" ) );
  s1 . sendMail ( CMail ( "sam", "alice", "order confirmation" ) );
  CMailIterator i9 = s0 . inbox ( "alice" );
  assert ( i9 && *i9 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( matchOutput ( *i9,  "From: john, To: alice, Body: deadline notice" ) );
  assert ( ++i9 && *i9 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( matchOutput ( *i9,  "From: peter, To: alice, Body: PR bullshit" ) );
  assert ( ++i9 && *i9 == CMail ( "thomas", "alice", "meeting details" ) );
  assert ( matchOutput ( *i9,  "From: thomas, To: alice, Body: meeting details" ) );
  assert ( ++i9 && *i9 == CMail ( "joe", "alice", "delivery details" ) );
  assert ( matchOutput ( *i9,  "From: joe, To: alice, Body: delivery details" ) );
  assert ( ! ++i9 );

  CMailIterator i10 = s1 . inbox ( "alice" );
  assert ( i10 && *i10 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( matchOutput ( *i10,  "From: john, To: alice, Body: deadline notice" ) );
  assert ( ++i10 && *i10 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( matchOutput ( *i10,  "From: peter, To: alice, Body: PR bullshit" ) );
  assert ( ++i10 && *i10 == CMail ( "thomas", "alice", "meeting details" ) );
  assert ( matchOutput ( *i10,  "From: thomas, To: alice, Body: meeting details" ) );
  assert ( ++i10 && *i10 == CMail ( "sam", "alice", "order confirmation" ) );
  assert ( matchOutput ( *i10,  "From: sam, To: alice, Body: order confirmation" ) );
  assert ( ! ++i10 );

  CMailServer s2;
  s2 . sendMail ( CMail ( "alice", "alice", "mailbox test" ) );
  CMailIterator i11 = s2 . inbox ( "alice" );
  assert ( i11 && *i11 == CMail ( "alice", "alice", "mailbox test" ) );
  assert ( matchOutput ( *i11,  "From: alice, To: alice, Body: mailbox test" ) );
  assert ( ! ++i11 );

  s2 = s0;
  s0 . sendMail ( CMail ( "steve", "alice", "newsletter" ) );
  s2 . sendMail ( CMail ( "paul", "alice", "invalid invoice" ) );
  CMailIterator i12 = s0 . inbox ( "alice" );
  assert ( i12 && *i12 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( matchOutput ( *i12,  "From: john, To: alice, Body: deadline notice" ) );
  assert ( ++i12 && *i12 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( matchOutput ( *i12,  "From: peter, To: alice, Body: PR bullshit" ) );
  assert ( ++i12 && *i12 == CMail ( "thomas", "alice", "meeting details" ) );
  assert ( matchOutput ( *i12,  "From: thomas, To: alice, Body: meeting details" ) );
  assert ( ++i12 && *i12 == CMail ( "joe", "alice", "delivery details" ) );
  assert ( matchOutput ( *i12,  "From: joe, To: alice, Body: delivery details" ) );
  assert ( ++i12 && *i12 == CMail ( "steve", "alice", "newsletter" ) );
  assert ( matchOutput ( *i12,  "From: steve, To: alice, Body: newsletter" ) );
  assert ( ! ++i12 );

  CMailIterator i13 = s2 . inbox ( "alice" );
  assert ( i13 && *i13 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( matchOutput ( *i13,  "From: john, To: alice, Body: deadline notice" ) );
  assert ( ++i13 && *i13 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( matchOutput ( *i13,  "From: peter, To: alice, Body: PR bullshit" ) );
  assert ( ++i13 && *i13 == CMail ( "thomas", "alice", "meeting details" ) );
  assert ( matchOutput ( *i13,  "From: thomas, To: alice, Body: meeting details" ) );
  assert ( ++i13 && *i13 == CMail ( "joe", "alice", "delivery details" ) );
  assert ( matchOutput ( *i13,  "From: joe, To: alice, Body: delivery details" ) );
  assert ( ++i13 && *i13 == CMail ( "paul", "alice", "invalid invoice" ) );
  assert ( matchOutput ( *i13,  "From: paul, To: alice, Body: invalid invoice" ) );
  assert ( ! ++i13 );

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
