/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 6/5/23
 */

#include "../src/CCalendar.h"
#include "../src/CInterface.h"
#include "../src/CViewDay.h"
#include "../src/CViewWeek.h"
#include "../src/CViewMonth.h"
#include "../src/CViewManager.h"

#include <cassert>
#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>


int main ()
{
    CCalendar calendar;
    CInterface interface;
    CViewDay view;
    CViewWeek viewWeek;
    CViewMonth viewMonth;

    assert (calendar.Add(CEvent(CEvent::GetDate("14.11.2002 10:15"),CEvent::GetDate("15.11.2002 10:00"), "Somalsko", "Work", "Easy street", "Do not forget", "Meeasssdfating","asdf")));
    assert (calendar.Add(CEvent(CEvent::GetDate("15.11.2002 10:15"),CEvent::GetDate("15.11.2002 10:45"), "Some note", "Work", "Easy street", "Do not forget", "Meeting","asdf")));
    assert (calendar.Add(CEvent(CEvent::GetDate("15.11.2002 12:15"),CEvent::GetDate("15.11.2002 14:00"), "Some other note", "Work", "Campus", "Do not forget", "Meeting","asdf")));
    assert (calendar.Add(CEvent(CEvent::GetDate("15.11.2002 15:15"),CEvent::GetDate("15.11.2002 16:45"), "Nothing", "Work", "Warehouse", "none", "Meeting","asdff")));
    assert (calendar.Add(CEvent(CEvent::GetDate("15.11.2002 17:00"),CEvent::GetDate("15.11.2002 18:00"), "Nothing", "Work", "Warehouse", "none", "Meeting","asdfasdf")));

    assert (calendar.Add(CEvent(CEvent::GetDate("15.11.2003 17:00"),CEvent::GetDate("15.11.2004 18:00"), "Nothing", "Work", "Warehouse", "none", "Meeting","asdfasdf")));
    viewWeek.Print(calendar,CViewManager::GetDate(interface), interface);
//    view.Print(calendar, CViewManager::GetDate(interface));
    calendar.SaveEvents(interface);

    assert (calendar.Add(CEvent(CEvent::GetDate("16.11.2002 17:00"),CEvent::GetDate("17.11.2002 18:59"), "Nothing", "Work", "Warehouse", "none", "Meeting","asdfasdf")));
    assert (calendar.Add(CEvent(CEvent::GetDate("18.11.2002 17:00"),CEvent::GetDate("18.11.2002 18:59"), "Nothing", "Work", "Warehouse", "none", "Meeting","asdfasdf")));
    assert (calendar.Add(CEvent(CEvent::GetDate("18.11.2002 19:00"),CEvent::GetDate("25.11.2002 13:00"), "Nothing", "Work", "Warehouse", "none", "Meeting","asdfasdf")));
    try {
        assert ( ! calendar.Add(CEvent(CEvent::GetDate("18.11.20000002 19:00"),CEvent::GetDate("25.11.2002 13:00"), "Nothing", "Work", "Warehouse", "none", "Meeting","asdfasdf")));
        assert ("No exception thrown" == nullptr);
    }
    catch (std::logic_error & ex) {}
    catch ( ...)
    {
        assert ("Wrong exception" == nullptr);
    }
    try {
        assert ( ! calendar.Add(CEvent(CEvent::GetDate("18.11.2002 19:00"),CEvent::GetDate("25.11.2001 13:00"), "Nothing", "Work", "Warehouse", "none", "Meeting","asdfasdf")));
        assert ("No exception thrown" == nullptr);
    }
    catch (std::logic_error & ex) {}
    catch ( ...)
    {
        assert ("Wrong exception" == nullptr);
    }
    assert (calendar.Remove(CEvent::GetDate("15.11.2002 10:15"),interface));
    assert (! calendar.Remove(CEvent::GetDate("15.11.2002 10:15"),interface));
    assert (calendar.Add(CEvent(CEvent::GetDate("15.12.2002 12:15"),CEvent::GetDate("15.12.2002 14:00"), "Some other note", "Work", "Campus", "Do not forget", "Meeting","asdf")));
    assert (calendar.Add(CEvent(CEvent::GetDate("15.1.2002 12:15"),CEvent::GetDate("15.1.2002 14:00"), "Some other note", "Work", "Campus", "Do not forget", "Meeting","asdf")));

    CCalendar calendar2;
    assert (calendar2.Add(CEvent(CEvent::GetDate("13.11.2002 12:15"),CEvent::GetDate("15.11.2002 10:00"), "Some other note", "Work", "Campus", "Do not forget", "Meeting","asdf")));
    assert (calendar2.Add(CEvent(CEvent::GetDate("15.11.2002 12:15"),CEvent::GetDate("15.11.2002 14:00"), "Some other note", "Work", "Campus", "Do not forget", "Meeting","asdf")));
    assert (calendar2.Add(CEvent(CEvent::GetDate("16.11.2002 12:15"),CEvent::GetDate("18.11.2002 14:00"), "Some other note", "Work", "Campus", "Do not forget", "Meeting","asdf")));
    assert (calendar2.Add(CEvent(CEvent::GetDate("18.11.2002 15:00"),CEvent::GetDate("25.11.2002 14:00"), "Some other note", "Work", "Campus", "Do not forget", "Meeting","asdf")));

    return 0;
}