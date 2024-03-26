#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <ctime>
#include <climits>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

// uncomment if your code implements initializer lists
// #define EXTENDED_SYNTAX

class CRange
{
  public:

    CRange(long long lo, long long hi)
    :m_Lo(lo),
     m_Hi(hi)
    {
        if (lo > hi)
        {
            throw std::logic_error("Nespravny vstup");
        }
    }

    bool  contains ( long long num) const // returns true if number is in range
    {
        return num >= m_Lo && num <= m_Hi;
    }
    bool contains ( const CRange & a) const// returns true if range is in range
    {
        return a.m_Lo >= m_Lo && a.m_Hi <= m_Hi;
    }
    bool overlay ( const CRange & a ) const
    {
        return (a.m_Lo <= m_Lo && a.m_Hi >= m_Lo - 1 && a.m_Hi <= m_Hi) || (a.m_Lo >= m_Lo && a.m_Hi >= m_Hi && a.m_Lo <= m_Hi + 1);
    }
    CRange & operator += (const CRange & rhs) {
        if (this->contains(rhs))
            return *this;
        if (this->overlay(rhs)) {
            this->m_Lo = min(this->m_Lo, rhs.m_Lo);
            this->m_Hi = max(this->m_Hi, rhs.m_Hi);
            return *this;
        }
        return *this;
    }

    bool operator == (const CRange & a) const
    {
        return (a.m_Lo == m_Lo) && (m_Hi == a.m_Hi);
    }
    friend bool compare (const CRange & a, const CRange & b );
  private:
    friend class CRangeList;
    friend ostream & operator << (ostream & out , const CRange & a);
    long long m_Lo;
    long long m_Hi;
};

bool compare ( const CRange & a, const CRange & b)
{
    return a.m_Lo < b.m_Lo;
}

class CRangeList
{
  public:
    // constructor
    CRangeList()
    {}
    CRangeList(const CRange & a)
    {
        m_Data.push_back(a);
    }


    // includes long long / range
    bool includes(long long num) const
    {
        return includes(CRange(num,num));
    }
    bool includes (const CRange & a) const
    {
        if (m_Data.empty())
            return false;
        if ( m_Data.size() == 1) //TODO tohle asi pujde odstranit
            return m_Data[0].contains(a);
        auto iter =lower_bound(m_Data.begin(), m_Data.end(),a,compare);
        if ( iter == m_Data.begin())
        {
            return iter->contains(a);
        }
        if (((iter->m_Lo <= a.m_Lo && iter->m_Hi >= a.m_Hi) || ((iter - 1)->m_Lo <= a.m_Lo && (iter - 1)->m_Hi >= a.m_Hi)))
            return true;
        return false;
    }

    CRangeList & operator += (const CRange & a)
            //budeme prochazet pole intervalu, kdyz nejaky interval bude obsahovat novy, tak nic nemenime
            //pokud se budou prekryvat tak upravime puvodni interval
            // pokud se nebudou prekryvat, tak ho vlozime na vhodne misto
            //prochazime do te doby nez minimum vkladaneho prvku je vetsi nez maximum porovnavaneho prvku
    {
        for ( size_t i = 0 ; i< m_Data.size() ; i++)
        {
            if (m_Data.size() == 0) {
                m_Data.push_back(a);
                return *this;
            }
            if (a.contains(m_Data[i]))
            {
                m_Data[i].m_Lo = a.m_Lo;
                m_Data[i].m_Hi = a.m_Hi;
                fixBadRange(i);
                return *this;
            }
            if (m_Data[i].contains(a)) // pokud pridavany interval je jiz obsazen
                return *this;
            if (m_Data[i].overlay(a)) // pokud se nejak prekryvaji
            {
                m_Data[i].m_Lo = min(a.m_Lo,m_Data[i].m_Lo);
                m_Data[i].m_Hi = max (a.m_Hi, m_Data[i].m_Hi);
                fixBadRange(i);
                return *this;
            }
            if (a.m_Hi < m_Data[i].m_Lo)
            {
                m_Data.insert(m_Data.begin() + i, a);
                return *this;
            }
        }
        m_Data.push_back(a);
        return *this;
    }
    CRangeList operator- ( const CRange & a)
    {
        CRangeList tmp(*this);
        tmp -= a;
        return tmp;
    }
    CRangeList & operator += ( const CRangeList & a)
    {
        for( size_t i = 0; i < a.m_Data.size(); i++) {
            *this += (a.m_Data[i]);
        }
        return *this;
    }
    CRangeList & operator-= ( const CRange & a)
    {
        for ( size_t i = 0 ; i < m_Data.size(); i++)
        {
            if ( m_Data.size() == 0)
                return *this;
            if ( a.contains(m_Data[i])) { // pokud a pokryva nejaky cely interval
                m_Data.erase(m_Data.begin() + i);
                i--; // zmeni se mi size m_Data ale ja potrebuju projit vsechny prvky
                continue;
            }
            if (m_Data[i].contains(a)) // pokud je a obsazeno v nejakem intervalu, tak odstranime puvodni interval a vlozime 2 mensi
            {
                substractRangeContaining(i,a);
                return *this;
            }
            if (m_Data[i].overlay(a))
            {
                subtractRangeOverlay(m_Data[i],a);
            }
            if ( a.m_Hi < m_Data[i].m_Lo) // kdyz uz neni co ubirat
                return *this;
        }
        return *this;
    }
    CRangeList & operator -= (const CRangeList & a)
    {
        for (size_t i = 0 ; i < a.m_Data.size() ; i++)
        {
            if (m_Data.size() == 0 )
                return *this;
            *this -= a.m_Data[i];
        }
        return *this;
    }
    // -= range / range list
    CRangeList & operator= (CRange a)
    {
        m_Data.push_back(a);
        return *this;
    }
    bool operator==(const CRangeList & a) const
    {
        if(m_Data.size() != a.m_Data.size())
            return false;
        for ( size_t i = 0 ; i < m_Data.size() ; i++)
            if (!(m_Data[i] == a.m_Data[i]))
                return false;
        return true;
    }
    bool operator != (const CRangeList & a ) const
    {
        return !(*this == a);
    }
    CRangeList operator+ (const CRange & a)
    {
        CRangeList tmp(*this);
        tmp += a;
        return tmp;
    }
    friend ostream & operator<< (ostream & out, const CRangeList & a);

    void printList () const
    {
        for (size_t i = 0; i< m_Data.size() ; i++)
        {
            cout << "<" << m_Data[i].m_Lo << ";" <<m_Data[i].m_Hi << ">" <<" || ";
        }
    }
  private:
    void substractRangeContaining(int i, const CRange & a)
    {
        if ( m_Data[i].m_Lo == a.m_Lo)
        {
            m_Data[i].m_Lo = a.m_Hi + 1;
            return;
        }
        if ( m_Data[i].m_Hi == a.m_Hi)
        {
            m_Data[i].m_Hi = a.m_Lo -1 ;
            return;
        }
        CRange tmp1(m_Data[i].m_Lo,a.m_Lo - 1);
        CRange tmp2(a.m_Hi + 1,m_Data[i].m_Hi);
        m_Data.erase(m_Data.begin() + i);
        m_Data.insert(m_Data.begin() + i, tmp1);
        m_Data.insert(m_Data.begin() + i + 1, tmp2);
    }
    void subtractRangeOverlay(CRange & a, const CRange & b )
    //metoda pocita s tim, ze se a a b poze prekryvaji, nerovnaji se, nebo jedna neobsahuje druhou
    {
        if (b.m_Hi >= a.m_Hi)
        {
            a.m_Hi = b.m_Lo - 1;
            return;
        }
        if(b.m_Hi >= a.m_Lo) //prekryvaji se zleva
        {
            a.m_Lo = b.m_Hi + 1;
            return;
        }
    }
    void fixBadRange (int index)
    {
        //zaridi aby od indexu nebyly neplatne intervaly
        if (m_Data[index].m_Hi < m_Data[index + 1].m_Lo - 1  || static_cast<size_t>(index) == m_Data.size() - 1)
            return;
        m_Data[index].m_Hi = max (m_Data[index + 1].m_Hi,m_Data[index].m_Hi);
        m_Data.erase(m_Data.begin() + index + 1);
        fixBadRange(index++);
    }
    vector<CRange> m_Data;
};

CRangeList operator+ ( const CRange & a, const CRange & b)
{
    CRangeList tmp(a);
    tmp += b;
    return tmp;
}
CRangeList operator- (const CRange & a, const CRange & b)
{
    CRangeList tmp(a);
    tmp -= b;
    return tmp;
}

ostream & operator << (ostream & out , const CRange & a)
{
    if (a.m_Lo == a.m_Hi) {
        out << a.m_Lo;
        return out;
    }
    out << "<" << a.m_Lo << ".." << a.m_Hi << ">";
    return out;
}
ostream & operator<< (ostream & out , const CRangeList & a)
{
    size_t i = 0;
    if (a.m_Data.size() == 0)
    {
        out << "{}";
        return out;
    }
    out << "{";
    for ( ; i < a.m_Data.size() - 1 ; i ++ )
    {
        out << a.m_Data[i] << ",";
    }
    out << a.m_Data[i];
    out << "}";
    return out;
}

long long min ( long long lhs, long long rhs)
{
    if (lhs <= rhs)
        return lhs;
    return rhs;
}
long long max ( long long lhs, long long rhs)
{
    if (lhs >= rhs)
        return lhs;
    return rhs;
}


#ifndef __PROGTEST__
string             toString                                ( const CRangeList& x )
{
  ostringstream oss;
  oss << x;
  return oss . str ();
}

int                main                                    ( void )
{
  CRangeList a, b;
//a = CRange(5,10);
//a.printList();
//cout << endl;
//a += CRange(9,12);
//a += CRange(11,12);
//a += CRange(2,3) += CRange(1,2) += CRange(-1, -1);
//a.printList();
  assert ( sizeof ( CRange ) <= 2 * sizeof ( long long ) );
  a = CRange ( 5, 10 );
    a += CRange ( 25, 100 );
  assert ( toString ( a ) == "{<5..10>,<25..100>}" );
  a += CRange ( -5, 0 );
  a += CRange ( 8, 50 );
  //a.printList();
  assert ( toString ( a ) == "{<-5..0>,<5..100>}" );
  a += CRange ( 101, 105 ) + CRange ( 120, 150 ) + CRange ( 160, 180 ) + CRange ( 190, 210 );
  //a.printList();
  assert ( toString ( a ) == "{<-5..0>,<5..105>,<120..150>,<160..180>,<190..210>}" );
  a += CRange ( 106, 119 ) + CRange ( 152, 158 );
  assert ( toString ( a ) == "{<-5..0>,<5..150>,<152..158>,<160..180>,<190..210>}" );
  a += CRange ( -3, 170 );
  a += CRange ( -30, 1000 );
  assert ( toString ( a ) == "{<-30..1000>}" );
  b = CRange ( -500, -300 ) + CRange ( 2000, 3000 ) + CRange ( 700, 1001 );
  a += b;
  assert ( toString ( a ) == "{<-500..-300>,<-30..1001>,<2000..3000>}" );
  a -= CRange ( -400, -400 );
  assert ( toString ( a ) == "{<-500..-401>,<-399..-300>,<-30..1001>,<2000..3000>}" );
  a -= CRange ( 10, 20 ) + CRange ( 900, 2500 ) + CRange ( 30, 40 ) + CRange ( 10000, 20000 );
  assert ( toString ( a ) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}" );
  try
  {
    a += CRange ( 15, 18 ) + CRange ( 10, 0 ) + CRange ( 35, 38 );
    assert ( "Exception not thrown" == nullptr );
  }
  catch ( const std::logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown" == nullptr );
  }
  assert ( toString ( a ) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}" );
  b = a;
  assert ( a == b );
  assert ( !( a != b ) );
  b += CRange ( 2600, 2700 );
  assert ( toString ( b ) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}" );
  assert ( a == b );
  assert ( !( a != b ) );
  b += CRange ( 15, 15 );
  assert ( toString ( b ) == "{<-500..-401>,<-399..-300>,<-30..9>,15,<21..29>,<41..899>,<2501..3000>}" );
  assert ( !( a == b ) );
  assert ( a != b );
  assert ( b . includes ( 15 ) );
  assert ( b . includes ( 2900 ) );
  assert ( b . includes ( CRange ( 15, 15 ) ) );
  assert ( b . includes ( CRange ( -350, -350 ) ) );
  assert ( b . includes ( CRange ( 100, 200 ) ) );
  assert ( !b . includes ( CRange ( 800, 900 ) ) );
  assert ( !b . includes ( CRange ( -1000, -450 ) ) );
  assert ( !b . includes ( CRange ( 0, 500 ) ) );
  a += CRange ( -10000, 10000 ) + CRange ( 10000000, 1000000000 );
  assert ( toString ( a ) == "{<-10000..10000>,<10000000..1000000000>}" );
  b += a;
  assert ( toString ( b ) == "{<-10000..10000>,<10000000..1000000000>}" );
  b -= a;
  assert ( toString ( b ) == "{}" );
  b += CRange ( 0, 100 ) + CRange ( 200, 300 ) - CRange ( 150, 250 ) + CRange ( 160, 180 ) - CRange ( 170, 170 );
  assert ( toString ( b ) == "{<0..100>,<160..169>,<171..180>,<251..300>}" );
  b -= CRange ( 10, 90 ) - CRange ( 20, 30 ) - CRange ( 40, 50 ) - CRange ( 60, 90 ) + CRange ( 70, 80 );
  assert ( toString ( b ) == "{<0..9>,<20..30>,<40..50>,<60..69>,<81..100>,<160..169>,<171..180>,<251..300>}" );
  CRangeList c;
  c += CRange (100, 150) += CRange (151,151);
  c -= CRange(0,200);
  c+= CRange ( 39430, 2035345) + CRange(100, 200) + CRange(-40,0) + CRange(-1111111, -41);
  c -= CRange (LLONG_MIN, LLONG_MAX);
  c += CRange ( LLONG_MIN, LLONG_MAX);
  CRangeList d( CRange ( 5,10));
  d+=CRange(15,20);

  CRangeList e(CRange(LLONG_MIN, 0));
  e-=CRange (LLONG_MIN,-5); // funguje
  e+=CRange ( LLONG_MIN, LLONG_MAX); // oka
  e += CRange(100 , LLONG_MAX); // dobre
  e -= CRange(LLONG_MAX,LLONG_MAX);
  e -= CRange(LLONG_MAX-5,LLONG_MAX) + CRange(LLONG_MAX,LLONG_MAX);
  e += CRange(LLONG_MAX , LLONG_MAX);
  e -= CRange(LLONG_MIN, LLONG_MAX);
  e.printList();

//#ifdef EXTENDED_SYNTAX
  //CRangeList x { { 5, 20 }, { 150, 200 }, { -9, 12 }, { 48, 93 } };
//  assert ( toString ( x ) == "{<-9..20>,<48..93>,<150..200>}" );
//  ostringstream oss;
//  oss << setfill ( '=' ) << hex << left;
//  for ( const auto & v : x + CRange ( -100, -100 ) )
//    oss << v << endl;
//  oss << setw ( 10 ) << 1024;
//  assert ( oss . str () == "-100\n<-9..20>\n<48..93>\n<150..200>\n400=======" );
//#endif /* EXTENDED_SYNTAX */
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
