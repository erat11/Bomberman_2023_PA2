#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#endif /* __PROGTEST__ */

const int READ_L = 1;
const int BYTE_SIZE = 8;

class fibMem
{
	public:
		std::vector<int> v;
		int i;
		fibMem ( )
		{
			v.push_back(1);
			v.push_back(2);
			i = 2;
		}
		void print ()
		{
			for ( int x : v ) std::cout << std::dec << x << " ";
			std::cout << "\n";
		}
};

void fibGenerator ( fibMem & f, int max )
{
	while ( f.i < max )
	{
		f.v.push_back( f.v[f.i-1] + f.v[f.i-2] );
		f.i++;
	}
}

int myPower ( int base, int exp )
{
	if ( exp < 0 ) return 0;
	int r = 1;
	while ( exp != 0 )
	{
		r *= base;
		exp--;
	}
	return r;
}

void reverseVec ( std::vector<bool> & r )
{
	std::vector<bool> v;
	for (int i = r.size() - 1; i >= 0; --i) v.push_back ( r[i] );
	r.clear();
	for ( auto x : v ) r.push_back ( x );
}

std::vector<bool> decToFibBin ( int q, fibMem f )
{
	std::vector<bool> v;
	q++;
	while ( q > f.v[f.i-1] ) fibGenerator( f, f.i + 1 );
	int j = f.i - 1;
	while ( q < f.v[j] ) j--;
	while ( j != -1 )
	{
		v.push_back( q >= f.v[j] );
		if ( q >= f.v[j] ) q -=  f.v[j];
		j--;
	}
	reverseVec ( v );
	v.push_back(true);
	return v;
}

int binToDec ( std::vector<bool> v )
{
	if ( v.empty() ) return 0;
	int sum = 0, e = 128;
	for (int i = 0; i < 8; ++i)
	{
		if ( v[i] ) sum += e;
		e /= 2;
	}
	return sum;
}

bool decToFib ( std::ofstream & out, int sum, fibMem & f, std::vector<bool> & wbyte )
{
	if ( sum > 1114111 ) return false;
	std::vector<bool> ret = decToFibBin ( sum , f );
	for ( bool x : ret )
	{
		wbyte . push_back ( x );
		if ( wbyte.size() == 8 )
		{
			reverseVec( wbyte );
			out.put ( binToDec ( wbyte ) );
			if ( out.fail ( ) )  return false;
			wbyte.clear();
		}
	}
	return true;
}

bool interpretUTF8Byte ( std::ofstream & out, std::vector<bool> & b, int & sum, fibMem & f, char c, std::vector<bool> & wbyte, int  & e, int & nextBytes )
{
	if ( !b[0] )
	{
		if ( nextBytes != 0 ) return false;
		if ( ! decToFib ( out, (int) c, f, wbyte ) ) return false;
		sum = 0;
	}
	else if ( b[0] && !b[1] )
	{
		if ( nextBytes <= 0 ) return false;
		nextBytes--;
		for ( int i = 2; i < 8; ++i ) sum += myPower ( 2, e-- ) * b[i];
		if ( e <= 0 )
		{
			if ( ! decToFib ( out, sum, f, wbyte ) ) return false;
			sum = 0;
			e = 0;
		}
	}
	else if ( b[0] &&  b[1] && !b[2] ) 					 { if ( nextBytes != 0 ) return false; e = 10; for ( int i = 3; i < 8; ++i ) sum += myPower ( 2, e-- ) * b[i]; nextBytes = 1; }
	else if ( b[0] &&  b[1] &&  b[2] && !b[3] ) 		 { if ( nextBytes != 0 ) return false; e = 15; for ( int i = 4; i < 8; ++i ) sum += myPower ( 2, e-- ) * b[i]; nextBytes = 2; }
	else if ( b[0] &&  b[1] &&  b[2] &&  b[3] && !b[4] ) { if ( nextBytes != 0 ) return false; e = 20; for ( int i = 5; i < 8; ++i ) sum += myPower ( 2, e-- ) * b[i]; nextBytes = 3; }
	else return false;
	return true;
}

bool utf8ToFibonacci ( const char * inFile, const char * outFile )
{
	std::ifstream in( inFile, std::ios::binary | std::ios::in );
	std::ofstream out( outFile, std::ios::binary | std::ios::out );
	if ( ! in || ! out ) return false;
	in  . seekg ( 0, std::ios::beg );
	out . seekp ( 0, std::ios::beg );
	fibMem f;
	std::vector<bool> wbyte;
	int sum = 0, e = 0, nB = 0;
	char c;
	while ( in.get ( c ) )
	{
		std::vector<bool> byte;
		for ( int i = 7; i >= 0; --i ) byte.push_back(  ( ( c >> i ) & 1 ) );
		if ( ! interpretUTF8Byte ( out, byte, sum, f, c, wbyte, e, nB ) ) return false;
	}
	if ( nB != 0 ) return false;
	if ( ! wbyte.empty() )
	{
		while ( wbyte.size() % 8 != 0 ) wbyte.push_back(false);
		reverseVec( wbyte );
		out.put ( binToDec ( wbyte ) );
	}
	out.close();
	return true;
}

void interpretFibByte ( std::vector<bool> & b, int & sum, fibMem & f, std::vector<int> & wr, bool & last, int & fibIndex )
{
	for ( auto x : b )
	{
		fibGenerator( f, fibIndex + 1 );
		if ( last && x ) { wr.push_back( --sum ); sum = 0; fibIndex = -1; last = false; }
		else  { sum += x * f.v[fibIndex];last = x; }
		fibIndex ++;
	}
	b.clear();
}

void getSumXToYwithZ ( int & sum, int & c, int x, int y, int z )
{
	for ( int i = x; i >= y; --i )
	{
		int q = myPower ( 2, i );
		if ( c >= q ) { sum += q/z; c -= q; }
	}
}

bool writeUTF8 ( std::ofstream & out, int c )
{
	int sum = 0;
	if ( c > 1114111 ) return false;
	if      ( c <= 127 ) { out.put ( (char) c ); if ( out.fail ( ) )  return false; }
	else if ( c <= 2047 )
	{
		sum = 192;
		getSumXToYwithZ ( sum, c, 10, 6, 64 );
		out.put ( (char) sum );
		if ( out.fail ( ) )  return false;
		sum = 128;
		getSumXToYwithZ ( sum, c, 5, 0, 1 );
		out.put ( (char) sum );
	}
	else if ( c <= 65535 )
	{
		sum = 224;
		getSumXToYwithZ ( sum, c, 15, 12, 4096 );
		out.put ( (char) sum );
		if ( out.fail ( ) )  return false;
		sum = 128;
		getSumXToYwithZ ( sum, c, 11, 6, 64 );
		out.put ( (char) sum );
		sum = 128;
		getSumXToYwithZ ( sum, c, 5, 0, 1 );
		out.put ( (char) sum );
	}
	else if ( c <= 2097151 )
	{
		sum = 240;
		getSumXToYwithZ ( sum, c, 20, 18, 262144 );
		out.put ( (char) sum );
		if ( out.fail ( ) ) return false;
		sum = 128;
		getSumXToYwithZ ( sum, c, 17, 12, 4096 );
		out.put ( (char) sum );
		sum = 128;
		getSumXToYwithZ ( sum, c, 11, 6, 64 );
		out.put ( (char) sum );
		sum = 128;
		getSumXToYwithZ ( sum, c, 5, 0, 1 );
		out.put ( (char) sum );
	}
	else return false;
	return true;
}

bool fibonacciToUtf8 ( const char * inFile, const char * outFile )
{
	std::ifstream in( inFile, std::ios::binary | std::ios::in );
	std::ofstream out( outFile, std::ios::binary | std::ios::out );
	if ( ! in || ! out ) return false;
	out.clear();
	in  . seekg ( 0, std::ios::beg );
	fibMem f;
	int sum = 0, fibIndex = 0;
	std::vector<bool> byte;
	bool last = false;
	char c;
	while ( in.get ( c ) )
	{
		std::vector<int> wr;
		for ( int i = 0; i < 8 ; ++i ) byte.push_back(  ( ( c >> i ) & 1 ) );
		interpretFibByte ( byte, sum, f, wr, last, fibIndex);
		for ( int x : wr ) if ( ! writeUTF8 ( out, x ) || x > 1114111 ) return false;
		wr.clear();
		if ( fibIndex >= 32 ) return false;
	}
	if ( fibIndex != 0 && sum != 0 ) return false;
	out.close();
	return true;
}

#ifndef __PROGTEST__

bool identicalFiles ( const char * file1, const char * file2 )
{
	std::ifstream f1, f2;
	f1 . open ( file1, std::ios_base::in );
	f2 . open ( file2, std::ios_base::in );
	if ( ! f1 . is_open () || ! f2 . is_open () )
		return false;
	std::string s1, s2;
	while ( true )
	{
		getline ( f1, s1 );
		getline ( f2, s2 );
		if ( s1 != s2 ) return false;
		if ( f1.eof() && f2.eof() ) break;
		if ( f1.eof() || f2.eof() ) return false;
	}
	return true;
}

int main ( int argc, char * argv [] )
{
	assert ( utf8ToFibonacci ( "example/src_0.utf8", "output.fib" ) && identicalFiles ( "output.fib", "example/dst_0.fib" ) );
	assert ( utf8ToFibonacci ( "example/src_1.utf8", "output.fib" ) && identicalFiles ( "output.fib", "example/dst_1.fib" ) );
	assert ( utf8ToFibonacci ( "example/src_2.utf8", "output.fib" ) && identicalFiles ( "output.fib", "example/dst_2.fib" ) );
	assert ( utf8ToFibonacci ( "example/src_3.utf8", "output.fib" ) && identicalFiles ( "output.fib", "example/dst_3.fib" ) );
	assert ( utf8ToFibonacci ( "example/src_4.utf8", "output.fib" ) && identicalFiles ( "output.fib", "example/dst_4.fib" ) );
	assert ( ! utf8ToFibonacci ( "example/src_5.utf8", "output.fib" ) );
	assert ( fibonacciToUtf8 ( "example/src_6.fib", "output.utf8" ) && identicalFiles ( "output.utf8", "example/dst_6.utf8" ) );
	assert ( fibonacciToUtf8 ( "example/src_7.fib", "output.utf8" ) && identicalFiles ( "output.utf8", "example/dst_7.utf8" ) );
	assert ( fibonacciToUtf8 ( "example/src_8.fib", "output.utf8" ) && identicalFiles ( "output.utf8", "example/dst_8.utf8" ) );
	assert ( fibonacciToUtf8 ( "example/src_9.fib", "output.utf8" ) && identicalFiles ( "output.utf8", "example/dst_9.utf8" ) );
	assert ( fibonacciToUtf8 ( "example/src_10.fib", "output.utf8" ) && identicalFiles ( "output.utf8", "example/dst_10.utf8" ) );
	assert ( ! fibonacciToUtf8 ( "example/src_11.fib", "output.utf8" ) );
	assert ( ! utf8ToFibonacci ( "example/src_20.utf8", "output.fib" ) );
	assert ( ! utf8ToFibonacci ( "in_5024961.bin", "output.fib" ) );
	assert ( ! fibonacciToUtf8 ( "src_0.fib", "output.fib" ) );
	assert ( ! utf8ToFibonacci ( "in_5027229.bin", "output.fib" ) );

	return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */