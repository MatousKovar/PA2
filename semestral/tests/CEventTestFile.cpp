/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 6/1/23
 */

#include "../src/CEvent.h"
#include "../src/CDate.h"
#include "../src/CInterface.h"
#include <iostream>
#include <string>
#include <cassert>
#include <stdexcept>

using namespace std;

int main ()
{
    CInterface interface;
    try {
        CEvent event1(CEvent::GetDate("1.1.2002 11:00"), CEvent::GetDate("2.2.2002 11:30"), "Some note", "Work",
                      "Prague 1, Namesti Republiky", "DO NOT FORGET", "Pick up a letter","asdf");
        CEvent event2(CEvent::GetDate("2.1.2002 11:00"), CEvent::GetDate("2.2.2002 11:30"), "Some note", "Work","asdf",
                      "Prague 1, Namesti Republiky", "DO NOT FORGET", "Pick up a letter");
        assert(event1 < event2);
        assert( event1.Intersecting(event2));
        CEvent event4 (CEvent::GetDate("1.1.2002 10:59"), CEvent::GetDate("2.2.2002 11:30"), "Some note", "Work", "Prague 1, Namesti Republiky", "DO NOT FORGET", "Pick up a letter","asdfasdf");
        assert (event1.Intersecting(event2));
        CEvent event5 (CEvent::GetDate("1.1.2002 11:00"), CEvent::GetDate("2.2.2002 11:10"), "Some note", "Work", "Prague 1, Namesti Republiky", "DO NOT FORGET", "Pick up a letter","asdfasf");
        assert (event1.Intersecting(event5));
    }
    catch ( ... )
    {
        assert("Vyhozena vyjimka" == nullptr);
    }

    CEvent event3 = CEvent::ReadEvent(interface);
//    //Testing invalid date exception
    try {
        CEvent event4 = CEvent::ReadEvent(interface);
        assert("Nebyla vyhozena vyjimka" == nullptr);
    }
    catch (const std::logic_error & err)
    {
    }
    catch ( ... )
    {
        assert("Invalid exception thrown" == nullptr);
    }
//
    try{
        CEvent event6(CEvent::GetDate("10.1.1111 11:11"), CEvent::GetDate("9.1.1111 11:11"), "sdf ","asdf", "asdf", "ioewnkc","asdf","asdfafsasd,asdf asdf");
        assert("Nevyhozena vyjimka" == nullptr);
    }
    catch (const std::logic_error & err)
    {
    }
    catch ( ... )
    {
        assert("Invalid exception thrown " == nullptr);
    }



    return 0;
}