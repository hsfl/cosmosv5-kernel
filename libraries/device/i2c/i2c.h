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
//! \defgroup i2c I2C bus device class library.

/*!
 * \file i2c.h
 * \brief Linux I2C and SMBus device communication.
 * \ingroup cosmos_kernel_device
 * \defgroup cosmos_i2c I2C
 *
 * Provides a thin C++ wrapper around the Linux i2c-dev kernel interface for
 * reading and writing I2C/SMBus registers. Supports byte, word, and block
 * transfers. Use as the low-level transport for sensor and peripheral drivers
 * that communicate over I2C on Linux embedded platforms.
 */

#ifndef COSMOS_DEVICE_I2C_CLASS_H
#define COSMOS_DEVICE_I2C_CLASS_H

#include "support/cosmos-errno.h"
#include "support/timelib.h"
#include "support/elapsedtime.h"
#include <semaphore.h>
//#include <linux/i2c-dev.h> /* for I2C_SLAVE */
#if !defined(COSMOS_WIN_OS)
#include "device/i2c/i2c-dev-smbus.h"
#endif

using std::cout;

namespace Cosmos {
    class I2C
    {
    public:
        //! \brief Open an I2C bus and bind to a device address.
        //! \param bus     I2C bus device path (e.g. "/dev/i2c-1").
        //! \param address 7-bit or 10-bit I2C device address.
        //! \param delay   Minimum inter-transaction delay in seconds; default 200 µs.
        //! \param probe   If true, verify the device responds during construction; default false.
        I2C(string bus, uint8_t address, double delay=2e-4, bool probe=false);

        //! \brief Close the I2C bus file descriptor.
        ~I2C();

        //! \brief Query the adapter's supported functionality flags.
        //! \return Linux I2C_FUNCS bitmask, or negative error code.
        int32_t get_funcs();

        //! \brief Change the target device address on the open bus.
        //! \param address New device address.
        //! \return 0 on success, negative error code on failure.
        int32_t set_address(uint64_t address);

        //! \brief Set the minimum inter-transaction delay.
        //! \param seconds Delay in seconds.
        //! \return 0 on success.
        int32_t set_delay(double seconds);

        //! \brief (Re-)open and configure the I2C bus.
        //! \return 0 on success, negative error code on failure.
        int32_t connect();

        //! \brief Acquire the bus mutex with a timeout.
        //! \param seconds Maximum seconds to wait for the lock.
        //! \return 0 on success, negative error code on timeout.
        int32_t lock(float seconds);

        //! \brief Release the bus mutex.
        //! \return 0 on success.
        int32_t unlock();

        //! \brief Send a command string and read a response string.
        //! \param data     Command to send.
        //! \param response Receives the response.
        //! \param bytes    Number of bytes to read; 0 = read until timeout; default 0.
        //! \return Bytes read on success, negative error code on failure.
        int32_t communicate(string data, string &response, size_t bytes=0);

        //! \brief Send a command vector and read a response vector.
        int32_t communicate(vector <uint8_t> data, vector <uint8_t> &response, size_t bytes=0);

        //! \brief Send a raw command buffer and read into a raw response buffer.
        //! \param data     Pointer to command bytes.
        //! \param len      Number of command bytes to send.
        //! \param response Pointer to response buffer.
        //! \param bytes    Number of bytes to read.
        //! \return Bytes read on success, negative error code on failure.
        int32_t communicate(uint8_t *data, size_t len, uint8_t *response, size_t bytes=0);

        //! \brief Write a string to the device.
        //! \param data String to write.
        //! \return Bytes written, or negative error code.
        int32_t send(string data);

        //! \brief Write a raw byte buffer to the device.
        int32_t send(uint8_t *data, size_t len);

        //! \brief Write a byte vector to the device.
        int32_t send(vector <uint8_t> data);

        //! \brief Read a fixed number of bytes into a string.
        //! \param data  Receives the read data.
        //! \param bytes Number of bytes to read.
        //! \return Bytes read, or negative error code.
        int32_t receive(string &data, size_t bytes);

        //! \brief Read a fixed number of bytes into a raw buffer.
        int32_t receive(uint8_t *data, size_t bytes);

        //! \brief Read a fixed number of bytes into a vector.
        int32_t receive(vector <uint8_t> &data, size_t bytes);

        //! \brief Poll the device until a marker byte is received or timeout expires.
        //! \param data     Pointer to receive buffer.
        //! \param len      Buffer capacity.
        //! \param markchar Byte value that signals end of response; default 0xff.
        //! \param timeout  Maximum seconds to wait; 0 = no timeout; default 0.
        //! \return Bytes received, or negative error code.
        int32_t poll(uint8_t *data, size_t len, uint8_t markchar=0xff, double timeout=0.);

        //! \brief Return the last error code set by an I2C operation.
        int32_t get_error();

        //! \brief Return the open file descriptor for the I2C bus.
        int32_t get_fh();

        //! \brief Return true if the bus is currently open and connected.
        bool get_connected();


    private:
        struct
        {
            bool connected = false;
            bool probe = true;
            string bus;
            uint8_t address;
            int fh = -1;
            uint64_t funcs;
            double delay = 1e-4;
        } handle;

        sem_t* i2csem=nullptr;
        double i2cwait = 5.;
        timespec absi2cwait = {0, 0};
        // mutex mtx;
        int32_t error;

    };


} // end of namespace Cosmos
#endif // COSMOS_DEVICE_I2C_CLASS_H
