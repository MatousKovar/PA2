/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 6.5.2023
 */

#pragma once

#include "CEvent.h"
#include "CInterface.h"
#include "CFileManager.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

/**
 * container for events, with some basic functionality to use
 */
class CCalendar {
public:
    CCalendar() = default;

    ~CCalendar() = default;

    /**
     * method will get CEvent - which will be created in command class??
     * @param event
     * @return
     */
    bool Add(const CInterface & interface);

    /**
     * Method called from Add (interface) method
     * This method is easier to create tests on
     * @param event
     * @param interface
     * @return
     */
    bool Add(const CEvent & event);

    /**
     * Method for deleting event based on its beginning date
     * This method uses overriden method Del below inside
     * This method is responsible for reading arguments from command line
     * @param interface
     * @return
     */
    bool Remove(const CInterface & interface);

    /**
     * Main funcitonality of del function
     * @param date
     * @param interface
     * @return
     */
    bool Remove(const CDate & date, const CInterface & interface);

    std::unique_ptr<CCalendar> Clone() const;

    /**
     * @param from
     * @param to
     * @return vector of CEvents in given range
     */
    std::vector<CEvent> GetEventsIn(const CDate & from, const CDate & to) const;

    /**
     * Reads filename from interface, and stores to file, it it is possible
     * @param interface
     * @return
     */
    bool SaveEvents(const CInterface & interface) const;


    /**
     * Finds event, that has is on given date
     * @param date
     * @return const iterator to event that meets criteria
     */
    std::vector<CEvent>::const_iterator Find(const CDate & date) const;

    /**
     * Uses above Find method, is able to print result
     * @param interface - interface to print result
     * @param date that event contains
     * @return
     */
    bool Find(const CInterface & interface) const;

    bool CheckForIntersection(const CEvent & event, std::vector<CEvent>::iterator iter) const;

    /**
     * GetBy methods are used by CQuery class
     * implemented by iterating through m_Events
     * @param date
     * @return
     */
    std::vector<CEvent> GetByDate(const CDate & date) const;

    std::vector<CEvent> GetByName(const std::string & name) const;

    std::vector<CEvent> GetByNote(const std::string & note) const;

    std::vector<CEvent> GetByPlace(const std::string & place) const;

    std::vector<CEvent> GetByPeople(const std::string & people) const;

    std::vector<CEvent> GetByType(const std::string & type) const;

    std::vector<CEvent> GetByTag(const std::string & tag) const;

    /**
     * Loads events knowing filename, used for loading from command line arguments
     * @param filename
     * @param interface
     * @return
     */
    bool LoadEvents(const std::string & filename, const CInterface & interface);

    bool LoadEvents(const CInterface & interface);

    /**
     * Method for creating Repeating events
     * @param interface
     * @param event
     */
    void RepeatEvent(const CInterface & interface, const CEvent & event);


private:
    void RepeatEventDay(const CEvent & event, int repeats);

    void RepeatEventWeek(const CEvent & event, int repeats);

    void RepeatEventMonth(const CEvent & event, int repeats);
    /**
     * Method for reading event from interface and creating object out of it
     * @param interface
     * @return
     */
    CEvent ReadEvent(const CInterface & interface) const;

    std::vector<CEvent> m_Events;

    friend class CCalendarTest;
};


