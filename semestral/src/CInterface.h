/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 5/8/23
 */

#pragma once

#include <string>
#include <iostream>
#include <iomanip>

/**
 * Class interface will be responsible for I/O operations
 * It will pass the read commands back to CApplication which will determine what will happen next
 */
class CInterface {
public:
    CInterface(std::ostream & out = std::cout, std::istream & in = std::cin);

    CInterface(const CInterface & other) = delete;

    /**
     * reads command from prompt
     * @return name and arguments of command
     */
    std::string ReadCommand() const;

    bool IsEOF() const { return m_In.eof(); }

    std::string ReadCommand(const std::string & message) const;

    std::ostream & PrintPrompt(const std::string & a) const;

    std::ostream & PrintTime(const int timeHour, const int timeMinute) const;

    std::ostream & GetOutstream() const { return m_Out; };

private:
    std::ostream & m_Out;
    std::istream & m_In;
};


