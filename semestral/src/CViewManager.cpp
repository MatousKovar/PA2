/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 6/5/23
 */

#include "CViewManager.h"

bool CViewManager::IsDigits(const std::string & str) {
    return str.find_first_not_of("0123456789 \n.:") == std::string::npos;
}

void CViewManager::GetView(const CInterface & interface, const CCalendar & calendar) {
    std::string type = interface.ReadCommand("Input type of view:\n");
    if (m_Views.find(type) == m_Views.end())
        throw std::runtime_error("Invalid type of View (types: day, week, month)\n");
    CDate date = GetDate(interface);
    m_Views[type]->Print(calendar, date, interface);
}

CDate CViewManager::GetDate(const CInterface & interface) {
    std::string buffer = interface.ReadCommand("Input date[dd.mm.yyyy]:\n");
    std::stringstream stream(buffer);
    int day = GetAttribute(stream, '.');
    int month = GetAttribute(stream, '.');
    int year = GetAttribute(stream, '\n');
    return CDate(year, month, day, 0, 0);

}

int CViewManager::GetAttribute(std::stringstream & stream, const char delimiter) {
    std::string attrString;
    try {
        std::getline(stream, attrString, delimiter);
        if (!IsDigits(attrString))
            throw std::invalid_argument("Invalid date: " + attrString + "\n");
        int attrInt = std::stoi(attrString);
        return attrInt;
    }
    catch (std::exception & err) {
        throw std::invalid_argument("Invalid attribute: " + attrString + "\n");
    }
}

