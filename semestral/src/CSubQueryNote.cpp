/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 6/8/23
 */

#include "CSubQueryNote.h"

std::vector<CEvent> CSubQueryNote::Execute(std::string value, const CCalendar & calendar) {
    return calendar.GetByNote(value);
}

std::unique_ptr<CSubQuery> CSubQueryNote::Clone() {
    return std::make_unique<CSubQueryNote>(*this);
}
