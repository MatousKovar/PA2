/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 5/29/23
 */

#pragma once

#include "CViewManager.h"
#include "CCommand.h"
#include <map>
#include <string>
#include <memory>


class CViewCommand : public CCommand {
public:
    CViewCommand() { manager = std::make_shared<CViewManager>(); };

    ~CViewCommand() override = default;

    void Execute(CCalendar & calendar, const CInterface & interface) override;

    std::unique_ptr<CCommand> Clone() const override;

private:
    std::shared_ptr<CViewManager> manager;
};


