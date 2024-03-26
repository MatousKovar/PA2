/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 5/10/23
 */

#include "CRemoveCommand.h"

std::unique_ptr<CCommand> CRemoveCommand::Clone() const {
    return std::make_unique<CRemoveCommand>(*this);
}

void CRemoveCommand::Execute(CCalendar & calendar, const CInterface & interface) {
    try {
        calendar.Remove(interface);
    }
    catch (std::invalid_argument & err) {
        interface.PrintPrompt(err.what());
    }
    catch (std::logic_error & err) {
        interface.PrintPrompt(err.what());
    }

}
