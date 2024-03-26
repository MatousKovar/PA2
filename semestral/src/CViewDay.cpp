/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 5/9/23
 */

#include "CViewDay.h"

std::shared_ptr<CView> CViewDay::Clone() {
    return std::make_shared<CViewDay>(*this);
}


int PrintEventStartingBefore(const CInterface & interface, const CEvent & event, const CDate & date) {
    if (event.GetFrom() < date) {
        interface.PrintPrompt(event.GetFrom().PrintDate());
        event.PrintDayEvent(interface);
        interface.PrintPrompt(".\n.\n");
        //event that takes up whole timespan
        if (date.NextDay() <= event.GetTo()) {
            interface.PrintPrompt(event.GetTo().PrintDate() + "\n");
            return 24;
        }
        return event.GetTo().GetHour();
    }
    return 0;
}

void CViewDay::IterateThroughEvents(int i, const CInterface & interface, size_t idx,
                                    const std::vector<CEvent> events) const {
    while (i < 24) {
        interface.PrintTime(i, 0);
        if (idx >= events.size()) {
            interface.PrintPrompt("\n");
            i++;
            continue;
        }
        if (events[idx].GetFrom().GetHour() == i) {
            i = PrintEvent(interface, events[idx]);
            idx++;
        }
        i++;
        interface.PrintPrompt("\n");
    }
}

void CViewDay::Print(const CCalendar & calendar, const CDate & date, const CInterface & interface) const {
    interface.PrintPrompt("Events planned for " + date.PrintJustDate() + "\n");
    std::vector<CEvent> events = calendar.GetEventsIn(date, date.NextDay());
    if (events.size() != 0) {
        size_t idx = 0;
        int i = PrintEventStartingBefore(interface, events[0], date);
        if (i != 0)
            idx++;
        IterateThroughEvents(i, interface, idx, events);
        return;
    }
    IterateThroughEvents(0, interface, 0, events);
}

int CViewDay::PrintEvent(const CInterface & interface, const CEvent & event) const {
    //Event that doesn't start at a whole hour, should print time it starts at also
    if (event.GetFrom().GetMinute() != 0) {
        interface.PrintPrompt("\n");
        interface.PrintTime(event.GetFrom().GetHour(), event.GetFrom().GetMinute());
    }
    event.PrintDayEvent(interface);
    //If event takes more than one day
    if (event.GetFrom().GetDay() != event.GetTo().GetDay()) {
        interface.PrintPrompt(".\n.\n");
        interface.PrintPrompt(event.GetTo().PrintDate());
        return 24;
    }
    interface.PrintPrompt(".\n.\n");
    interface.PrintTime(event.GetTo().GetHour(), event.GetTo().GetMinute());
    return event.GetTo().GetHour();
}

