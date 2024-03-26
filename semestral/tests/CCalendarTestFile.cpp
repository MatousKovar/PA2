/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 6/1/23
 */
#include "../src/CCalendar.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <cassert>
using namespace std;
class CCalendarTest
{
public:
    CCalendarTest(CCalendar & calendar) : m_Calendar(calendar), m_Interface(){}
    void Run()
    {
        assert ( m_Calendar.Find(CEvent::GetDate("20.11.2002 11:11")) == m_Calendar.m_Events.end());
        assert (m_Calendar.Add(CEvent(CEvent::GetDate("11.11.2002 11:11"),CEvent::GetDate("12.11.2002 11:11"), "Some note", "Work", "Easy street", "Do not forget", "Meeting", "")));
        assert(! m_Calendar.Add(CEvent(CEvent::GetDate("10.11.2002 11:11"),CEvent::GetDate("12.11.2002 11:11"), "Some note", "Work", "Easy street", "Do not forget", "Meeting", " ")));
        assert(! m_Calendar.Add(CEvent(CEvent::GetDate("11.11.2002 11:11"),CEvent::GetDate("12.11.2002 11:11"), "Some note", "Work", "Easy street", "Do not forget", "Meeting", " ")));
        assert(! m_Calendar.Add(CEvent(CEvent::GetDate("12.11.2002 11:11"),CEvent::GetDate("15.11.2002 11:11"), "Some note", "Work", "Easy street", "Do not forget", "Meeting", " ")));
        try {
            m_Calendar.Add(CEvent(CEvent::GetDate("60.11.2002 11:11"),CEvent::GetDate("15.11.2002 11:11"), "Some note", "Work", "Easy street", "Do not forget", "Meeting", " "));
            assert("No exception thrown" ==  nullptr);
        }
        catch (...) {}

        try {
            m_Calendar.Add(CEvent(CEvent::GetDate("11.11.2002 11:11"),CEvent::GetDate("8.11.2002 11:11"), "Some note", "Work", "Easy street", "Do not forget", "Meeting",""));
            assert("No exception thrown" ==  nullptr);
        }
        catch (...) {}

        assert (m_Calendar.Add(CEvent(CEvent::GetDate("15.11.2002 11:12"),CEvent::GetDate("20.11.2002 11:11"), "Some note", "Work", "Easy street", "Do not forget", "Meeting","")));
        assert (m_Calendar.Add(CEvent(CEvent::GetDate("4.11.2002 11:11"),CEvent::GetDate("9.11.2002 11:11"), "Some note", "Work", "Easy street", "Do not forget", "Meeting","")));
        assert ( ! m_Calendar.Add(CEvent(CEvent::GetDate("3.11.2002 11:11"),CEvent::GetDate("5.11.2002 11:11"), "Some note", "Work", "Easy street", "Do not forget", "Meeting","")));
        assert (m_Calendar.Add(CEvent(CEvent::GetDate("3.11.2022 11:11"),CEvent::GetDate("5.11.2022 11:11"), "Some note", "Work", "Easy street", "Do not forget", "Meeting","")));

        assert ( ! m_Calendar.Remove(CEvent::GetDate("1.1.2000 11:11"), m_Interface));
        assert (  m_Calendar.Remove(CEvent::GetDate("3.11.2022 11:11"), m_Interface));
        assert (m_Calendar.Add(CEvent(CEvent::GetDate("4.11.1900 11:11"),CEvent::GetDate("9.11.1999 11:11"), "Some note", "Work", "Easy street", "Do not forget", "Meeting","")));
        assert (m_Calendar.Remove(CEvent::GetDate("4.11.1900 11:11"),m_Interface));


        try
        {
            m_Calendar.Remove(CEvent::GetDate("30.2.2000 11:11"),m_Interface);
            assert("Exception not thrown" == nullptr);
        }
        catch (...) {}
        assert (m_Calendar.Add(CEvent(CEvent::GetDate("20.11.2002 11:12"),CEvent::GetDate("20.11.2002 11:55"), "Some note", "Work", "Easy street", "Do not forget", "Meeting","")));

        assert ( m_Calendar.Find(CEvent::GetDate("20.11.2002 11:11")) != m_Calendar.m_Events.end());
        assert ( m_Calendar.Find(CEvent::GetDate("20.11.2002 11:12")) != m_Calendar.m_Events.end());
        assert ( m_Calendar.Find(CEvent::GetDate("20.11.2002 11:15")) != m_Calendar.m_Events.end());

        assert ( m_Calendar.Find(CEvent::GetDate("20.11.2002 11:55"))!= m_Calendar.m_Events.end());
        assert ( m_Calendar.Find(CEvent::GetDate("20.11.2002 11:56"))== m_Calendar.m_Events.end());

        //adding to the last index in events
        assert (m_Calendar.Add(CEvent(CEvent::GetDate("20.11.4000 11:12"),CEvent::GetDate("20.11.4000 11:55"), "Some note", "Work", "Easy street", "Do not forget", "Meeting","")));
        //addint to the first index in events
        assert (m_Calendar.Add(CEvent(CEvent::GetDate("20.11.1800 11:12"),CEvent::GetDate("20.11.1800 11:55"), "Some note", "Work", "Easy street", "Do not forget", "Meeting","")));
        //searching for first
        assert ( m_Calendar.Find( CEvent::GetDate("20.11.1800 11:12"))!= m_Calendar.m_Events.end());
        //searching for last
        assert ( m_Calendar.Find( CEvent::GetDate("20.11.4000 11:12"))!= m_Calendar.m_Events.end());

        assert (m_Calendar.Remove(CEvent::GetDate("15.11.2002 11:12"),m_Interface));
        try {
            assert (!m_Calendar.Remove(CEvent::GetDate("15.11.200211:12"), m_Interface));
            assert ("Exception not thrown" == nullptr);
        }
        catch (std::invalid_argument & err){}
        assert (m_Calendar.Remove(CEvent::GetDate("20.11.4000 11:12"),m_Interface));
        assert (m_Calendar.Remove(CEvent::GetDate("20.11.1800 11:12"),m_Interface));
        try {
            assert (!m_Calendar.Add(
                    CEvent(CEvent::GetDate("20a.11.1800 11:12"), CEvent::GetDate("20.11.1800 11:55"), "Some note",
                           "Work", "Easy street", "Do not forget", "Meeting", "")));
            assert ("No Exception thrown" == nullptr);
        }
        catch (std::invalid_argument & err)
        {}

    }
private:
    CCalendar m_Calendar;
    CInterface m_Interface;

};
int main()
{
    CCalendar calendar;
    CCalendarTest tester(calendar);
    tester.Run();

    return 0;
}