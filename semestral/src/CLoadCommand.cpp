/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 5/10/23
 */

#include "CLoadCommand.h"

std::unique_ptr<CCommand> CLoadCommand::Clone() const {
    return std::make_unique<CLoadCommand>(*this);
}

void CLoadCommand::Execute(CCalendar & calendar, const CInterface & interface) {
    calendar.LoadEvents(interface);
}
