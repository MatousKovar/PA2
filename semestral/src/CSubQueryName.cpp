/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 6/8/23
 */

#include "CSubQueryName.h"

std::vector<CEvent> CSubQueryName::Execute(std::string value, const CCalendar & calendar) {
    return calendar.GetByName(value);
}

std::unique_ptr<CSubQuery> CSubQueryName::Clone() {
    return std::make_unique<CSubQueryName>(*this);
}
