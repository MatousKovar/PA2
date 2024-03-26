/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 6/10/23
 */

#include "CHelpCommand.h"

void CHelpCommand::Execute(CCalendar & calendar, const CInterface & interface) {
    interface.PrintPrompt("Type command(view, add, remove, find, query, save, load)\n");
}

std::unique_ptr<CCommand> CHelpCommand::Clone() const {
    return std::make_unique<CHelpCommand>();
}
