//! \ingroup devices
//! \defgroup devicedisk Disk device information and usage library.

/*!
 * \file devicedisk.h
 * \brief Disk space and filesystem information utilities.
 * \ingroup cosmos_kernel_device
 * \defgroup cosmos_devicedisk Device Disk
 *
 * Queries available disk space, total capacity, and inode counts for a given
 * filesystem path. Used by agents to monitor storage resources and enforce
 * disk-usage limits on log and data directories.
 */

#ifndef DEVICEDISK_H
#define DEVICEDISK_H

#include "support/configCosmos.h"
//#include "support/jsonlib.h"
//#include "support/datalib.h"
#include "support/stringlib.h"

#include <cstdint> // uint64_t
#ifndef COSMOS_MAC_OS
#if defined (COSMOS_WIN_OS)
#include "windows.h"
#include <tchar.h>
#else
#include <stdlib.h>
#include <sys/statvfs.h>
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#endif
#endif

#if defined(COSMOS_MAC_OS)
    #include <sys/disk.h>
    #include <sys/ioctl.h>
    #include <fcntl.h>
#endif


//static const double GiB = 1024. * 1024. * 1024.;

class DeviceDisk
{
public:
    uint64_t Size=0;
    uint64_t Used=0;
    uint64_t Free=0;
    double SizeGiB=0.;
    double UsedGiB=0.;
    double FreeGiB=0.;
    double FreePercent=0.;

    struct info
    {
        string filesystem="";
        string mount="";
        uint64_t size=0;
        uint64_t used=0;
        uint64_t free=0;
    };

    //! \brief Construct a DeviceDisk and populate the mounted filesystem list.
    DeviceDisk();

    //! \brief Return filesystem info structs for all mounted volumes.
    //! \return Vector of info entries (mount point, total, used, free in bytes).
    vector <info> getInfo();

    //! \brief Return the used-fraction (0.0–1.0) of the filesystem containing \p path.
    //! \param path Any path on the target filesystem.
    //! \return Ratio of used space to total space.
    double getAll(const string path);

    //! \brief Return the total size of the filesystem containing \p path in bytes.
    uint64_t getSize(const string path);

    //! \brief Return the used bytes on the filesystem containing \p path.
    uint64_t getUsed(const string path);

    //! \brief Return the free bytes on the filesystem containing \p path.
    uint64_t getFree(string path);

    //! \brief Return the used-fraction across all mounted filesystems combined.
    double getAll();

    //! \brief Return the total bytes across all mounted filesystems.
    uint64_t getSize();

    //! \brief Return the used bytes across all mounted filesystems.
    uint64_t getUsed();

    //! \brief Return the free bytes across all mounted filesystems.
    uint64_t getFree();

    //! \brief Return the free space on the filesystem containing \p path in gibibytes.
    double getFreeGiB(string path);

    //! \brief Return the used space on the filesystem containing \p path in gibibytes.
    double getUsedGiB(string path);

    //! \brief Return the total size of the filesystem containing \p path in gibibytes.
    double getSizeGiB(string path);

    //! \brief Return the free space across all filesystems in gibibytes.
    double getFreeGiB();

    //! \brief Return the used space across all filesystems in gibibytes.
    double getUsedGiB();

    //! \brief Return the total size across all filesystems in gibibytes.
    double getSizeGiB();
};

#endif // DEVICEDISK_H
