/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 5/10/23
 */

#include "CFindCommand.h"

std::unique_ptr<CCommand> CFindCommand::Clone() const {
    return std::make_unique<CFindCommand>(*this);
}

void CFindCommand::Execute(CCalendar & calendar, const CInterface & interface) {
    try {
        calendar.Find(interface);
    }
    catch (std::exception & err) {}
}


