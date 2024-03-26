/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 6/8/23
 */

#include "CSubQueryTag.h"

std::vector<CEvent> CSubQueryTag::Execute(std::string value, const CCalendar & calendar) {
    return calendar.GetByTag(value);
}

std::unique_ptr<CSubQuery> CSubQueryTag::Clone() {
    return std::make_unique<CSubQueryTag>(*this);
}
