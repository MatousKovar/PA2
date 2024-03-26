/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 6/6/23
 */

#pragma once

#include "CInterface.h"
#include "CEvent.h"
#include "CCalendar.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
/**
 * Class for managing loading and saving to files
 */
class CFileManager {
public:
    static bool SaveEvents(const CInterface & interface, const std::vector<CEvent> & events);

    static bool LoadEvents(const CInterface & interface, std::vector<CEvent> & events);

    static bool LoadEvents(const CInterface & interface, std::vector<CEvent> & events, std::string filename);

private:
    /**
     * Reads one attribute from line in stream, checks for " ", and removes them
     * @param stream
     * @param delimiter
     * @return
     */
    static std::string GetAttribute(std::stringstream & stream, const char delimiter);

    /**
     * Method for reading whole event from line from file
     * @param line - line that was read from file
     * @return
     */
    static CEvent ReadEvent(std::string line);
};


