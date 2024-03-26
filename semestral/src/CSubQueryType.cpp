/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 6/8/23
 */

#include "CSubQueryType.h"

std::vector<CEvent> CSubQueryType::Execute(std::string value, const CCalendar & calendar) {
    return calendar.GetByType(value);
}

std::unique_ptr<CSubQuery> CSubQueryType::Clone() {
    return std::make_unique<CSubQueryType>(*this);
}
