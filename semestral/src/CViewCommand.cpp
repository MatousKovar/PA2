/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 5/29/23
 */

#include "CViewCommand.h"

std::unique_ptr<CCommand> CViewCommand::Clone() const {
    return std::make_unique<CViewCommand>(*this);
}

void CViewCommand::Execute(CCalendar & calendar, const CInterface & interface) {
    try {
        manager->GetView(interface, calendar);
    }
    catch (std::exception & err) {
        interface.PrintPrompt(err.what());
    }
}
