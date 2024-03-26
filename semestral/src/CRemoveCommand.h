/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 5/10/23
 */

#pragma once

#include "CCommand.h"
#include <string>

/**
 * Represents command for removing events
 */

class CRemoveCommand : public CCommand {
public:

    CRemoveCommand() {}

    void Execute(CCalendar & calendar, const CInterface & interface) override;

    std::unique_ptr<CCommand> Clone() const override;
};


