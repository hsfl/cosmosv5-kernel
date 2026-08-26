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
 * \file bytelib.h
 * \brief Byte-level manipulation and endian conversion utilities.
 * \ingroup cosmos_kernel_math
 * \defgroup cosmos_bytelib Byte Utilities
 *
 * Provides functions for reading and writing multi-byte integers in both
 * little-endian and big-endian byte order, along with general byte-buffer
 * manipulation helpers. Use when interfacing with hardware registers or
 * network protocols that specify a particular byte order.
 */

#ifndef _BYTELIB_H
#define _BYTELIB_H

#include <vector>
#include "math/constants.h"

//! \brief Detect the byte order of the running platform.
//! \return ByteOrder::LITTLEENDIAN or ByteOrder::BIGENDIAN.
ByteOrder local_byte_order();

#if ((SIZE_WIDTH) == (UINT64_WIDTH))
//! \brief Swap the byte order of an unsigned integer to the requested order.
//! \param bits  Width of the value in bits (8, 16, 32, or 64).
//! \param value Value to swap.
//! \param order Target byte order; default LITTLEENDIAN.
//! \return Value with bytes reordered as requested.
uint64_t uintswap(uint8_t bits, uint64_t value, ByteOrder order=ByteOrder::LITTLEENDIAN);
#else
//! \brief Swap the byte order of an unsigned integer to the requested order.
//! \param bits  Width of the value in bits (8, 16, 32, or 64).
//! \param value Value to swap.
//! \param order Target byte order; default LITTLEENDIAN.
//! \return Value with bytes reordered as requested.
size_t uintswap(uint8_t bits, size_t value, ByteOrder order=ByteOrder::LITTLEENDIAN);
#endif

//! \brief Read a uint8 from a byte buffer.
//! \param pointer Source byte buffer.
//! \param order   Byte order of the source data; default LITTLEENDIAN.
//! \return Interpreted uint8_t value.
uint8_t uint8from(const uint8_t *pointer, ByteOrder order=ByteOrder::LITTLEENDIAN);

//! \brief Read a uint8 from a byte buffer (alias for uint8from).
//! \param pointer Source byte buffer.
//! \param order   Byte order of the source data; default LITTLEENDIAN.
//! \return Interpreted uint8_t value.
uint8_t uint8to(const uint8_t *pointer, ByteOrder order=ByteOrder::LITTLEENDIAN);

//! \brief Copy and reorder a byte vector according to \p order.
//! \param src   Source byte vector.
//! \param dst   Destination byte vector (resized to match src).
//! \param order Target byte order; default LITTLEENDIAN.
void uint8from(const vector<uint8_t> &src, vector<uint8_t> &dst, ByteOrder order=ByteOrder::LITTLEENDIAN);

//! \brief Copy and reorder a byte vector according to \p order (output by value).
//! \param src   Source byte vector.
//! \param dst   Destination byte vector (resized to match src).
//! \param order Target byte order; default LITTLEENDIAN.
void uint8to(const vector<uint8_t> &src, vector<uint8_t> dst, ByteOrder order=ByteOrder::LITTLEENDIAN);

//! \brief Deserialise a uint16 from a raw byte buffer.
//! \param pointer Pointer to at least 2 bytes of source data.
//! \param order   Byte order of the source data; default LITTLEENDIAN.
//! \return Interpreted uint16_t value.
uint16_t uint16from(uint8_t *pointer, ByteOrder order=ByteOrder::LITTLEENDIAN);

//! \brief Byte-swap a uint16 value to the requested order.
//! \param value  Input uint16 value.
//! \param order  Target byte order; default LITTLEENDIAN.
//! \return Byte-swapped uint16_t value.
uint16_t uint16from(const uint16_t &value, ByteOrder order=ByteOrder::LITTLEENDIAN);

//! \brief Deserialise an int16 from a raw byte buffer.
//! \param pointer Pointer to at least 2 bytes of source data.
//! \param order   Byte order of the source data; default LITTLEENDIAN.
//! \return Interpreted int16_t value.
int16_t int16from(uint8_t *pointer, ByteOrder order=ByteOrder::LITTLEENDIAN);

//! \brief Deserialise a uint32 from a raw byte buffer.
//! \param pointer Pointer to at least 4 bytes of source data.
//! \param order   Byte order of the source data; default LITTLEENDIAN.
//! \return Interpreted uint32_t value.
uint32_t uint32from(const uint8_t *pointer, ByteOrder order=ByteOrder::LITTLEENDIAN);

//! \brief Deserialise an int32 from a raw byte buffer.
//! \param pointer Pointer to at least 4 bytes of source data.
//! \param order   Byte order of the source data; default LITTLEENDIAN.
//! \return Interpreted int32_t value.
int32_t int32from(uint8_t *pointer, ByteOrder order=ByteOrder::LITTLEENDIAN);

//! \brief Deserialise a float from a raw byte buffer.
//! \param pointer Pointer to at least 4 bytes of source data.
//! \param order   Byte order of the source data; default LITTLEENDIAN.
//! \return Interpreted float value.
float floatfrom(uint8_t *pointer, ByteOrder order=ByteOrder::LITTLEENDIAN);

//! \brief Deserialise a double from a raw byte buffer.
//! \param pointer Pointer to at least 8 bytes of source data.
//! \param order   Byte order of the source data; default LITTLEENDIAN.
//! \return Interpreted double value.
double doublefrom(uint8_t *pointer, ByteOrder order=ByteOrder::LITTLEENDIAN);

//! \brief Serialise a uint32 into a raw byte buffer.
//! \param value   Value to write.
//! \param pointer Destination buffer (at least 4 bytes).
//! \param order   Target byte order; default LITTLEENDIAN.
void uint32to(uint32_t value, uint8_t *pointer, ByteOrder order=ByteOrder::LITTLEENDIAN);

//! \brief Serialise an int32 into a raw byte buffer.
//! \param value   Value to write.
//! \param pointer Destination buffer (at least 4 bytes).
//! \param order   Target byte order; default LITTLEENDIAN.
void int32to(int32_t value, uint8_t *pointer, ByteOrder order=ByteOrder::LITTLEENDIAN);

//! \brief Serialise a uint16 into a raw byte buffer.
//! \param value   Value to write.
//! \param pointer Destination buffer (at least 2 bytes).
//! \param order   Target byte order; default LITTLEENDIAN.
void uint16to(uint16_t value, uint8_t *pointer, ByteOrder order=ByteOrder::LITTLEENDIAN);

//! \brief Serialise an int16 into a raw byte buffer.
//! \param value   Value to write.
//! \param pointer Destination buffer (at least 2 bytes).
//! \param order   Target byte order; default LITTLEENDIAN.
void int16to(int16_t value, uint8_t *pointer, ByteOrder order=ByteOrder::LITTLEENDIAN);

//! \brief Serialise a float into a raw byte buffer.
//! \param value   Value to write.
//! \param pointer Destination buffer (at least 4 bytes).
//! \param order   Target byte order; default LITTLEENDIAN.
void floatto(float value, uint8_t *pointer, ByteOrder order=ByteOrder::LITTLEENDIAN);

//! \brief Serialise a double into a raw byte buffer.
//! \param value   Value to write.
//! \param pointer Destination buffer (at least 8 bytes).
//! \param order   Target byte order; default LITTLEENDIAN.
void doubleto(double value, uint8_t *pointer, ByteOrder order=ByteOrder::LITTLEENDIAN);

//! \brief Invert (bit-flip) every byte in a buffer and return as a new vector.
//! \param data  Pointer to the source byte array.
//! \param count Number of bytes to invert.
//! \return New vector containing the bitwise-inverted bytes.
vector<uint8_t> vector8invert(uint8_t* data, uint32_t count);


#endif
