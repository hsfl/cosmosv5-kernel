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


/*!
 * \file print_utils.h
 * \brief Formatted console and debug output utilities.
 * \ingroup cosmos_kernel_config
 * \defgroup cosmos_print_utils Print Utilities
 *
 * Provides helper functions for colour-coded, timestamped, and
 * severity-labelled output to stdout/stderr. Use during development and agent
 * operation to emit structured diagnostic messages without pulling in a
 * heavyweight logging framework.
 */

#ifndef _COSMOS_PRINT_UTILS
#define _COSMOS_PRINT_UTILS

// COSMOS Library for Printing Vector and stuff like that to the stream

// v1.0 : 2014-10-05
// library created

//---------------------------------------------------------------
// Print class
#include "support/configCosmos.h"
#include "math/mathlib.h"

#include <iostream>     // std::cout
#include <string>       // string
#include <sstream>      // std::ostringstream

// TODO: remove the dependency of mathlib later, change rvector to vector?

//! \ingroup support
//! \defgroup printutils Output Utilities
//! Output Utilities.
//!
//! Various utilities to support output of the numerous COSMOS data types.

//! \ingroup cosmos_print_utils
//! \defgroup printutils_functions Output Utilties functions
//! @{

class PrintUtils
{

private:

public:

    //! \brief Construct a PrintUtils object with default settings.
    PrintUtils();

    //! \brief Print an rvector with prefix, scale, suffix, precision, and field width.
    //! \param prefix     Label printed before the vector.
    //! \param v          Vector to print.
    //! \param scale      Scale factor applied to each component before printing.
    //! \param suffix     Label printed after the vector.
    //! \param precision  Decimal places for each component.
    //! \param fieldwidth Minimum field width per component.
    void vector(string prefix, rvector v, double scale, string suffix, int precision, int fieldwidth);

    //! \brief Print an rvector with no labels or scale.
    void vector(rvector v);
    //! \brief Print an rvector with a prefix label.
    void vector(string prefix, rvector v);
    //! \brief Print an rvector with prefix and suffix labels.
    void vector(string vector_name, rvector v, string suffix);
    //! \brief Print an rvector with prefix, suffix, and precision.
    void vector(string prefix, rvector v, string suffix, int precision);

    //! \brief Print an rvector and its magnitude with a label.
    void vectorAndMag(string vector_name, rvector v);
    //! \brief Print an rvector and its magnitude with label and suffix.
    void vectorAndMag(string vector_name, rvector v, string suffix);

    //! \brief Print a scaled rvector with units and precision, followed by a newline.
    //! \param v         Vector to print.
    //! \param factor    Scale factor applied to each component.
    //! \param units     Units string appended after the vector.
    //! \param precision Decimal places.
    void vector_endl(rvector v,
                     double factor,
                     string units,
                     int precision);

    //! \brief Print two rvectors side by side with labels.
    void vector2(string name_v1, rvector v1, string name_v2, rvector v2);
    //! \brief Print two rvectors side by side with labels and precision.
    void vector2(string name_v1, rvector v1, string name_v2, rvector v2, int precision);

    //! \brief Print a newline character to stdout.
    void endline();

    //! \brief Format a quaternion as a string with full formatting options.
    //! \param prefix     Label before the quaternion.
    //! \param q          Quaternion to format.
    //! \param suffix     Label after the quaternion.
    //! \param precision  Decimal places per component.
    //! \param fieldwidth Minimum field width per component.
    //! \return Formatted string.
    string quat(string prefix, quaternion q, string suffix, int precision, int fieldwidth);
    //! \brief Format a quaternion as a compact string.
    string quat(quaternion q);
    //! \brief Print a quaternion with given precision.
    void quat(quaternion q, int precision);
    //! \brief Print a quaternion with a prefix label.
    void quat(string prefix, quaternion q);
    //! \brief Print a quaternion with prefix and precision.
    void quat(string prefix, quaternion q, int precision);
    //! \brief Print a quaternion with prefix and suffix.
    void quat(string prefix, quaternion q, string suffix);
    //! \brief Print a quaternion with prefix, suffix, and precision.
    void quat(string prefix, quaternion q, string suffix, int precision);

    bool printOn;        ///< Global print-enable flag.
    int precision;       ///< Default decimal places.
    int fieldwidth;      ///< Default field width per component.
    double scale;        ///< Default scale factor.
    string prefix;       ///< Default prefix label.
    string suffix;       ///< Default suffix label.
    string delimiter;    ///< Component delimiter string.
    bool delimiter_flag; ///< If true, print \c delimiter between components.
    bool use_brackets;   ///< If true, enclose vectors in square brackets.
    string fullMessage;  ///< Accumulated output string for batch formatting.

    //! \brief Reset all formatting options to their default values.
    void reset();

    //! \brief Print a plain text string to stdout.
    //! \param text String to print.
    void text(string text);

    //! \brief Print a scalar with prefix and suffix.
    void scalar(string prefix, double s, string suffix);
    //! \brief Print a scalar with full formatting options.
    //! \param prefix     Label before the value.
    //! \param s          Scalar value.
    //! \param scale      Scale factor applied before printing.
    //! \param suffix     Label after the value.
    //! \param precision  Decimal places.
    //! \param fieldwidth Minimum field width.
    void scalar(string prefix, double s, double scale, string suffix, int precision, int fieldwidth);
    //! \brief Print a scalar with a suffix only.
    void scalar(double s, string suffix);
    //! \brief Print a scalar with a prefix only.
    void scalar(string prefix, double s);
    //! \brief Print a scalar with no labels.
    void scalar(double s);

};

//! @}


#endif //_COSMOS_PRINT_UTILS

