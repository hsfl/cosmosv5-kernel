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

/*! \file stringlib.h
	\brief stringlib include file
	A library providing functions for handling various types of string.
*/

//! \ingroup support
//!	\defgroup stringlib String handling library

/*!
 * \file stringlib.h
 * \brief String manipulation, number formatting, and conversion helpers.
 * \ingroup cosmos_kernel_json
 * \defgroup cosmos_stringlib String Utilities
 *
 * Collects miscellaneous string utilities: split/join, trim, case conversion,
 * integer/float-to-string and string-to-number with error checking, and
 * various COSMOS-specific string conventions. Used by the JSON, time, and
 * namespace libraries.
 */

#ifndef _STRINGLIB_H
#define _STRINGLIB_H 1

#include "support/configCosmos.h"
//#include "support/jsonobject.h"

#include <errno.h>
#include <string>
#include <sstream>

//! \ingroup cosmos_stringlib
//! \defgroup stringlib_functions String handling functions
//! @{

//! \brief Split a string into tokens on any character in \p delimeters.
//! \param in         Input string.
//! \param delimeters Set of delimiter characters; default " \t,".
//! \param multi      If true, treat consecutive delimiters as one; default true.
//! \return Vector of token strings (delimiters not included).
vector < string > string_split(string in, string delimeters=" \t,", bool multi=true);

//! \brief Find the \p num-th occurrence of any character in \p chars within \p input.
//! \param input Input string.
//! \param chars Set of characters to search for.
//! \param num   Which occurrence to find (1-based); default 1.
//! \return Position of the match, or string::npos if not found.
size_t string_find(string input, string chars, uint16_t num=1);

//! \brief Find the \p num-th occurrence of any character in \p chars across a vector of strings.
//! \param input Vector of strings searched in order.
//! \param chars Set of characters to search for.
//! \param num   Which occurrence to find (1-based); default 1.
//! \return Absolute character position across the joined vector, or string::npos.
size_t string_find(vector<string> input, string chars, uint16_t num=1);

//! \brief Join a vector of strings with \p delimeter.
//! \param in        Input vector.
//! \param delimeter Separator placed between elements; default " ".
//! \param first     Index of the first element to include; default 0.
//! \param last      Index of the last element to include (inclusive); default 65535 (all).
//! \return Joined string.
string string_join(vector<string>& in, string delimeter=" ", uint16_t first=0, uint16_t last=65535);

//! \brief Tokenise a C string into an array of word pointers (strtok-style).
//! \param string Input C string (modified in place).
//! \param word   Output array of pointers to each token.
//! \param size   Maximum number of tokens to extract.
//! \return Number of tokens found.
uint16_t string_parse(char *string, char *word[], uint16_t size);

//! \brief Replace all occurrences of \p from with \p to within \p str.
//! \param str  Input string.
//! \param from Substring to find.
//! \param to   Replacement substring.
//! \return Modified string.
string string_replace(string str, const std::string &from, const std::string &to);

//! \brief Wildcard string comparison (* matches any sequence, ? matches one char).
//! \param wild   Pattern string containing * and/or ? wildcards.
//! \param string String to test.
//! \return 0 if \p string matches \p wild, non-zero otherwise.
int string_cmp(const char *wild, const char *string);

//! \brief Convert a byte vector to a hexadecimal (and optionally ASCII) string.
//! \param buffer Input byte vector.
//! \param ascii  If true, append ASCII representation alongside hex; default false.
//! \param start  Starting byte index; default 0.
//! \return Hex (and optionally ASCII) string representation.
string to_hex_string(const vector<uint8_t> &buffer, bool ascii=false, uint16_t start=0);

//! \brief Convert a raw byte buffer to a hexadecimal (and optionally ASCII) string.
//! \param buffer Pointer to the source buffer.
//! \param size   Number of bytes to convert.
//! \param ascii  If true, append ASCII alongside hex; default false.
//! \param start  Starting byte index; default 0.
//! \return Hex string representation.
string to_hex_string(uint8_t* buffer, uint16_t size, bool ascii=false, uint16_t start=0);

//! \brief Parse a hex string (e.g. "0a1b2c") into a byte vector.
//! \param hex Input hex string (two characters per byte, no prefix).
//! \return Decoded byte vector.
vector<uint8_t> from_hex_string(std::string hex);

//! \brief Decode a byte vector of ASCII hex characters into a binary byte vector.
//! \param hex Input byte vector where each pair of bytes represents one hex digit pair.
//! \return Decoded binary byte vector.
vector<uint8_t> from_hex_vector(vector<uint8_t>& hex);
#if ((SIZE_WIDTH) == (UINT64_WIDTH))
//! \brief Parse a hex string to a native unsigned integer.
//! \param value Hex string (e.g. "1a2b").
//! \return Parsed unsigned integer value.
uint64_t from_hex(string value);
#else
//! \brief Parse a hex string to a native unsigned integer.
size_t from_hex(string value);
#endif

//! \brief Convert a single ASCII hex character ('0'–'9', 'a'–'f', 'A'–'F') to its nibble value.
//! \param value Hex character.
//! \return Nibble value (0–15).
uint8_t from_hex(char value);

//! \brief Convert a byte vector to a printable ASCII string, using hex escapes for non-printable bytes.
//! \param buf Input byte vector.
//! \param hex If true, show non-printable bytes as hex; default true.
//! \return ASCII-safe string representation.
string to_astring(vector<uint8_t> buf, bool hex=true);

//! \brief Convert a string to a printable ASCII string, using hex escapes for non-printable bytes.
//! \param buf   Input string.
//! \param hex   If true, show non-printable bytes as hex; default false.
//! \return ASCII-safe string representation.
string to_astring(string buf, bool hex=false);

//! \brief Convert a C char array to a printable ASCII string.
//! \param value  Pointer to char data.
//! \param length Number of characters.
//! \param hex    If true, show non-printable bytes as hex; default false.
//! \return ASCII-safe string representation.
string to_astring(char *value, size_t length, bool hex=false);

//! \brief Reinterpret a byte vector as an ASCII string (all bytes treated as characters).
//! \param buf    Input byte vector.
//! \param offset Starting byte index; default 0.
//! \return String built from the byte values.
string byte_vector_to_string(const vector<uint8_t>& buf, uint16_t offset=0);

//! \brief Convert an ASCII string to a byte vector (each character becomes one byte).
//! \param buf Input string.
//! \return Byte vector.
vector<uint8_t> string_to_byte_vector(const string &buf);

#if ((SIZE_WIDTH) == (UINT64_WIDTH))
//! \brief Format an unsigned integer as a binary string.
//! \param value    Value to format.
//! \param digits   Minimum field width; default 0 (no padding).
//! \param zerofill If true, left-pad with '0'; default false.
//! \return Binary string (e.g. "101010").
string to_binary(uint64_t value, uint16_t digits=0, bool zerofill=false);

//! \brief Format an unsigned integer as a hexadecimal string.
//! \param value    Value to format.
//! \param digits   Minimum field width; default 0.
//! \param zerofill If true, left-pad with '0'; default false.
//! \return Hex string (e.g. "1a2b").
string to_hex(uint64_t value, uint16_t digits=0, bool zerofill=false);

//! \brief Format an unsigned integer as a decimal string.
//! \param value    Value to format.
//! \param digits   Minimum field width; default 0.
//! \param zerofill If true, left-pad with '0'; default false.
//! \return Decimal string.
string to_unsigned(uint64_t value, uint16_t digits=0, bool zerofill=false);
#else
string to_binary(size_t value, uint16_t digits=0, bool zerofill=false);
string to_hex(size_t value, uint16_t digits=0, bool zerofill=false);
string to_unsigned(size_t value, uint16_t digits=0, bool zerofill=false);
#endif

#if ((PTRDIFF_WIDTH) == (INT64_WIDTH))
//! \brief Format a signed integer as a decimal string.
//! \param value    Value to format.
//! \param digits   Minimum field width; default 0.
//! \param zerofill If true, left-pad with '0'; default false.
//! \return Signed decimal string.
string to_signed(int64_t value, uint16_t digits=0, bool zerofill=false);
#else
string to_signed(ptrdiff_t value, uint16_t digits=0, bool zerofill=false);
#endif

//! \brief Format a double as a fixed-point decimal string.
//! \param value     Value to format.
//! \param digits    Total field width.
//! \param precision Digits after the decimal point; default 0.
//! \param zerofill  If true, left-pad integer part with '0'; default false.
//! \return Fixed-point decimal string.
string to_fixed(double value, uint16_t digits, uint16_t precision=0, bool zerofill=false);

//! \brief Format a float as a floating-point string.
//! \param value     Value to format.
//! \param precision Significant digits; -1 means use default; default -1.
//! \return Floating-point string.
string to_floating(float value, int16_t precision=-1);

//! \brief Format a double as a floating-point string.
//! \param value     Value to format.
//! \param precision Significant digits; -1 means use default; default -1.
//! \return Floating-point string.
string to_floating(double value, int16_t precision=-1);

//! \brief Format a float in scientific notation.
string to_floatexp(float value, int16_t precision=-1);

//! \brief Format a double in scientific notation.
string to_floatexp(double value, int16_t precision=-1);

//! \brief Format a float using the shorter of fixed or scientific notation.
string to_floatany(float value, int16_t precision=-1);

//! \brief Format a double using the shorter of fixed or scientific notation.
string to_floatany(double value, int16_t precision=-1);

//! \brief Format an MJD value as a human-readable date-time string.
//! \param value MJD to format.
//! \return Date-time string.
string to_mjd(double value);

//! \brief Format a temperature value with units.
//! \param value     Temperature in the given units.
//! \param units     Unit character: 'K' = Kelvin, 'C' = Celsius, 'F' = Fahrenheit; default 'K'.
//! \param precision Decimal places; default 4.
//! \return Formatted temperature string (e.g. "298.1500K").
string to_temperature(double value, char units='K', uint8_t precision=4);

//! \brief Format an angle value with units.
//! \param value     Angle in radians (converted to \p units for display).
//! \param units     Unit character: 'R' = radians, 'D' = degrees; default 'R'.
//! \param precision Decimal places; default 4.
//! \return Formatted angle string.
string to_angle(double value, char units='R', uint8_t precision=4);

//! \brief Format a boolean value as a string.
//! \param value Input boolean.
//! \param type  Format style: 'Y' = "Yes"/"No", 'T' = "True"/"False", '1' = "1"/"0"; default 'Y'.
//! \return Formatted boolean string.
string to_bool(bool value, char type='Y');

//! \brief Format an MJD as a Unix time string.
//! \param mjd       Input MJD.
//! \param precision Decimal places for seconds; default 0.
//! \return Unix time string representation.
string to_unixtime(double mjd, uint8_t precision=0);

//! \brief Format a JSON key-value pair where the value is a string.
//! \param key   JSON key.
//! \param value String value.
//! \return JSON fragment "\"key\":\"value\"".
string to_json(string key, string value);

//! \brief Format a JSON key-value pair where the value is a double.
string to_json(string key, double value);
#if ((PTRDIFF_WIDTH) == (INT64_WIDTH))
//! \brief Format a JSON key-value pair where the value is an int64.
string to_json(string key, int64_t value);
#else
string to_json(string key, ptrdiff_t value);
#endif
//! \brief Format a JSON key-value pair where the value is an int32.
string to_json(string key, int32_t value);
//! \brief Format a JSON key-value pair where the value is an int16.
string to_json(string key, int16_t value);
//! \brief Format a JSON key-value pair where the value is an int8.
string to_json(string key, int8_t value);
#if ((SIZE_WIDTH) == (UINT64_WIDTH))
//! \brief Format a JSON key-value pair where the value is a uint64.
string to_json(string key, uint64_t value);
#else
string to_json(string key, size_t value);
#endif
//! \brief Format a JSON key-value pair where the value is a uint32.
string to_json(string key, uint32_t value);
//! \brief Format a JSON key-value pair where the value is a uint16.
string to_json(string key, uint16_t value);
//! \brief Format a JSON key-value pair where the value is a uint8.
string to_json(string key, uint8_t value);

//! \brief Format a diagnostic label-value pair (label: value).
//! \param label Label string.
//! \param value String value.
//! \return Formatted "label: value" string.
string to_label(string label, string value);

//! \brief Format a diagnostic label-value pair for a double.
//! \param label     Label string.
//! \param value     Double value.
//! \param precision Decimal places; default 0 (default precision).
//! \param mjd       If true, also interpret \p value as MJD and append date; default false.
//! \return Formatted label-value string.
string to_label(string label, double value, uint16_t precision=0, bool mjd=false);

//! \brief Format a diagnostic label-value pair for a float.
string to_label(string label, float value, uint16_t precision=0, bool mjd=false);
#if ((PTRDIFF_WIDTH) == (INT64_WIDTH))
//! \brief Format a diagnostic label-value pair for an int64.
string to_label(string label, int64_t value, uint16_t digits=0, bool hex=false);
#endif
//! \brief Format a diagnostic label-value pair for an int32.
string to_label(string label, int32_t value, uint16_t digits=0, bool hex=false);
//! \brief Format a diagnostic label-value pair for an int16.
string to_label(string label, int16_t value, uint16_t digits=0, bool hex=false);
//! \brief Format a diagnostic label-value pair for an int8.
string to_label(string label, int8_t value, uint16_t digits=0, bool hex=false);
#if ((SIZE_WIDTH) == (UINT64_WIDTH))
//! \brief Format a diagnostic label-value pair for a uint64.
string to_label(string label, uint64_t value, uint16_t digits=0, bool hex=false);
#endif
//! \brief Format a diagnostic label-value pair for a uint32.
string to_label(string label, uint32_t value, uint16_t digits=0, bool hex=false);
//! \brief Format a diagnostic label-value pair for a uint16.
string to_label(string label, uint16_t value, uint16_t digits=0, bool hex=false);
//! \brief Format a diagnostic label-value pair for a uint8.
string to_label(string label, uint8_t value, uint16_t digits=0, bool hex=false);
//! \brief Format a diagnostic label-value pair for a bool.
string to_label(string label, bool value);

//! \brief Strip leading/trailing whitespace and non-printable characters from a string.
//! \param value Input string.
//! \return Cleaned string.
string clean_string(string value);

//! \brief Parse a decimal string to uint64.
//! \param svalue Decimal string.
//! \return Parsed value.
uint64_t to_uint64(string svalue);
//! \brief Parse a decimal C-string to uint64, using only the first \p digits characters.
uint64_t to_uint64(const char* svalue, uint16_t digits=0);

//! \brief Parse a decimal string to uint32.
uint32_t to_uint32(string svalue);
//! \brief Parse a decimal C-string to uint32.
uint32_t to_uint32(const char* svalue, uint16_t digits=0);

//! \brief Parse a decimal string to uint16.
uint16_t to_uint16(string svalue);
//! \brief Parse a decimal C-string to uint16.
uint16_t to_uint16(const char* svalue, uint16_t digits=0);

//! \brief Parse a decimal string to uint8.
uint8_t to_uint8(string svalue);
//! \brief Parse a decimal C-string to uint8.
uint8_t to_uint8(const char* svalue, uint16_t digits=0);

//! \brief Parse a decimal string to int64.
int64_t to_int64(string svalue);
//! \brief Parse a decimal C-string to int64.
int64_t to_int64(const char* svalue, uint16_t digits=0);

//! \brief Parse a decimal string to int32.
int32_t to_int32(string svalue);
//! \brief Parse a decimal C-string to int32.
int32_t to_int32(const char* svalue, uint16_t digits=0);

//! \brief Parse a decimal string to int16.
int16_t to_int16(string svalue);
//! \brief Parse a decimal C-string to int16.
int16_t to_int16(const char* svalue, uint16_t digits=0);

//! \brief Parse a decimal string to int8.
int8_t to_int8(string svalue);
//! \brief Parse a decimal C-string to int8.
int8_t to_int8(const char* svalue, uint16_t digits=0);

//! \brief Parse a floating-point string to double.
//! \param svalue Decimal or scientific-notation string.
//! \return Parsed double value.
double_t to_double(string svalue);
//! \brief Parse a floating-point C-string to double.
double_t to_double(const char* svalue, uint16_t digits=0);

//! \brief Parse a floating-point string to float.
float_t to_float(string svalue);
//! \brief Parse a floating-point C-string to float.
float_t to_float(const char* svalue, uint16_t digits=0);

// Class to parse a comma delimited string
class StringParser {

    vector<string> vect;

public:
    // the offset allows you to move the index by an offset value
	// this can be useful if, for example, getFieldNumber(1) should logically be getFieldNumber(2)
    // the offset then is 1
    int offset; ///< Index offset applied to all getFieldNumber calls.

    //! \brief Construct a parser splitting on commas.
    //! \param str Input string.
    StringParser(string str);

    //! \brief Construct a parser splitting on a custom delimiter.
    //! \param str       Input string.
    //! \param delimiter Field separator character.
    StringParser(string str, char delimiter);

    //! \brief Return the field at the given (offset-adjusted) index as a string.
    //! \param index Field index (0-based before offset adjustment).
    //! \return Field string, or empty string if out of range.
    string getFieldNumber(uint32_t index);

    //! \brief Return the field at the given index parsed as a double.
    //! \param index Field index.
    //! \return Parsed double value.
    double getFieldNumberAsDouble(uint32_t index);

    //! \brief (Re-)split \p str on \p delimiter and repopulate the field vector.
    //! \param str       String to split.
    //! \param delimiter Field separator character.
    void splitString(string str, char delimiter);

    size_t numberOfFields; ///< Number of fields found after splitting.

    //! \brief Return the field at the given index parsed as an int.
    //! \param index Field index.
    //! \return Parsed integer value.
    int getFieldNumberAsInteger(uint32_t index);
};

//! @}

#endif
