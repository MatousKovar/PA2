/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 5/10/23
 */

#include "CSaveCommand.h"

std::unique_ptr<CCommand> CSaveCommand::Clone() const {
    return std::make_unique<CSaveCommand>(*this);
}

void CSaveCommand::Execute(CCalendar & calendar, const CInterface & interface) {
    try {
        calendar.SaveEvents(interface);
    }
    catch (std::runtime_error & err) {
        interface.PrintPrompt(err.what());
    }
}

