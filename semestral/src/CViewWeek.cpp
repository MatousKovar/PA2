/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 5/9/23
 */

#include "CViewWeek.h"

std::shared_ptr<CView> CViewWeek::Clone() {
    return std::make_shared<CViewWeek>(*this);
}


void CViewWeek::Print(const CCalendar & calendar, const CDate & date, const CInterface & interface) const {
    interface.PrintPrompt("Events for week from " + date.DayOfWeek() + ", " + date.PrintJustDate() + "\n");

    CDate currentDate = date;
    CDate endDate = date.DateWeekLater();

    while (currentDate < endDate) {
        std::vector<CEvent> events = calendar.GetEventsIn(++currentDate, currentDate.NextDay());
        for (const auto & event: events) {
            currentDate = PrintEvent(interface, event, date, endDate);
        }
        //no events starting in given timespan
        if (events.size() == 0) {
            interface.PrintPrompt(currentDate.DayOfWeek() + ":\n");
            currentDate = currentDate.NextDay();
            continue;
        }
        // if events that start in one day also end in one day - preventing infinite loop
        if ((events.end() - 1)->GetTo() < currentDate.NextDay() &&
            ((events.end() - 1)->GetTo().GetDay() == (events.end() - 1)->GetFrom().GetDay()))
            currentDate = currentDate.NextDay();
    }
}

CDate
CViewWeek::PrintEvent(const CInterface & interface, const CEvent & event, const CDate & from, const CDate & to) const {
    event.PrintWeekEvent(interface);
    return (event.GetTo());
}


