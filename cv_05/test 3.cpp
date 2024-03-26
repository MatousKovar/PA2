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
    int getSeconds()
    {
        int totalSeconds = 0;
        totalSeconds += m_Hour * 60 * 60;
        totalSeconds += m_Minute *60;
        totalSeconds += m_Second;
        return totalSeconds;
    }
    //idealne by bylo lepsi si to prevest do sekund a s tim pocitat
    void secondsCarryPositive(int seconds)
    {
        int totalSeconds = getSeconds() + seconds;

        m_Hour = totalSeconds / 3600;
        m_Minute = (totalSeconds % 3600) / 60;
        m_Second = ( (totalSeconds % 3600) % 60 );

   }
    void secondsCarryNegative(int seconds)
    {
        int CTimeSeconds = getSeconds();
        int totalSeconds = abs( CTimeSeconds - seconds) ;
        if (CTimeSeconds < seconds)
        {
            m_Hour = 23 - totalSeconds / 3600;
            m_Minute = 59 - (totalSeconds % 3600) / 60;
            m_Second = 59 - ( (totalSeconds % 3600) % 60 ) ;
        }
        else {
            m_Hour = totalSeconds / 3600;
            m_Minute = (totalSeconds % 3600) / 60;
            m_Second = ((totalSeconds % 3600) % 60);
        }
    }
public:
    // constructor, destructor
    CTime()
    {}
    CTime(int hour, int minute, int second = 0)
        : m_Hour(hour),
          m_Minute(minute),
          m_Second(second)
    {
        if (hour < 0 || hour > 23 || minute >59 || minute < 0 || second < 0 || second >59)
            throw std::invalid_argument("Nespravny vstup\n");
    }
    ~CTime()
    {}
    // arithmetic operators
    CTime operator+ (int seconds)
    {
        return CTime(*this) += seconds;
    }
    CTime & operator+= (int seconds)
    {
        secondsCarryPositive(seconds);
        return *this;
    }
    CTime & operator-= ( int seconds)
    {
        secondsCarryNegative(seconds);
        return *this;
    }
    int operator- ( const CTime & obj)
    {
        int res = 0;
        res += abs(m_Hour - obj.m_Hour) * 60 * 60;
        res += abs(m_Minute - obj.m_Minute) * 60;
        res += abs(m_Second - obj.m_Second);
        return res;
    }
    CTime operator- (int seconds)
    {
        return CTime(*this) -= seconds;
    }
    // comparison operators

    bool operator< (const CTime & obj) const
    {
        if (obj.m_Hour == m_Hour)
        {
            if (m_Minute == obj.m_Minute)
                return m_Minute < obj.m_Minute;
            else return m_Second < obj.m_Second;
        }
        return m_Hour < obj.m_Hour;
    }
    bool operator <= (const CTime & obj) const
    {
        if (obj.m_Hour == m_Hour)
        {
            if (m_Minute == obj.m_Minute)
                return m_Minute < obj.m_Minute;
            else return m_Second <= obj.m_Second;
        }
        return m_Hour < obj.m_Hour;
    }
    bool operator !=  (const CTime & obj) const
    {
        return ! (* this == obj);
    }
    bool operator> (const CTime & obj) const
    {
        if (obj.m_Hour == m_Hour)
        {
            if (m_Minute == obj.m_Minute)
                return m_Minute > obj.m_Minute;
            else return m_Second > obj.m_Second;
        }
        return m_Hour > obj.m_Hour;
    }
    bool operator>= (const CTime & obj) const
    {
        if (obj.m_Hour == m_Hour)
        {
            if (m_Minute == obj.m_Minute)
                return m_Minute > obj.m_Minute;
            else return m_Second >= obj.m_Second;
        }
        return m_Hour > obj.m_Hour;
    }
    bool operator== (const CTime & obj) const
    {
        return ((m_Hour == obj.m_Hour) && (m_Minute == obj.m_Minute) && (m_Second == obj.m_Second));
    }
    //kdyz je int v parametru tak to funguje a++, kdyz ne, tak ++a
    CTime & operator++ (int)
    {
        CTime tmp = *this;
        ++(*this);
        return tmp;

        //todo nejdriv vytvorit kopii
//        if ((m_Second + 1) >= 60)
//            secondsCarryPositive(1);
//        else m_Second ++;
//        return *this;
    }
    CTime & operator++ ()
    {
        //printTime();
        if ((m_Second + 1) >= 60)
            secondsCarryPositive(1);
        else m_Second ++;
        return *this;
    }
    CTime & operator-- (int)
    {
        CTime & tmp = *this;
        --(*this);
        return tmp;
//        if ((m_Second - 1) < 0)
//            secondsCarryNegative(1);
//        else m_Second --;
//        printTime();
//        return *this;
    }
    CTime & operator-- ()
    {
        if ((m_Second - 1) < 0)
            secondsCarryNegative(1);
        else m_Second --;
        //printTime();
        return *this;
    }

     // output operator

     //proc nejde pouzit ostringstream??
     //proc funguje jen jako friend?
     friend ostream& operator << (std::ostream & out, const CTime & obj)
    {
        out << setfill('0') << setw(2) << obj.m_Hour << ":"
            << setfill('0') << setw(2) << obj.m_Minute << ":"
            << setfill('0') << setw(2) << obj.m_Second;
        return out;
    }

    void printTime() const
    {
        cout << m_Hour << " " << m_Minute << " " << m_Second << endl;
    }
    friend class ::CTimeTester;
};
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

//    CTime a = CTime(12,30,30);
//    a.printTime();
//    a + 30;
//    a.printTime();

    CTime a { 12, 30 };
    assert( CTimeTester::test( a, 12, 30, 0 ) );

    CTime b { 13, 30 };
    assert( CTimeTester::test( b, 13, 30, 0 ) );

    assert( b - a == 3600 );

    assert( CTimeTester::test( a + 60, 12, 31, 0 ) );
    //a.printTime();
    assert( CTimeTester::test( a - 60, 12, 29, 0 ) );

    assert( a < b );
    assert( a <= b );
    assert( a != b );
    assert( !( a > b ) );
    assert( !( a >= b ) );
    assert( !( a == b ) );


   // while(true)
        //--a;
    while ( ++a != b );
//
    assert( a == b );
//
    std::ostringstream output;
    assert( static_cast<bool>( output << a ) );
    assert( output.str() == "13:30:00" );
//
    assert( a++ == b++ );
    a.printTime();
    b.printTime();
    assert( a == b );
//
    assert( --a == --b );
    assert( a == b );

    assert( a-- == b-- );
    assert( a == b );
    return 0;
}
#endif /* __PROGTEST__ */
