/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 6/8/23
 */

#include "CSubQueryPeople.h"

std::vector<CEvent> CSubQueryPeople::Execute(std::string value, const CCalendar & calendar) {
    return calendar.GetByPeople(value);
}

std::unique_ptr<CSubQuery> CSubQueryPeople::Clone() {
    return std::make_unique<CSubQueryPeople>(*this);
}
