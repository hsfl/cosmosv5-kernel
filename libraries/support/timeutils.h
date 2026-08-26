/********************************************************************
* Copyright (C) 2015 by Interstel Technologies, Inc.
*   and Hawaii Space Flight Laboratory.
*
* This file is part of the COSMOS/core that is the central
* module for COSMOS. For more information on COSMOS go to
* <http://cosmos-project.com>
*
* The COSMOS/core software is licenced under the
* GNU Lesser General Public License (LGPL) version 3 licence.
*
* You should have received a copy of the
* GNU Lesser General Public License
* If not, go to <http://www.gnu.org/licenses/>
*
* COSMOS/core is free software: you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public License
* as published by the Free Software Foundation, either version 3 of
* the License, or (at your option) any later version.
*
* COSMOS/core is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* Refer to the "licences" folder for further information on the
* condititons and terms to use this software.
********************************************************************/

// just for the old code for timeval, deprecated
//#if defined(_MSC_VER) && (_MSC_VER >= 1020)
//#include <winsock.h>
//#endif

/*!
 * \file timeutils.h
 * \brief Time formatting and calendar utility functions.
 * \ingroup cosmos_kernel_time
 * \defgroup cosmos_timeutils Time Utilities
 *
 * Builds on timelib.h to add human-readable time formatting (ISO 8601, GPS
 * week/second strings), calendar arithmetic (day-of-year, leap-year checks),
 * and parsing of common time string representations.
 */

#ifndef TIME_UTILS
#define TIME_UTILS

#include "support/configCosmos.h"
#include <iostream>
#include <string>

#ifndef CROSS_TYPE_arm
#include <chrono>
#include <ctime>

class TimeUtils {

public:

    //! \brief Format a system_clock time_point as a local-time string.
    //! \param tp Time point to format.
    //! \return Human-readable local-time string.
    string timeString(const std::chrono::system_clock::time_point &tp);

    //! \brief Construct a system_clock time_point from calendar components (UTC).
    //! \param year Gregorian year.
    //! \param mon  Month (1–12).
    //! \param day  Day (1–31).
    //! \param hour Hour (0–23).
    //! \param min  Minute (0–59).
    //! \param sec  Second (0–59).
    //! \return Corresponding system_clock time_point.
    std::chrono::system_clock::time_point makeTimePoint(int year, int mon, int day, int hour, int min, int sec);

    //! \brief Return the current time as a system_clock time_point (UTC).
    //! \return Current UTC time_point.
    std::chrono::system_clock::time_point timePointUtc();

    //! \brief Fractional seconds since the Unix epoch.
    //! \return Current time in fractional seconds since 1970-01-01T00:00:00Z.
    double secondsSinceEpoch();

    //! \brief Duration elapsed since midnight (local time).
    //! \return system_clock::duration since 00:00:00 today.
    std::chrono::system_clock::duration secondsSinceMidnight();

    //! \brief Self-test for secondsSinceMidnight(); prints results to stdout.
    void testSecondsSinceMidnight();
};

#endif

#endif
