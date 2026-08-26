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

//TODO: consider renaming this class to Timer or StopWatch

/*!
 * \file elapsedtime.h
 * \brief High-resolution elapsed time measurement.
 * \ingroup cosmos_kernel_time
 * \defgroup cosmos_elapsedtime Elapsed Time
 *
 * Provides the ElapsedTime class for measuring wall-clock durations with
 * sub-millisecond resolution using the platform monotonic clock. Use for
 * profiling, timeout enforcement, and rate-limiting within agents and drivers.
 */

#ifndef ELAPSED_TIME
#define ELAPSED_TIME

#include "support/configCosmos.h"
#include <iostream>
#include <string>
#include <ctime>

#ifdef CROSS_TYPE_arm
#include <sys/time.h>
#else
#include <chrono>
#endif

/*! \file elapsedtime.h
*	\brief Elapsed %Time header file
*/

//! \ingroup support
//! \defgroup elapsedtime Elapsed Time
//! %Elapsed Time.
//! The functions in this library support the measurement of various types of elapsed time. Suppoert is provided
//! for both absolute elapsed time (ElapsedTime::split()) and relative elapsed time (ElapsedTime::lap()).

//! \ingroup cosmos_elapsedtime
//! \defgroup elapsedtime_functions Elapsed Time function declarations
//! @{

// profiling class
// On windows using MinGw32 it does not get better than 1ms
class ElapsedTime {
    //new for c++11
#ifdef CROSS_TYPE_arm
    //	struct timespec timeStart, timeNow, timeCheck;
    timeval timeStart, timeStop, timeNow, timeCheck, timeAlarm;
#else
    std::chrono::steady_clock::time_point timeStart, timeStop, timeNow, timeCheck, timeAlarm; //
#endif

public:
    //! \brief Construct and start the timer.
    //! \param initialseconds Seconds to pre-load as already-elapsed; default 0.
    ElapsedTime(double initialseconds=0.);

    //! \brief Print elapsed time since start to stdout.
    void printElapsedTime();
    //! \brief Print a label followed by elapsed time since start to stdout.
    //! \param text Label to prepend.
    void printElapsedTime(string text);

    //! \brief Elapsed time in milliseconds since start.
    //! \return Milliseconds elapsed.
    double getElapsedTimeMiliSeconds();

    //! \brief Elapsed time in seconds since start.
    //! \return Seconds elapsed.
    double getElapsedTime();

    //! \brief Elapsed time in seconds since a given MJD start time.
    //! \param startTimeMjd Reference start time in MJD.
    //! \return Seconds between \p startTimeMjd and now.
    double getElapsedTimeSince(double startTimeMjd);

    //! \brief Elapsed time in seconds between two MJD values.
    //! \param startMjd Start MJD.
    //! \param endMjd   End MJD.
    //! \return Seconds between the two MJD values.
    double getElapsedTime(double startMjd, double endMjd);

    //! \brief Return elapsed time since the last lap() call, then reset the lap mark.
    //! \return Seconds since the previous lap.
    double lap();

    //! \brief Return total elapsed time since start without resetting.
    //! \return Seconds since start.
    double split();

    //! \brief Return elapsed time since the last check() call without resetting start.
    //! \return Seconds since the previous check.
    double check();

    //! \brief Reset the timer to zero (or a given pre-load).
    void reset();

    //! \brief Start (or restart) the timer.
    //! \param initialseconds Seconds to pre-load; default 0.
    void start(double initialseconds=0.);

    //! \brief Stop the timer and return elapsed seconds.
    //! \return Seconds elapsed since start.
    double stop();

    //! \brief Begin a tic/toc interval (stores the current time).
    void tic();

    //! \brief End a tic/toc interval and return elapsed seconds.
    //! \return Seconds since tic().
    double toc();

    //! \brief End a tic/toc interval, print a label with elapsed time, and return elapsed seconds.
    //! \param text Label to print alongside the elapsed time.
    //! \return Seconds since tic().
    double toc(string text);

    //! \brief Configure a countdown timer for \p seconds.
    //! \param seconds Total countdown duration; default 0.
    //! \return Seconds remaining (equal to \p seconds immediately after set).
    double set(double seconds=0.);

    //! \brief Return seconds remaining on the countdown timer.
    //! \return Remaining seconds; 0 if the timer has expired.
    double timer();

    //! \brief Print a summary of the timer state to stdout.
    void info();

    bool print = true;       ///< If true, toc() and related functions print to stdout.
    double elapsedTime = 0.; ///< Accumulated elapsed time in seconds.
    double remainingTime = 0.; ///< Remaining countdown time in seconds.


};



//class ElapsedTimeOld {
//    // old plain c
//    struct timeval time1, time2;

//    //IN UNIX the use of the timezone struct is obsolete;
//    //See http://linux.about.com/od/commands/l/blcmdl2_gettime.htm
//    struct timezone x;
//    float timeDiff;


//public:
//    int timeval_subtract (struct timeval* result, struct timeval* x, struct timeval* y);
//    float elapsed_time(struct timeval a,struct timeval b);
//};
//! @}

//! \brief Block for \p seconds, printing a countdown message at each \p step interval.
//! \param seconds Total wait time in seconds.
//! \param step    Reporting interval in seconds; default 10.
//! \param message Optional label printed with each countdown update; default "".
//! \return 0 on completion.
int32_t timed_countdown(int32_t seconds, int32_t step=10, string message="");

#endif
