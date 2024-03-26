/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 6.5.2023
 */

#pragma once

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>


class CDate {
public:
    CDate(unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute);

    CDate() = default;

//    CDate (const std::string & date) {ParseDate(date);};

    CDate(const CDate & other) = default;

    ~CDate() = default;

    bool operator<(const CDate & other) const;

    friend std::ostream & operator<<(std::ostream & out, const CDate & date);

    bool operator==(const CDate & other) const;

    bool operator<=(const CDate & other) const;

    /**
     * Adds one minute to date
     * @return
     */
    CDate & operator++();

    /**
     * @return CDate with day Increased by 1 and time set to 00:00
     */
    CDate NextDay() const;

    CDate NextDayKeepTime() const;

    CDate DateWeekLater() const;

    CDate DateWeekLaterKeepTime() const;

    CDate DateMonthLater() const;

    CDate DateMonthLaterKeepTime() const;

    /**
     * @return "m_Day.m_Month.m_Year m_Hour:m_Minute"
     */
    std::string PrintDate() const;

    /**
     * @return "m_Day.m_Month.m_Year"
     */
    std::string PrintJustDate() const;

    /**
     * @return "m_Hour:m_Minute"
     */
    std::string PrintJustTime() const;

    /**
     * m_Day.m_Month
     * @return
     */
    std::string PrintJustDateNoYear() const;


    bool IsInRange(const CDate & from, const CDate & to) const;

    /**
     * Tests logic of given date
     */
    bool ValidateDate(unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute) const;


    std::string DayOfWeek() const;

    int GetYear() const { return m_Year; }

    int GetMonth() const { return m_Month; }

    int GetDay() const { return m_Day; }

    int GetHour() const { return m_Hour; };

    int GetMinute() const { return m_Minute; }

private:
    static bool IsLeap(unsigned int year);

    unsigned int m_Year;
    unsigned int m_Month;
    unsigned int m_Day;
    unsigned int m_Hour;
    unsigned int m_Minute;
};
