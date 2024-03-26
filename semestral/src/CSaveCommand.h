/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 5/10/23
 */
#include "CCommand.h"

#pragma once

/**
 * Representing command for saving events to file
 */

class CSaveCommand : public CCommand {
public:

    CSaveCommand() {};

    void Execute(CCalendar & calendar, const CInterface & interface) override;

    std::unique_ptr<CCommand> Clone() const override;

};


