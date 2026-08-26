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

//! \ingroup devices
//! \defgroup serialclass Serial port device class library.

/*!
 * \file serialclass.h
 * \brief Cross-platform serial port with SLIP, NMEA, and XMODEM support.
 * \ingroup cosmos_kernel_device
 * \defgroup cosmos_serialclass Serial
 *
 * Implements a full-featured serial port class covering port open/close, baud
 * rate and parity configuration, raw and line-mode I/O, and framing helpers
 * for SLIP, NMEA sentences, and XMODEM file transfer. Works on Linux, macOS,
 * and Windows. The preferred high-level serial API; use instead of the
 * deprecated cssl_lib.
 */

#ifndef SERIALCLASS_H
#define SERIALCLASS_H

#include "support/configCosmos.h"
//#include "support/cosmos-errno.h"
#include "support/sliplib.h"
#include "support/elapsedtime.h"
#if defined(COSMOS_LINUX_OS) || defined(COSMOS_CYGWIN_OS) || defined(COSMOS_MAC_OS)
#include <termios.h>
#include <sys/select.h>
#endif


namespace Cosmos {
    class Serial
    {
    public:
        //! \brief Open and configure a serial port.
        //! \param dname   Device path (e.g. "/dev/ttyUSB0" or "COM3").
        //! \param dbaud   Baud rate; default 9600.
        //! \param dbits   Data bits (5–8); default 8.
        //! \param dparity Parity: "none", "odd", or "even"; default "none".
        //! \param dstop   Stop bits (1 or 2); default 1.
        Serial(string dname, size_t dbaud=9600, size_t dbits=8, std::string dparity="none", size_t dstop=1);

        //! \brief Close the serial port and optionally restore original settings.
        ~Serial();

        //! \brief Open the serial device and apply the stored configuration.
        //! \return 0 on success, negative error code on failure.
        int32_t open_device();

        //! \brief Close the serial device.
        //! \return 0 on success, negative error code on failure.
        int32_t close_device();

        //! \brief Control whether the original port settings are restored on close.
        //! \param argument True = restore; false = leave as-is.
        //! \return 0 on success.
        int32_t set_restoreonclose(bool argument);

        //! \brief Reconfigure baud rate, data bits, parity, and stop bits on the open port.
        //! \param dbaud   Baud rate.
        //! \param dbits   Data bits.
        //! \param dparity Parity (0=none, 1=odd, 2=even).
        //! \param dstop   Stop bits.
        //! \return 0 on success, negative error code on failure.
        int32_t set_params(size_t dbaud, size_t dbits, size_t dparity, size_t dstop);

        //! \brief Change only the baud rate without altering other settings.
        //! \param dbaud New baud rate.
        //! \return 0 on success, negative error code on failure.
        int32_t change_baud(size_t dbaud);

        //! \brief Set hardware (RTS/CTS) and software (XON/XOFF) flow control.
        //! \param rtscts  True to enable RTS/CTS hardware flow control.
        //! \param xonxoff True to enable XON/XOFF software flow control.
        //! \return 0 on success, negative error code on failure.
        int32_t set_flowcontrol(bool rtscts, bool xonxoff);

#if defined(COSMOS_LINUX_OS) || defined(COSMOS_CYGWIN_OS) || defined(COSMOS_MAC_OS)
        //! \brief Set POSIX read timeout and minimum character count.
        //! \param minchar Minimum bytes before a read returns (VMIN).
        //! \param timeout Read timeout in seconds (VTIME, rounded to 0.1 s).
        //! \return 0 on success, negative error code on failure.
        int32_t set_timeout(int minchar, double timeout);
#else
        int32_t set_timeout(int, double timeout);
#endif

        //! \brief Set a symmetric read/write timeout in seconds.
        //! \param timeout Timeout in seconds.
        //! \return 0 on success.
        int32_t set_timeout(double timeout);

        //! \brief Set the write (inter-character) timeout in seconds.
        int32_t set_wtimeout(double timeout);

        //! \brief Set the read timeout in seconds.
        int32_t set_rtimeout(double timeout);

        //! \brief Set the DTR (Data Terminal Ready) signal state.
        //! \param state True = assert DTR; false = deassert.
        //! \return 0 on success, negative error code on failure.
        int32_t set_dtr(bool state);

        //! \brief Set the RTS (Request To Send) signal state.
        //! \param state True = assert RTS; false = deassert.
        //! \return 0 on success, negative error code on failure.
        int32_t set_rts(bool state);

        //! \brief Return the current CTS (Clear To Send) signal state.
        //! \return True if CTS is asserted.
        bool get_cts();

        //! \brief Write a single byte to the port.
        //! \param c Byte to send.
        //! \return 1 on success, negative error code on failure.
        int32_t put_char(uint8_t c);

        //! \brief Write a string to the port.
        //! \param data String to send.
        //! \return Bytes written, or negative error code.
        int32_t put_string(string data);

        //! \brief Write a byte vector to the port.
        int32_t put_data(vector <uint8_t> data);

        //! \brief Write a raw byte buffer to the port.
        //! \param data Pointer to data.
        //! \param size Number of bytes.
        //! \return Bytes written, or negative error code.
        int32_t put_data(const uint8_t *data, size_t size);

        //! \brief SLIP-encode and write a byte vector to the port.
        int32_t put_slip(const vector<uint8_t>& data);

        //! \brief SLIP-encode and write a raw buffer to the port.
        int32_t put_slip(const uint8_t *data, size_t size);

        //! \brief Write an NMEA sentence (appends checksum and CRLF) to the port.
        int32_t put_nmea(vector <uint8_t> data);

        //! \brief Wait for all output bytes to be transmitted.
        //! \return 0 on success, negative error code on failure.
        int32_t drain();

        //! \brief Poll for a single available byte without blocking.
        //! \return 1 if a byte is available, 0 if not, negative error code on failure.
        int32_t poll_char();

        //! \brief Read one byte (blocks until available or timeout).
        //! \return Byte value (0–255) on success, negative error code on failure.
        int32_t get_char();

        //! \brief Read one byte into \p buffer.
        //! \param buffer Receives the byte.
        //! \return 1 on success, negative error code on failure.
        int32_t get_char(uint8_t &buffer);

        //! \brief Read up to \p size bytes into a vector.
        //! \param data   Receives the bytes.
        //! \param size   Maximum bytes to read; default SIZE_MAX (read all available).
        //! \param append If true, append to existing vector; otherwise replace; default false.
        //! \return Bytes read, or negative error code.
        int32_t get_data(vector <uint8_t> &data, size_t size=SIZE_MAX, bool append=false);

        //! \brief Read up to \p size bytes into a string.
        int32_t get_string(string &data, size_t size=SIZE_MAX);

        //! \brief Read bytes into a string until the end character \p endc is received.
        //! \param data Receives the bytes (including \p endc).
        //! \param endc Terminator character; 0 = read until timeout; default 0.
        //! \return Bytes read, or negative error code.
        int32_t get_string(string &data, char endc=0);

        //! \brief Read exactly \p size bytes into a raw buffer.
        //! \param data  Destination buffer.
        //! \param size  Number of bytes to read.
        //! \return Bytes read, or negative error code.
        int32_t get_data(uint8_t *data, size_t size);

        //! \brief Read one SLIP frame into a vector.
        //! \param data  Receives the decoded payload.
        //! \param size  Maximum payload size; default SIZE_MAX.
        //! \return Payload bytes, or negative error code.
        int32_t get_slip(vector <uint8_t> &data, size_t size=SIZE_MAX);

        //! \brief Read one SLIP frame into a raw buffer.
        int32_t get_slip(uint8_t *data, size_t size=0);

        //! \brief Read one NMEA sentence (terminated by LF) into a vector.
        int32_t get_nmea(vector <uint8_t> &data, size_t size);

        //! \brief Read one XMODEM block into a vector.
        int32_t get_xmodem(vector <uint8_t> &data, size_t size);

        //! \brief Return the last error code set by a serial operation.
        int32_t get_error();

        //! \brief Return true if the port is currently open.
        bool get_open() const;

        //! \brief Send a single byte (legacy alias for put_char).
        int32_t SendByte(uint8_t byte);

        //! \brief Receive up to \p size bytes into \p buf (legacy alias for get_data).
        int32_t ReceiveBuffer(uint8_t *buf, int size);

        //! \brief Receive a single byte into \p buf (legacy alias for get_char).
        int32_t ReceiveByte(uint8_t &buf);

        //! \brief Send \p size bytes from \p buffer (legacy alias for put_data).
        int32_t SendBuffer(uint8_t *buffer, int size);

        map<string, size_t> Parity = {{"none", 0}, {"odd", 1}, {"even", 2}};

    private:
        int fd = -1;                   /* tty file descriptor */
        int32_t error;
        vector <uint32_t> baud_speed[2] = {
            {0, 50, 75, 110, 134, 150, 200, 300, 500, 1200, 1800, 2400, 4800, 9600, 19200, 38400},
            {57600, 115200, 230400, 460800, 500000, 576000, 921600, 1000000, 1152000, 1500000, 2000000, 2500000, 3000000, 3500000, 4000000}
        };
        // timeout in sec 2 ms
        double wictimeout = .002;
        double rictimeout = .05;
        bool restoreonclose = true;

#if defined(COSMOS_LINUX_OS) || defined(COSMOS_CYGWIN_OS) || defined(COSMOS_MAC_OS)
        struct termios tio{};       /* termios structure for the port */
        struct termios oldtio{};    /* old termios structure */
#else // windows
        DCB dcb; // port settings
        HANDLE handle;
#endif

        string name;
        size_t baud;
        size_t bits;
        size_t parity;
        size_t stop;

#define XMODEM_SOH 0x01
#define XMODEM_EOT 0x04
#define XMODEM_ACK 0x06
#define XMODEM_NAK 0x15
#define XMODEM_CAN 0x18

    };


} // end of namespace Cosmos
#endif // SERIALCLASS_H
