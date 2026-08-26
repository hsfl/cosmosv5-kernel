/*!
 * \file crclib.h
 * \brief CRC-16 and CRC-32 computation routines.
 * \ingroup cosmos_kernel_math
 * \defgroup cosmos_crclib CRC
 *
 * Implements standard cyclic redundancy check algorithms (CRC-16 and CRC-32)
 * for data integrity verification. Use to append or validate checksums on
 * serial packets, file payloads, or any binary data stream.
 */

#ifndef _CRCLIB_H
#define _CRCLIB_H

#include <cstdint>
#include <map>
#include <fstream>
using std::ifstream;
#include <string>
using std::string;
#include <vector>
using std::vector;

#include "support/cosmos-errno.h"
#include "math/bytelib.h"


//! CRC-16-CCITT Normal
#define CRC16CCITT 0x1021
#define CRC16CCITTMSB 0x1021
#define CRC16CCITTMSBINIT 0xffff
//! CRC-16-CCITT Reversed
#define CRC16CCITTR 0x8408
#define CRC16CCITTLSB 0x8408
#define CRC16CCITTLSBINIT 0x0000
//! CRC-16-CCITT Reversed Reciprocal
#define CRC16CCITTRR 0x8810

//! CRC-16-IBM Normal
#define CRC16IBM 0x8005
#define CRC16IBMMSB 0x8005
#define CRC16IBMMSBINIT 0xffff
//! CRC-16-IBM Reversed
#define CRC16IBMR 0xa001
#define CRC16IBMLSB 0xa001
#define CRC16IBMLSBINIT 0x0000
//! CRC-16-IBM Reversed Reciprocal
#define CRC16IBMRR 0xc002

//! \brief Compute CRC-16-CCITT over a raw byte buffer.
//! \param buf  Pointer to the data.
//! \param size Number of bytes.
//! \param lsb  If true, use LSB-first (reversed) variant; default true.
//! \return 16-bit CRC.
uint16_t calc_crc16ccitt(uint8_t *buf, int size, bool lsb=true);

//! \brief Compute a configurable CRC-16 over a raw byte buffer.
//! \param buf      Pointer to the data.
//! \param size     Number of bytes.
//! \param poly     Generator polynomial; default CRC16CCITTMSB.
//! \param crc      Initial CRC value; default CRC16CCITTMSBINIT.
//! \param xorout   Final XOR mask; default 0.
//! \param lsbfirst If true, process bits LSB first; default false.
//! \return 16-bit CRC.
uint16_t calc_crc16(uint8_t *buf, uint16_t size, uint16_t poly=CRC16CCITTMSB, uint16_t crc=CRC16CCITTMSBINIT, uint16_t xorout=0x0, bool lsbfirst=false);

//! \brief Compute a configurable CRC-16 over a byte vector.
//! \param buf      Input byte vector.
//! \param poly     Generator polynomial; default CRC16CCITTMSB.
//! \param crc      Initial CRC value; default CRC16CCITTMSBINIT.
//! \param xorout   Final XOR mask; default 0.
//! \param lsbfirst If true, process bits LSB first; default false.
//! \return 16-bit CRC.
uint16_t calc_crc16(vector<uint8_t> buf, uint16_t poly=CRC16CCITTMSB, uint16_t crc=CRC16CCITTMSBINIT, uint16_t xorout=0x0, bool lsbfirst=false);

//! \brief CRC-16-CCITT LSB-first over a string.
//! \param buf        Input string.
//! \param initialcrc Starting CRC; default CRC16CCITTLSBINIT.
//! \param skip       Number of leading bytes to skip; default 0.
//! \return 16-bit CRC.
uint16_t calc_crc16ccitt_lsb(string buf, uint16_t initialcrc=CRC16CCITTLSBINIT, uint16_t skip=0);

//! \brief CRC-16-CCITT LSB-first over a byte vector.
//! \param buf        Input byte vector.
//! \param initialcrc Starting CRC; default CRC16CCITTLSBINIT.
//! \param skip       Number of leading bytes to skip; default 0.
//! \return 16-bit CRC.
uint16_t calc_crc16ccitt_lsb(vector<uint8_t> &buf, uint16_t initialcrc=CRC16CCITTLSBINIT, uint16_t skip=0);

//! \brief CRC-16-CCITT LSB-first over a raw byte buffer.
//! \param buf        Pointer to the data.
//! \param size       Number of bytes.
//! \param initialcrc Starting CRC; default CRC16CCITTLSBINIT.
//! \return 16-bit CRC.
uint16_t calc_crc16ccitt_lsb(uint8_t *buf, uint16_t size, uint16_t initialcrc=CRC16CCITTLSBINIT);

//! \brief Generic CRC-16 LSB-first over a raw byte buffer.
//! \param buf    Pointer to the data.
//! \param size   Number of bytes.
//! \param poly   Generator polynomial; default CRC16CCITTLSB.
//! \param crc    Initial CRC; default CRC16CCITTLSBINIT.
//! \param xorout Final XOR mask; default 0.
//! \return 16-bit CRC.
uint16_t calc_crc16_lsb(uint8_t *buf, uint16_t size, uint16_t poly=CRC16CCITTLSB, uint16_t crc=CRC16CCITTLSBINIT, uint16_t xorout=0x0);

//! \brief Generic CRC-16 LSB-first over a byte vector.
//! \param buf    Input byte vector.
//! \param poly   Generator polynomial; default CRC16CCITTLSB.
//! \param crc    Initial CRC; default CRC16CCITTLSBINIT.
//! \param xorout Final XOR mask; default 0.
//! \param skip   Number of leading bytes to skip; default 0.
//! \return 16-bit CRC.
uint16_t calc_crc16_lsb(vector<uint8_t> &buf, uint16_t poly=CRC16CCITTLSB, uint16_t crc=CRC16CCITTLSBINIT, uint16_t xorout=0x0, uint16_t skip=0);

//! \brief CRC-16-CCITT MSB-first over a string.
//! \param buf        Input string.
//! \param initialcrc Starting CRC; default CRC16CCITTMSBINIT.
//! \param skip       Number of leading bytes to skip; default 0.
//! \return 16-bit CRC.
uint16_t calc_crc16ccitt_msb(string buf, uint16_t initialcrc=CRC16CCITTMSBINIT, uint16_t skip=0);

//! \brief CRC-16-CCITT MSB-first over a byte vector.
//! \param buf        Input byte vector.
//! \param initialcrc Starting CRC; default CRC16CCITTMSBINIT.
//! \param skip       Number of leading bytes to skip; default 0.
//! \return 16-bit CRC.
uint16_t calc_crc16ccitt_msb(vector<uint8_t> &buf, uint16_t initialcrc=CRC16CCITTMSBINIT, uint16_t skip=0);

//! \brief CRC-16-CCITT MSB-first over a raw byte buffer.
//! \param buf        Pointer to the data.
//! \param size       Number of bytes.
//! \param initialcrc Starting CRC; default CRC16CCITTMSBINIT.
//! \return 16-bit CRC.
uint16_t calc_crc16ccitt_msb(uint8_t *buf, uint16_t size, uint16_t initialcrc=CRC16CCITTMSBINIT);

//! \brief Generic CRC-16 MSB-first over a raw byte buffer.
//! \param buf    Pointer to the data.
//! \param size   Number of bytes.
//! \param poly   Generator polynomial; default CRC16CCITTMSB.
//! \param crc    Initial CRC; default CRC16CCITTMSBINIT.
//! \param xorout Final XOR mask; default 0.
//! \return 16-bit CRC.
uint16_t calc_crc16_msb(uint8_t *buf, uint16_t size, uint16_t poly=CRC16CCITTMSB, uint16_t crc=CRC16CCITTMSBINIT, uint16_t xorout=0x0);

//! \brief Generic CRC-16 MSB-first over a byte vector.
//! \param buf    Input byte vector.
//! \param poly   Generator polynomial; default CRC16CCITTMSB.
//! \param crc    Initial CRC; default CRC16CCITTMSBINIT.
//! \param xorout Final XOR mask; default 0.
//! \param skip   Number of leading bytes to skip; default 0.
//! \return 16-bit CRC.
uint16_t calc_crc16_msb(vector<uint8_t> &buf, uint16_t poly=CRC16CCITTMSB, uint16_t crc=CRC16CCITTMSBINIT, uint16_t xorout=0x0, uint16_t skip=0);

//! \brief Compute CRC-16-IBM over a raw byte buffer.
//! \param buf  Pointer to the data.
//! \param size Number of bytes.
//! \param lsb  If true, use LSB-first variant; default true.
//! \return 16-bit CRC.
uint16_t calc_crc16ibm(uint8_t *buf, int size, bool lsb=true);

class CRC16
{

public:
    uint16_t lookup[256];
    struct crcset
    {
        bool lsbfirst;
        uint16_t polynomial;
        uint16_t initialcrc;
        uint16_t xorout;
        uint16_t test;
    };

    std::map<string, crcset> types;
//    CRC16(uint16_t polynomial=0x1021, uint16_t initial=0xffff, uint16_t xorout=0x0, bool lsbfirst=false);
    //! \brief Construct a CRC16 object with the default CCITT-false parameters.
    CRC16();
    //! \brief Select a named CRC-16 variant (e.g. "ccitt-false", "ibm").
    //! \param type Name of the CRC variant to configure.
    //! \return The initial CRC value for the selected type.
    uint16_t set(string type);
    //! \brief Configure the CRC-16 parameters explicitly.
    //! \param polynomial Generator polynomial; default 0x1021.
    //! \param initialcrc Starting CRC value; default 0xffff.
    //! \param xorout     Final XOR mask; default 0.
    //! \param lsbfirst   If true, process bits LSB first; default false.
    //! \return The configured initial CRC value.
    uint16_t set(uint16_t polynomial=0x1021, uint16_t initialcrc=0xffff, uint16_t xorout=0x0, bool lsbfirst=false);
    //! \brief Compute CRC-16 over a byte vector.
    //! \param message Input data.
    //! \return 16-bit CRC.
    uint16_t calc(vector<uint8_t> message);
    //! \brief Compute CRC-16 over the first \p size bytes of a byte vector.
    //! \param message Input data.
    //! \param size    Number of bytes to process.
    //! \return 16-bit CRC.
    uint16_t calc(vector<uint8_t> message, uint16_t size);
    //! \brief Compute CRC-16 over the first \p size characters of a string.
    //! \param message Input string.
    //! \param size    Number of characters to process.
    //! \return 16-bit CRC.
    uint16_t calc(string message, uint16_t size);
    //! \brief Compute CRC-16 over an entire string.
    //! \param message Input string.
    //! \return 16-bit CRC.
    uint16_t calc(string message);
    //! \brief Compute CRC-16 over a raw byte buffer.
    //! \param message Pointer to the data.
    //! \param size    Number of bytes.
    //! \return 16-bit CRC.
    uint16_t calc(uint8_t *message, uint16_t size);
    //! \brief Compute CRC-16 over the contents of a file.
    //! \param file_path Path to the file.
    //! \return 16-bit CRC, or a negative COSMOS error code on failure.
    int32_t calc_file(string file_path);

    uint16_t test;

private:
    string type = "ccitt-false";
    uint16_t initial;
    uint16_t polynomial;
    uint16_t xorout;
    bool lsbfirst = true;
};

#endif
