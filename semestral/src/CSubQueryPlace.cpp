/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 6/8/23
 */

#include "CSubQueryPlace.h"

std::vector<CEvent> CSubQueryPlace::Execute(std::string value, const CCalendar & calendar) {
    return calendar.GetByPlace(value);
}

std::unique_ptr<CSubQuery> CSubQueryPlace::Clone() {
    return std::make_unique<CSubQueryPlace>(*this);
}
