/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 5/9/23
 */

#include "CViewMonth.h"

std::shared_ptr<CView> CViewMonth::Clone() {
    return std::make_shared<CViewMonth>(*this);
}

void CViewMonth::Print(const CCalendar & calendar, const CDate & date, const CInterface & interface) const {
    interface.PrintPrompt("Events planned for month from: " + date.PrintJustDate() + "\n");
    CDate CurrentDate = date;
    CDate EndDate = date.DateMonthLater();

    while (CurrentDate <= EndDate) {
        std::vector<CEvent> events = calendar.GetEventsIn(++CurrentDate, CurrentDate.NextDay());

        for (const auto & event: events) {
            CurrentDate = PrintEvent(interface, event);
        }
        if (events.size() == 0) {
            interface.PrintPrompt(CurrentDate.PrintJustDateNoYear() + ":\n");
            CurrentDate = CurrentDate.NextDay();
            continue;
        }
        if ((events.end() - 1)->GetTo() < CurrentDate.NextDay() &&
            ((events.end() - 1)->GetTo().GetDay() == (events.end() - 1)->GetFrom().GetDay()))
            CurrentDate = CurrentDate.NextDay();
        else
            CurrentDate = (events.end() - 1)->GetTo();
    }
}

CDate CViewMonth::PrintEvent(const CInterface & interface, const CEvent & event) const {
    event.PrintMonthEvent(interface);
    return event.GetTo();
}


