/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 6/8/23
 */

#include "CQueryCommand.h"

void CQueryCommand::Execute(CCalendar & calendar, const CInterface & interface) {
    try {
        query->Run(calendar, interface);
    }
    catch (std::invalid_argument & err) {
        interface.PrintPrompt(err.what());
    }
    catch (std::runtime_error & err) {
        interface.PrintPrompt(err.what());
    }
    catch (std::logic_error & err) {
        interface.PrintPrompt(err.what());
    }
}

std::unique_ptr<CCommand> CQueryCommand::Clone() const {
    return std::make_unique<CQueryCommand>();
}
