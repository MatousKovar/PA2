/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 6.5.2023
 */

#pragma once

#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdexcept>
#include "CDate.h"
#include "CInterface.h"

class CEvent {
public:
    CEvent(const CDate & mFrom, const CDate & mTo, const std::string & mNote, const std::string & mType,
           const std::string & mPlace, const std::string & mTag, const std::string & mName,
           const std::string & mPeople);

    CEvent(const CEvent & other) = default;

    ~CEvent() = default;

    std::string Print() const;

    /**
     * For sorting events based on their m_From attribute
     * @param other
     * @return
     */
    bool operator<(const CEvent & other) const;

    /**
     * Method for determining if there is a collission between two events
     * @param other
     * @return
     */
    bool Intersecting(const CEvent & other) const;

    /**
     * Main method for reading events
     * Responsible for reading correct format of date
     * Stores all attributes
     * @param interface - method reads from command line using interface
     * @return
     */
    static CEvent ReadEvent(const CInterface & interface);

    /**
     * Way of printing event when used in CViewDay
     * @param interface
     */
    void PrintDayEvent(const CInterface & interface) const;

    /**
     * Way of printing event when used in CViewMonth
     * @param interface
     */
    void PrintMonthEvent(const CInterface & interface) const;

    /**
     * Way of printing event when used in CViewWeek
     * @param interface
     */
    void PrintWeekEvent(const CInterface & interface) const;

    /**
     * @return event in format, that can be outputted into file
     */
    std::string SaveEvent() const;

    /**
     * @return CDate m_From
     */
    const CDate & GetFrom() const;

    /**
     * @return CDate m_TO
     */
    const CDate & GetTo() const;

    /**
     * Method for creating CDate object, Takes care of correct parsing
     * CDate realises just of date
     * Used also by CFileManager
     * @param interface
     * @return
     */
    static CDate GetDate(const std::string & date);

    const std::string & GetNote() const;

    const std::string & GetType() const;

    const std::string & GetPlace() const;

    const std::string & GetTag() const;

    const std::string & GetName() const;

    const std::string & GetPeople() const;

    /**
     * Changes date by day
     */
    CEvent ShiftEventDay() const;

    CEvent ShiftEventWeek() const;

    CEvent ShiftEventMonth() const;

private:
    /**
     * Method for getting CDate attributes from stringstream
     * Helper Method for ReadEvent
     * @param stream
     * @param delimiter
     * @return
     */
    static int GetAttribute(std::stringstream & stream, const char delimiter);
    /**
     * Helper method, to determine if string only contains valid characters
     * @param str
     * @return
     */
    static bool IsDigits(const std::string & str);

    CDate m_From;
    CDate m_To;
    std::string m_Note;
    std::string m_Type;
    std::string m_Place;
    std::string m_Tag;
    std::string m_Name;
    std::string m_People;
};



