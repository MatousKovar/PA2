/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 5/10/23
 */

#pragma once

#include "CCommand.h"

/**
 * Representing command for loading events from file
 */
class CLoadCommand : public CCommand {
public:

    CLoadCommand() {};

    void Execute(CCalendar & calendar, const CInterface & interface) override;

    std::unique_ptr<CCommand> Clone() const override;
};


