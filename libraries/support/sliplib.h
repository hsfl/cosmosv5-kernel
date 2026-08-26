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
 * \file sliplib.h
 * \brief SLIP serial line framing per RFC 1055.
 * \ingroup cosmos_kernel_network
 * \defgroup cosmos_sliplib SLIP
 *
 * Implements the Serial Line Internet Protocol (SLIP, RFC 1055) for framing
 * arbitrary byte payloads on a serial link. Provides encode (stuff) and decode
 * (unframe) functions. Used by the serial-port and radio communication drivers
 * that need packet boundaries on a byte-stream medium.
 */

#ifndef _SLIPLIB_H
#define _SLIPLIB_H

/*!	\file sliplib.h
	\brief COSMOS SLIP support include file
*/

//! \ingroup support
//! \defgroup sliplib COSMOS SLIP encoding library
//! A library providing support functions for the COSMOS specific SLIP protocol.


#include "support/configCosmosKernel.h"

//#include "support/cosmos-errno.h"
#include <cstring>

//! \ingroup cosmos_sliplib
//! \defgroup sliplib_constants COSMOS SLIP support constants
//! @{

// **************************************************************************
// SLIP Byte Definitions - Added 20140610 - To prevent errors with other SLIP Libs
// **************************************************************************
//! SLIP Buffer End character
#ifndef SLIP_FEND
    #define SLIP_FEND 0xC0
#endif
//! SLIP Buffer Escape character
#ifndef SLIP_FESC
    #define SLIP_FESC 0xDB
#endif
//! SLIP Buffer Escaped End character
#ifndef SLIP_TFEND
    #define SLIP_TFEND 0xDC
#endif
//! SLIP Buffer Escaped Escape character
#ifndef SLIP_TFESC
    #define SLIP_TFESC 0xDD
#endif

//! @}

//! \ingroup cosmos_sliplib
//! \defgroup sliplib_functions COSMOS SLIP support functions
//! @{

//! \brief Decode a SLIP-framed raw buffer into a payload buffer.
//! \param sbuf  Pointer to SLIP-encoded source data.
//! \param ssize Length of source data in bytes.
//! \param rbuf  Destination buffer for the decoded payload.
//! \param rsize Capacity of the destination buffer in bytes.
//! \return Number of payload bytes written, or a negative error code.
int32_t slip_unpack(const uint8_t *sbuf, uint16_t ssize, uint8_t *rbuf, uint16_t rsize);

//! \brief Decode a SLIP-framed raw buffer (alias for slip_unpack).
//! \param sbuf  Pointer to SLIP-encoded source data.
//! \param ssize Length of source data in bytes.
//! \param rbuf  Destination buffer for the decoded payload.
//! \param rsize Capacity of the destination buffer in bytes.
//! \return Number of payload bytes written, or a negative error code.
int32_t slip_decode(const uint8_t *sbuf, uint16_t ssize, uint8_t *rbuf, uint16_t rsize);

//! \brief Encode a payload into a SLIP-framed buffer.
//! \param rbuf  Pointer to raw payload data.
//! \param rsize Length of payload in bytes.
//! \param sbuf  Destination buffer for SLIP-encoded output.
//! \param ssize Capacity of the destination buffer in bytes.
//! \return Number of encoded bytes written, or a negative error code.
int32_t slip_pack(const uint8_t *rbuf, uint16_t rsize, uint8_t *sbuf, uint16_t ssize);

//! \brief Encode a payload into a SLIP-framed buffer (alias for slip_pack).
int32_t slip_encode(const uint8_t *rbuf, uint16_t rsize, uint8_t *sbuf, uint16_t ssize);

//! \brief Decode a SLIP-framed byte vector into a payload vector.
//! \param sbuf SLIP-encoded source vector.
//! \param rbuf Destination vector (resized to fit the payload).
//! \return Number of payload bytes, or a negative error code.
int32_t slip_unpack(vector<uint8_t> &sbuf, vector<uint8_t> &rbuf);

//! \brief Decode a SLIP-framed byte vector (alias for vector slip_unpack).
int32_t slip_decode(vector<uint8_t> &sbuf, vector<uint8_t> &rbuf);

//! \brief Encode a payload vector into a SLIP-framed vector.
//! \param rbuf Raw payload vector.
//! \param sbuf Destination vector for SLIP-encoded output.
//! \return Number of encoded bytes, or a negative error code.
int32_t slip_pack(vector<uint8_t> &rbuf, vector<uint8_t> &sbuf);

//! \brief Encode a payload vector into a SLIP-framed vector (alias for vector slip_pack).
int32_t slip_encode(vector<uint8_t> &rbuf, vector<uint8_t> &sbuf);

//! \brief Read and decode one SLIP packet from an open file stream.
//! \param fp  Open FILE pointer positioned at the start of a SLIP frame.
//! \param buf Output vector filled with the decoded payload bytes.
//! \return Number of payload bytes read, or a negative error code.
int32_t slip_extract(FILE *fp, vector<uint8_t> &buf);

//! \brief Compute the CRC-16 checksum of a raw byte buffer.
//! \param buf  Pointer to data.
//! \param size Number of bytes.
//! \return CRC-16 value.
uint16_t slip_calc_crc(uint8_t *buf, uint16_t size);

//! \brief Compute the CRC-16 checksum of a byte vector.
//! \param buf Input data vector.
//! \return CRC-16 value.
uint16_t slip_calc_crc(vector<uint8_t> &buf);


//! @}

#endif
