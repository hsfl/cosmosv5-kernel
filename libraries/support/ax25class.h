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
 * \file ax25class.h
 * \brief AX.25 packet framing for amateur radio links.
 * \ingroup cosmos_kernel_network
 * \defgroup cosmos_ax25 AX.25
 *
 * Implements the AX.25 data-link protocol (Level 2) for building and parsing
 * UI frames used over amateur radio links. Handles address encoding, flag
 * stuffing, and FCS computation. Required by the KISS and TNC device drivers.
 */

#ifndef AX25LIB
#define AX25LIB

#include "support/configCosmosKernel.h"
//#include "support/cosmos-errno.h"
//#include "math/bytelib.h"
#include "math/crclib.h"

#include <cstring>
#include <iostream>

#define PACKETMAX 1024


namespace Cosmos {
    namespace Support {
        class Ax25Handle
        {

        public:
            struct packet_header
            {
                uint8_t destination_callsign[6];
                uint8_t destination_stationID;
                uint8_t source_callsign[6];
                uint8_t source_stationID;
                uint8_t control = 0x03;
                uint8_t protocolID = 0xf0;
            };

            struct packet_content
            {
                packet_header header;
                vector <uint8_t> data;
            };

            //! \brief Construct with explicit AX.25 header parameters.
            //! \param dest_call  Destination callsign (up to 6 characters).
            //! \param sour_call  Source callsign (up to 6 characters).
            //! \param dest_stat  Destination SSID status byte.
            //! \param sour_stat  Source SSID status byte.
            //! \param cont       Control field byte (e.g. 0x03 for UI frame).
            //! \param prot       Protocol ID byte (e.g. 0xf0 for no layer 3).
            Ax25Handle(string dest_call, string sour_call, uint8_t dest_stat, uint8_t sour_stat, uint8_t cont, uint8_t prot);

            //! \brief Construct with default (empty) header fields.
            Ax25Handle();

            //! \brief Set the destination callsign.
            void set_destination_callsign(string destination);
            //! \brief Get the destination callsign.
            string get_destination_callsign();
            //! \brief Set the destination SSID byte.
            void set_destination_stationID(uint8_t ID);
            //! \brief Get the destination SSID byte.
            uint8_t get_destination_stationID();
            //! \brief Set the source callsign.
            void set_source_callsign(string source);
            //! \brief Get the source callsign.
            string get_source_callsign();
            //! \brief Set the source SSID byte.
            void set_source_stationID(uint8_t ID);
            //! \brief Get the source SSID byte.
            uint8_t get_source_stationID();
            //! \brief Set the AX.25 control field byte.
            void set_control(uint8_t control_number);
            //! \brief Get the AX.25 control field byte.
            uint8_t get_control();
            //! \brief Set the protocol ID byte.
            void set_protocolID(uint8_t protocol);
            //! \brief Get the protocol ID byte.
            uint8_t get_protocolID();
            //! \brief Return a copy of the current packet header.
            packet_header get_header();
            //! \brief Return the current payload data vector.
            vector <uint8_t> get_data();
            //! \brief Return the stored AX.25-framed packet.
            vector <uint8_t> get_ax25_packet();
            //! \brief Return the stored HDLC-framed (bit-stuffed) packet.
            vector <uint8_t> get_hdlc_packet();
            //! \brief Set the payload data.
            //! \param input Payload bytes.
            //! \return 0 on success, negative error code on failure.
            int32_t set_data(vector <uint8_t> input);
            //! \brief Load and parse a raw AX.25 packet into header and data.
            //! \param packet Raw AX.25 packet bytes.
            //! \return 0 on success, negative error code on failure.
            int32_t set_ax25_packet(vector <uint8_t> packet);
            //! \brief Load and parse an HDLC-framed packet into the AX.25 packet.
            //! \param packet HDLC-framed packet bytes.
            //! \return 0 on success, negative error code on failure.
            int32_t set_hdlc_packet(vector <uint8_t> packet);
            //! \brief Deserialise \c ax25_packet into header and data, verifying CRC.
            //! \param checkcrc If true, verify CRC before accepting; default true.
            //! \return 0 on success, negative error code on failure.
            int32_t unload(bool checkcrc=true);
            //! \brief Serialise header and data into \c ax25_packet (with CRC).
            //! \param data Optional payload to set before loading; default empty (use existing).
            //! \return 0 on success, negative error code on failure.
            int32_t load(vector<uint8_t> data={});
            //! \brief HDLC-stuff an AX.25 packet into \c hdlc_packet.
            //! \param ax25data  AX.25 packet to stuff; default empty (use \c ax25_packet).
            //! \param flagcount Number of 0x7e flag bytes to prepend/append; default 2.
            //! \param flag      Flag byte value; default 0x7e.
            //! \return 0 on success, negative error code on failure.
            int32_t stuff(vector<uint8_t> ax25data={}, uint8_t flagcount=2, uint8_t flag=0x7e);
            //! \brief HDLC-unstuff an HDLC frame into \c ax25_packet.
            //! \param hdlcdata HDLC frame to unstuff; default empty (use \c hdlc_packet).
            //! \param flag     Flag byte value; default 0x7e.
            //! \return 0 on success, negative error code on failure.
            int32_t unstuff(vector<uint8_t> hdlcdata={}, uint8_t flag=0x7e);
            vector<uint8_t> hdlc_packet;
            vector<uint8_t> ax25_packet;

        private:
            packet_header header;
            uint16_t crc;
            uint16_t crccalc;
            vector <uint8_t> data;
            int32_t error;
            //    vector<uint8_t> flags = {0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e};
            vector<uint8_t> flags = {0x7e, 0x7e};
            CRC16 calc_crc;


            friend ::std::ostream& operator<<(::std::ostream& out, Ax25Handle& K);
        };
    }
}

//Functions to print packets
//! \brief Print a packet's bytes as ASCII (non-printable bytes shown as '.').
//! \param packet Pointer to the packet data.
//! \param count  Number of bytes to print.
void print_ascii(unsigned char* packet, unsigned int count);

//! \brief Print a packet's bytes as space-separated hex values.
//! \param packet Pointer to the packet data.
//! \param count  Number of bytes to print.
void print_hex(unsigned char* packet, unsigned int count);

#endif
