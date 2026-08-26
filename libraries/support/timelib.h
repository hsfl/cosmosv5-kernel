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

/*! \file timelib.h
    \brief timelib include file
    A library providing functions for handling various types of time.
*/

//! \ingroup support
//! \defgroup timelib Time handling library
//! Time systems support.
//!
//! The following time systems are supported withing COSMOS:
//! - Coordinated Universal Time (UTC)
//! - Universal Time (UT1)
//! - Terrestrial Time (TT)
//! - GPS Time
//! - Barycentric Dynamical Time (TDB)
//! - Greenwhich Mean Sidereal Time (GMST)
//! - Greenwhich Apparent Sidereal Time (GAST)
//! Except for Sidereal time, these are all represented internally as Modified Julian Day.
//! This library provides functions to convert between these systems, and to represent
//! MJD in various other forms.

/*!
 * \file timelib.h
 * \brief MJD, UTC, GPS time conversions and current-time access.
 * \ingroup cosmos_kernel_time
 * \defgroup cosmos_timelib Time Library
 *
 * Defines the Modified Julian Date (MJD) as the primary COSMOS time type and
 * provides conversion functions to/from UTC calendar time, GPS time, Unix
 * epoch, and TLE epoch. Also provides current-wall-clock functions. This is
 * the foundational time library; all other COSMOS time utilities depend on it.
 */

#ifndef _TIMELIB_H
#define _TIMELIB_H 1

#include "support/configCosmos.h"
//#include "math/mathlib.h"

#ifdef COSMOS_WIN_BUILD_MSVC
#include "support/timeutils.h"
#endif

#include <cmath>
#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <ctime>
#include <ratio>

//! \ingroup cosmos_timelib
//! \defgroup timelib_constants Time handling constants
namespace Cosmos {
    namespace Support {
        //! @{


#define MJD_UNIX_OFFSET 40587.
#define JD_MJD_OFFSET 2400000.5
#define MJD2JD(mjd) (double)((mjd) + JD_MJD_OFFSET)
#define JD2MJD(jd) (double)((jd) - JD_MJD_OFFSET)
#define DAY_TO_SECONDS 86400.
        //#define CURR_MJD current_mjd()
        //#define CURRENT_TIME_us CURR_MJD*DAY_TO_SECONDS*1000*1000
#define SECONDS_TO_DAYS(x)              ((double)(x) / DAY_TO_SECONDS)
#define DAYS_TO_SECONDS(x)              ((double)(x) * DAY_TO_SECONDS)
#define TIME_SECS_TO_DAYS(x)            ((double)(x) / DAY_TO_SECONDS)
#define TIME_DAYS_TO_SECS(x)            ((double)(x) * DAY_TO_SECONDS)
#define TIME_SECS_SINCE_MJD(x)          (  ( (currentmjd(0.)-(x)) ) * DAY_TO_SECONDS   )
#define TIME_UNIXs_TO_MJD(unixSecs)     (  ( (unixSecs) / DAY_TO_SECONDS ) + MJD_UNIX_OFFSET  )            // From http://stackoverflow.com/questions/466321/convert-unix-timestamp-to-julian
#define TIME_MJD_TO_UNIXs(mjd)          (((mjd) - MJD_UNIX_OFFSET) * DAY_TO_SECONDS)                       // Inverse operation of above
#define TIME_UNIX_TV_TO_DOUBLE_SECS(x)  ( ((double)(x.tv_sec)) + ((double)(x.tv_usec)  / 1000000. ) )
#define UPTIME (DAY_TO_SECONDS*(currentmjd(0.)-mjd_start_time))

        //! @}

        //! \ingroup cosmos_timelib
        //! \defgroup timelib_typedefs Time handling type definitions
        //! @{
        struct timestruc
        {
            double mjd;
            int32_t year;
            int32_t month;
            int32_t day;
            double fd;
            double tt_mjd;
            double gmst_rad;
        };

        struct calstruc
        {
            int32_t year;
            int32_t month = 1;
            int32_t dom = 1;
            int32_t doy = 1;
            int32_t hour = 0;
            int32_t minute = 0;
            int32_t second = 0;
            int32_t nsecond = 0;
        };

        class DateTime {

        public:
            double mjd; // represented in Modified Julian Date

            DateTime();
            DateTime(int year, int month, int day, int hour, int minute, double seconds);
            //    ~DateTime();

        };

        //! @}

        //! \ingroup cosmos_timelib
        //! \defgroup timelib_functions Time handling functions
        //! @{

        //! \brief Return the current wall-clock time as MJD (UTC).
        //! \return Current time in Modified Julian Date.
        double currentmjd();

        //! \brief Return the current wall-clock time as MJD with an added offset.
        //! \param offset Seconds to add to the current time.
        //! \return Current MJD + offset converted to days.
        double currentmjd(double offset);

        //! \brief Return the current Unix time in microseconds.
        //! \param offset Additional microseconds to add; default 0.
        //! \return Current Unix time in microseconds since the Unix epoch.
        uint64_t get_unix_time(uint64_t offset=0);

        //! \brief Return the current local time as a human-readable string.
        //! \return Local time string in the platform's default locale format.
        string get_local_time();

        //! \brief Compute elapsed seconds between \p mjd and now.
        //! \param mjd Reference time in MJD.
        //! \return Elapsed seconds (positive if \p mjd is in the past).
        double tmdiff(double mjd);

        //! \brief Convert a calstruc calendar date to MJD.
        //! \param date Populated calstruc.
        //! \return Corresponding MJD.
        double cal2mjd(calstruc date);

        //! \brief Convert a fractional Gregorian year to MJD.
        //! \param year Fractional year (e.g. 2000.5 = 1 July 2000).
        //! \return Corresponding MJD.
        double cal2mjd(double year);

        //! \brief Convert year + day-of-year to MJD.
        //! \param year      Gregorian year.
        //! \param dayOfYear Day of year (1-based, fractional); default 1.
        //! \return Corresponding MJD.
        double cal2mjd(int32_t year, double dayOfYear=1.);

        //! \brief Convert year, month, fractional day to MJD.
        //! \param year  Gregorian year.
        //! \param month Month (1–12); default 1.
        //! \param day   Day (1-based, fractional); default 1.
        //! \return Corresponding MJD.
        double cal2mjd(int32_t year, int32_t month=1, double day=1.);

        //! \brief Convert year, month, day, hour, minute, second, nanosecond to MJD.
        //! \param year    Gregorian year.
        //! \param month   Month (1–12); default 1.
        //! \param day     Day (1–31); default 1.
        //! \param hour    Hour (0–23); default 0.
        //! \param minute  Minute (0–59); default 0.
        //! \param second  Second (0–59); default 0.
        //! \param nsecond Nanosecond fraction; default 0.
        //! \return Corresponding MJD.
        double cal2mjd(int32_t year, int32_t month=1, int32_t day=1, int32_t hour=0, int32_t minute=0, int32_t second=0, int32_t nsecond=0);

        //! \brief Convert a Gregorian calendar date/time to MJD.
        //! \param year   Gregorian year.
        //! \param month  Month (1–12).
        //! \param day    Day (1–31).
        //! \param hour   Hour (0–23).
        //! \param minute Minute (0–59).
        //! \param second Fractional seconds.
        //! \return Corresponding MJD.
        double gregorianToModJulianDate(int32_t year, int32_t month, int32_t day,
                                        int32_t hour, int32_t minute, double second);

        //! \brief Convert MJD (UTC) to a POSIX timeval.
        //! \param utc MJD value; default 0 uses current time.
        //! \return Corresponding struct timeval.
        timeval utc2unix(double utc=0.);

        //! \brief Convert MJD (UTC) to a POSIX timeval (alias for utc2unix).
        timeval utc2timeval(double utc=0.);

        //! \brief Convert MJD (UTC) to a POSIX timespec.
        //! \param utc MJD value; default 0 uses current time.
        //! \return Corresponding struct timespec.
        timespec utc2timespec(double utc=0.);

        //! \brief Convert MJD (UTC) to Unix seconds (double).
        //! \param utc MJD value; default 0 uses current time.
        //! \return Fractional seconds since the Unix epoch.
        double utc2unixseconds(double utc=0.);

        //! \brief Convert MJD (UTC) to a Unix date string.
        //! \param utc MJD value; default 0 uses current time.
        //! \return Human-readable date string in Unix ctime format.
        string utc2unixdate(double utc=0.);

        //! \brief Convert MJD (UTC) to an ISO 8601 UTC string.
        //! \param mjd    MJD value; default 0 uses current time.
        //! \param offset Additional seconds to add before formatting; default 0.
        //! \return ISO 8601 string (e.g. "2024-01-15T12:34:56Z").
        string utc2iso8601(double mjd=0., double offset=0.);

        //! \brief Convert MJD (UTC) to a formatted date-time string.
        //! \param mjd   MJD value; default 0 uses current time.
        //! \param style Format style selector (0 = default); default 0.
        //! \return Formatted date-time string.
        string utc2datetime(double mjd=0., uint16_t style=0);

        //! \brief Parse an ISO 8601 date string to MJD.
        //! \param date ISO 8601 string.
        //! \return Corresponding MJD (UTC).
        double iso86012utc(string date);

        //! \brief Parse an ISO 8601 date string to MJD (alias for iso86012utc).
        double iso86012mjd(string date);

        //! \brief Convert GPS time (seconds since GPS epoch) to GPS week and week-seconds.
        //! \param gps     GPS time in seconds since GPS epoch.
        //! \param week    Output: GPS week number.
        //! \param seconds Output: seconds within the GPS week.
        void    gps2week(double gps, uint32_t& week, double& seconds);

        //! \brief Convert GPS week + week-seconds to GPS time in seconds.
        //! \param week    GPS week number.
        //! \param seconds Seconds within the week.
        //! \return GPS time in seconds since the GPS epoch.
        double  week2gps(uint32_t week, double seconds);

        //! \brief Convert MJD to fractional Gregorian year.
        //! \param mjd Input MJD.
        //! \return Fractional year.
        double mjd2year(double mjd);

        //! \brief Convert MJD to day-of-year (1-based fractional).
        //! \param mjd Input MJD.
        //! \return Fractional day of year.
        double mjd2doy(double mjd);

        //! \brief Convert MJD to a calstruc calendar structure.
        //! \param mjd        Input MJD.
        //! \param resolution Sub-second resolution digits; default 0.
        //! \return Populated calstruc.
        calstruc mjd2cal(double mjd, uint8_t resolution=0);

        //! \brief Convert MJD to year, month, and fractional day.
        //! \param mjd   Input MJD.
        //! \param year  Output Gregorian year.
        //! \param month Output month (1–12).
        //! \param day   Output fractional day.
        //! \return 0 on success.
        int32_t mjd2ymd(double mjd, int32_t &year, int32_t &month, double &day);

        //! \brief Convert MJD to year, month, day, and day-of-year.
        //! \param mjd   Input MJD.
        //! \param year  Output Gregorian year.
        //! \param month Output month.
        //! \param day   Output fractional day.
        //! \param doy   Output fractional day-of-year.
        //! \return 0 on success.
        int32_t mjd2ymd(double mjd, int32_t &year, int32_t &month, double &day, double &doy);

        //! \brief Convert MJD to a Gregorian date string.
        //! \param mjd Input MJD.
        //! \return Gregorian date string (e.g. "2024-01-15 12:34:56").
        string mjdToGregorian(double mjd);

        //! \brief Convert MJD to individual Gregorian date/time components.
        //! \param mjd    Input MJD.
        //! \param year   Output year.
        //! \param month  Output month.
        //! \param day    Output day.
        //! \param hour   Output hour.
        //! \param minute Output minute.
        //! \param second Output second.
        //! \return 0 on success.
        int32_t mjdToGregorian(double mjd, int32_t *year, int32_t *month, int32_t *day,
                               int32_t *hour, int32_t *minute, int32_t *second);

        //! \brief Convert MJD to a compact Gregorian format string.
        string mjdToGregorianFormat(double mjd);

        //! \brief Convert MJD to "DD MMM YYYY" date string (uppercase month).
        string  mjdToGregorianDDMMMYYYY(double mjd);

        //! \brief Convert MJD to "DD Mmm YYYY" date string (title-case month).
        string  mjdToGregorianDDMmmYYYY(double mjd);

        //! \brief Convert MJD to ISO 8601 UTC string.
        //! \param mjd    Input MJD; default 0 uses current time.
        //! \param offset Seconds to add before formatting; default 0.
        //! \return ISO 8601 string.
        string  mjd2iso8601(double mjd=0., double offset=0.);

        //! \brief Convert MJD to Julian Date.
        //! \param mjd Input MJD.
        //! \return Corresponding JD.
        double  mjd2jd(double mjd);

        //! \brief Convert Julian Date to MJD.
        //! \param jd Input Julian Date.
        //! \return Corresponding MJD.
        double  jd2mjd(double jd);

        //! \brief Format an MJD as a TLE epoch string ("YYDDD.dddddddd").
        //! \param mjd Input MJD.
        //! \return TLE epoch string.
        string mjd2tlef(double mjd);

        //! \brief Convert a POSIX timeval to MJD (UTC).
        //! \param unixtime POSIX timeval.
        //! \return Corresponding MJD.
        double unix2utc(struct timeval unixtime);

        //! \brief Convert Unix seconds (double) to MJD (UTC).
        //! \param unixtime Fractional seconds since the Unix epoch.
        //! \return Corresponding MJD.
        double unix2utc(double unixtime);

        //! \brief Convert Terrestrial Time (TT) MJD to Barycentric Dynamical Time (TDB) MJD.
        //! \param mjd Input MJD in TT.
        //! \return Corresponding MJD in TDB.
        double  tt2tdb(double mjd);

        //! \brief Compute the Julian century (T) from J2000.0 for a given MJD.
        //! \param mjd Input MJD.
        //! \return Julian centuries since J2000.0.
        double  julcen(double mjd);

        //! \brief Wrap an angle to the range [0, 2π).
        //! \param angle Input angle in radians.
        //! \return Angle wrapped to [0, 2π).
        double  ranrm(double angle);

        //! \brief Determine whether a Gregorian year is a leap year.
        //! \param year Gregorian year.
        //! \return 1 if leap year, 0 otherwise.
        int16_t isleap(int32_t year);

        //! \brief Format an elapsed-seconds count as "DD:HH:MM:SS".
        //! \param elapsed_seconds Total elapsed seconds.
        //! \return Formatted duration string.
        string  seconds2DDHHMMSS(double elapsed_seconds);

        //! \brief Set the local system clock to a given UTC (requires root).
        //! \param utc_to Target time in MJD.
        //! \param limit  Minimum time difference in seconds before applying; default 0 (always set).
        //! \return Actual time set, in MJD.
        double set_local_clock(double utc_to, float limit=0.);

        //! \brief Sleep for a given number of microseconds.
        //! \param usec Duration to sleep in microseconds.
        //! \return 0 on success, negative error code if interrupted.
        int32_t microsleep(uint64_t usec);

        //! \brief Sleep for a given number of seconds (fractional).
        //! \param seconds Duration to sleep in seconds.
        //! \return 0 on success, negative error code if interrupted.
        int32_t secondsleep(double seconds);

        //! \brief Return the MJD of the start of the Gregorian year containing \p mjd.
        //! \param mjd Reference MJD; default 0 uses current time.
        //! \return MJD of 00:00:00 on 1 January of the containing year.
        double newyear(double mjd=0);

        //! \brief Return the MJD of the start of the decade containing \p mjd.
        //! \param mjd Reference MJD; default 0 uses current time.
        //! \return MJD of 00:00:00 on 1 January of the containing decade.
        double newdecade(double mjd=0);

        //! \brief Return centisecond count from a reference MJD as a uint32.
        //! \param mjd Reference MJD; default 0 uses current time.
        //! \return Centiseconds since the reference epoch.
        uint32_t centisec(double mjd=0.);

        //! \brief Convert a centisecond count to MJD.
        //! \param centi Centisecond count.
        //! \return Corresponding MJD.
        double centisec2mjd(uint32_t centi);

        //! \brief Return decisecond count from a reference MJD as a uint32.
        //! \param mjd Reference MJD; default 0 uses current time.
        //! \return Deciseconds since the reference epoch.
        uint32_t decisec(double mjd=0.);

        //! \brief Convert a decisecond count to MJD.
        //! \param deci Decisecond count.
        //! \return Corresponding MJD.
        double decisec2mjd(uint32_t deci);

        //! \brief Format an MJD as a compact date-name string (e.g. "20240115T123456").
        //! \param mjd Input MJD.
        //! \return Compact date-name string.
        string to_datename(double mjd);

        //! \brief Format an MJD as an ISO 8601 UTC string (alias for mjd2iso8601).
        //! \param mjd Input MJD.
        //! \return ISO 8601 string.
        string to_iso8601(double mjd);

        //! @}
    }
}

#endif
