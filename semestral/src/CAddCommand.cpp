/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 5/10/23
 */

#include "CAddCommand.h"

std::unique_ptr<CCommand> CAddCommand::Clone() const {
    return std::make_unique<CAddCommand>(*this);
}

void CAddCommand::Execute(CCalendar & calendar, const CInterface & interface) {
    try {
        calendar.Add(interface);
    }
    catch (std::invalid_argument & err) {
        interface.PrintPrompt(err.what());
    }
    catch (std::logic_error & err) {
        interface.PrintPrompt(err.what());
    }
}

