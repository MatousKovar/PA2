#ifndef __PROGTEST__
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
#endif /* __PROGTEST__ */

class CTimeStamp
{
  public:
    CTimeStamp(int year, int month, int day, int hour, int minute, int second)
    :    m_Year(year),
         m_Month(month),
         m_Day(day),
         m_Hour(hour),
         m_Min(minute),
         m_Sec(second)
    {}
    bool operator< (const CTimeStamp & rhs) const
    {
        if (m_Year == rhs.m_Year)
        {
            if (m_Month == rhs.m_Month)
            {
                if( m_Day == rhs.m_Day)
                {

                }
                else
                    return m_Day < rhs.m_Day;
                if (m_Hour == rhs.m_Hour)
                {
                    if (m_Min == rhs.m_Min)
                    {
                        return m_Sec < rhs.m_Sec;
                    }
                    else
                        return m_Min < rhs.m_Min;
                }
                else
                    return m_Hour < rhs.m_Hour;
            }
            else
                return m_Month < rhs.m_Month;
        }
        else
            return m_Year < rhs.m_Year;
    }
    bool operator== (const CTimeStamp & rhs) const
    {
        return (m_Year == rhs.m_Year && m_Month == rhs.m_Month && m_Day == rhs.m_Day && m_Hour == rhs.m_Hour && m_Min == rhs.m_Min && m_Sec == rhs.m_Sec);
    }
    bool operator <= (const CTimeStamp & rhs) const
    {
        return (*this == rhs || *this < rhs);
    }


private:
    int m_Year,m_Month,m_Day,m_Hour,m_Min,m_Sec;
};
class CContact
{
  public:
    CContact(CTimeStamp timeStamp, int phone1,int phone2)
    :   m_TimeStamp(timeStamp),
        m_Phone1(phone1),
        m_Phone2(phone2)
    {}
    bool operator< (const CContact & rhs) const
    {
        return m_TimeStamp < rhs.m_TimeStamp;
    }
    bool isInRange(const CTimeStamp & from, const CTimeStamp & to) const
    {
        return (m_TimeStamp <= to && (from <= m_TimeStamp));
    }
private:
    friend class CEFaceMask;
    CTimeStamp m_TimeStamp;
    int m_Phone1;
    int m_Phone2;
};
class CEFaceMask
{
  public:
    CEFaceMask()
    {
    }
    CEFaceMask & addContact( const CContact & a )
    {
        m_Data.insert(a);
        m_People[a.m_Phone1].m_numberOfContacts++;
        m_People[a.m_Phone1].m_Contacts.insert(a);
        m_People[a.m_Phone2].m_numberOfContacts++;
        m_People[a.m_Phone2].m_Contacts.insert(a);
        return *this;
    }
    // default constructor
    // addContact ( contact )
    // getSuperSpreaders ( from, to )
    int contactsInTimeStamp(const set<CContact> & a, const CTimeStamp & from , const CTimeStamp & to  )
    {
        int res = 0;
        for ( auto i = a.begin() ; i != a.end() ; i++ )
        {
            if (i->isInRange(from,to))
                res ++;
        }
        return res;
    }
    vector<int> getSuperSpreaders (const CTimeStamp &  from,const CTimeStamp &  to )
    {
        int max = 0;
        vector<int> res;
        for ( auto const& a : m_People)
        {
            int personContacts = contactsInTimeStamp(a.second.m_Contacts, from, to);
            if (personContacts > max)
            {
                max = personContacts;
                res.clear();
                res.push_back(a.first);
                continue;
            }
            if (personContacts == max )
                res.push_back(a.first);
        }
        return res;
    }
private:
    struct defaultMapValue{
        int m_numberOfContacts = 0;
        set<CContact> m_Contacts;
    };
    set<CContact> m_Data;
    map<int, defaultMapValue> m_People;
};

void printVector ( const vector<int> & a)
{
    for (size_t i = 0 ; i < a.size() ; i++)
    {
        cout << a[i] << " ";
    }
    cout << endl;
}
#ifndef __PROGTEST__
int main ()
{
  CEFaceMask test;

  test . addContact ( CContact ( CTimeStamp ( 2021, 1, 10, 12, 40, 10 ), 111111111, 222222222 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 1, 12, 12, 40, 10 ), 333333333, 222222222 ) )
       . addContact ( CContact ( CTimeStamp ( 2021, 2, 14, 15, 30, 28 ), 222222222, 444444444 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 2, 15, 18, 0, 0 ), 555555555, 444444444 ) );
  assert ( test . getSuperSpreaders ( CTimeStamp ( 2021, 1, 1, 0, 0, 0 ), CTimeStamp ( 2022, 1, 1, 0, 0, 0 ) ) == (vector<int> {222222222}) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 3, 20, 18, 0, 0 ), 444444444, 666666666 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 3, 25, 0, 0, 0 ), 111111111, 666666666 ) );
  assert ( test . getSuperSpreaders (  CTimeStamp ( 2021, 1, 1, 0, 0, 0 ), CTimeStamp ( 2022, 1, 1, 0, 0, 0 ) ) == (vector<int> {222222222, 444444444}) );



  CEFaceMask test2;
  test2.addContact(CContact(CTimeStamp(2021,1,1,1,1,1),111111111,222222222));
  test2.addContact(CContact(CTimeStamp(2021,1,1,1,1,1),111111111,222222222));
  test2.addContact(CContact(CTimeStamp(2021,1,1,1,1,1),111111111,222222222));
  test2.addContact(CContact(CTimeStamp(2021,1,1,1,1,1),111111111,222222222));
  test2.addContact(CContact(CTimeStamp(2021,1,1,1,1,2),111111111,333333333));
  test2.addContact(CContact(CTimeStamp(2021,1,1,1,1,3),111111111,444444444));
  test2.addContact(CContact(CTimeStamp(2021,1,1,1,2,1),111111111,444444444));

  vector<int> a = test2.getSuperSpreaders(CTimeStamp(2021,1,1,1,1,1),CTimeStamp(2021,1,1,1,1,1));
  printVector(a);
//  assert(test2.getSuperSpreaders(CTimeStamp(2020,1,1,1,1,1),CTimeStamp(2021,1,1,1,1,1)) == (vector<int> {111111111,222222222}));
  return 0;
}
#endif /* __PROGTEST__ */
