/*!
 * \file channellib.h
 * \brief Channel multiplexing over PacketComm.
 * \ingroup cosmos_kernel_network
 * \defgroup cosmos_channellib Channel
 *
 * Implements a lightweight channel abstraction that multiplexes multiple
 * logical data streams over a single PacketComm transport. Each channel is
 * identified by a name and carries typed payloads. Use when an agent needs to
 * share a single radio or socket link among several concurrent data flows.
 */

#ifndef CHANNELLIB_H
#define CHANNELLIB_H

/*! \file channellib.h
* \brief Channel Support
*/

#include <mutex>
#include <thread>
#include "support/configCosmos.h"
#include "support/packetcomm.h"
#include "support/timelib.h"
#include "support/elapsedtime.h"

namespace Cosmos {
    namespace Support {
        //! \class Channel channellib.h "support/channellib.h"
        //! This class allows the construction and management of a ::vector of queues with associated
        //! names, sizes, capacities and other properties. Initialization, using ::Init, will create
        //! a set of standard channels: SELF, NET EPS ADCS, FILE, and EXEC. These can be used to
        //! support standard sub-agents by the same name. Channels are then extended using ::Add.
        //! Each channel has a queue of Cosmos::Support::PacketComm packets of ::maximum length.
        //! Packets are added using ::Push, until ::maximum is reached, at which older packets are removed
        //! from the queue. Packets are extracted from the queue using ::Pull.
        class Channel
        {
        public:
            Channel();
            ~Channel();

            //! Storage for channels
            static constexpr uint16_t NATIVE_BUFFER_SIZE = 1400;
            static constexpr uint16_t PACKETCOMM_PACKETIZED_SIZE = NATIVE_BUFFER_SIZE;
            static constexpr uint16_t PACKETCOMM_WRAPPED_SIZE = PACKETCOMM_PACKETIZED_SIZE;
            static constexpr uint16_t PACKETCOMM_DATA_SIZE = PACKETCOMM_WRAPPED_SIZE - (COSMOS_SIZEOF(PacketComm::Header)+2);

            struct channelstruc
            {
                channelstruc() {age_timer.start(get_unix_time());}
                ElapsedTime age_timer;
                uint32_t packets = 0;
                uint32_t level = 0;
                size_t bytes = 0;
                string name = "";
                queue<PacketComm> quu;
                std::recursive_mutex* mtx = nullptr;
                uint16_t datasize = PACKETCOMM_DATA_SIZE;
                uint16_t rawsize = PACKETCOMM_PACKETIZED_SIZE;
                uint16_t maximum = 100;
                float byte_rate = 1e9;
                float wakeup_timer = 600.;
                // Radio test variables
                thread testthread;
                uint8_t testrunning = 0;
                ElapsedTime testet;
                uint32_t testcount = 0;
                float testseconds = 0.;
                float speed = 0.;
                float remaining = 60.;
                uint8_t test_dest = 0;
                uint8_t test_start = 0;
                uint8_t test_byte = test_start;
                uint8_t test_step = 1;
                uint8_t test_stop = 100;
                uint32_t test_id = 0;
                uint32_t test_bytes = 1000;
                int8_t enabled = 0;
            };
            vector<channelstruc> channel;
            //! \brief Initialise the channel table and set the verification word.
            //! \param verification Magic word stored for integrity checks; default 0x352e.
            //! \return 0 on success.
            int32_t Init(uint32_t verification=0x352e);

            //! \brief Verify that the channel table was initialised with the expected magic word.
            //! \param verification Expected magic word.
            //! \return 0 if valid, negative error code if mismatch.
            int32_t Check(uint32_t verification);

            //! \brief Add a named channel to the table.
            //! \param name      Channel name.
            //! \param datasize  Maximum data payload size in bytes; default PACKETCOMM_WRAPPED_SIZE.
            //! \param rawsize   Maximum raw (packetized) frame size; default 0 (same as datasize).
            //! \param byte_rate Nominal link rate in bytes/s; default 1e8.
            //! \param maximum   Queue depth limit (packets); default 100.
            //! \return Channel number on success, negative error code on failure.
            int32_t Add(string name, uint16_t datasize=PACKETCOMM_WRAPPED_SIZE, uint16_t rawsize=0, float byte_rate=1e8, uint16_t maximum=100);

            //! \brief Update a named channel's parameters (0 = keep existing).
            int32_t Update(string name, uint16_t datasize=0, uint16_t rawsize=0, float byte_rate=0, uint16_t maximum=0);

            //! \brief Update a channel by number's parameters (0 = keep existing).
            int32_t Update(uint8_t number, uint16_t datasize=0, uint16_t rawsize=0, float byte_rate=0, uint16_t maximum=0);

            //! \brief Look up a channel number by name.
            //! \param name Channel name.
            //! \return Channel number, or negative error code if not found.
            int32_t Find(string name);

            //! \brief Look up a channel name by number.
            //! \param number Channel number.
            //! \return Channel name string, or "" if not found.
            string Find(uint8_t number);

            //! \brief Enqueue a packet on a named channel.
            //! \param name   Channel name.
            //! \param packet Packet to enqueue.
            //! \return Queue depth after push, or negative error code.
            int32_t Push(string name, PacketComm &packet);

            //! \brief Enqueue a packet on a channel by number.
            int32_t Push(uint8_t number, PacketComm &packet);

            //! \brief Dequeue the front packet from a named channel.
            //! \param name   Channel name.
            //! \param packet Receives the dequeued packet.
            //! \return Remaining queue depth, or negative error code if empty.
            int32_t Pull(string name, PacketComm &packet);

            //! \brief Dequeue the front packet from a channel by number.
            int32_t Pull(uint8_t number, PacketComm &packet);

            //! \brief Return the queue depth of a named channel (or all channels if name="").
            int32_t Size(string name="");

            //! \brief Return the queue depth of a channel by number (or all if 0).
            int32_t Size(uint8_t number=0);

            //! \brief Discard all queued packets from a named channel.
            //! \return 0 on success.
            int32_t Clear(string name);

            //! \brief Discard all queued packets from a channel by number.
            int32_t Clear(uint8_t number);

            //! \brief Set the enabled state of a named channel.
            //! \param name  Channel name.
            //! \param value Positive = enable, 0 = disable, negative = toggle.
            //! \return New enabled state.
            int32_t Enable(string name, int8_t value);

            //! \brief Set the enabled state of a channel by number.
            int32_t Enable(uint8_t number, int8_t value);

            //! \brief Query the enabled state of a named channel.
            //! \return Current enabled value.
            int32_t Enabled(string name);

            //! \brief Query the enabled state of a channel by number.
            int32_t Enabled(uint8_t number);

            //! \brief Return seconds since a named channel last received a packet.
            double Age(string name);

            //! \brief Return seconds since a channel by number last received a packet.
            double Age(uint8_t number);

            //! \brief Get or set the wakeup timer threshold for a named channel (seconds).
            //! \param name  Channel name.
            //! \param value New threshold; 0 = query only; default 0.
            //! \return Current wakeup threshold in seconds.
            double WakeupTimer(string name, double value = 0.);

            //! \brief Get or set the wakeup timer threshold for a channel by number.
            double WakeupTimer(uint8_t number, double value = 0.);

            //! \brief Return total bytes received by a named channel.
            size_t Bytes(string name);

            //! \brief Return total bytes received by a channel by number.
            size_t Bytes(uint8_t number);

            //! \brief Return the current queue depth in bytes for a named channel.
            size_t Level(string name);

            //! \brief Return the current queue depth in bytes for a channel by number.
            size_t Level(uint8_t number);

            //! \brief Return the nominal byte rate of a named channel.
            float ByteRate(string name);

            //! \brief Return the nominal byte rate of a channel by number.
            float ByteRate(uint8_t number);

            //! \brief Return the total packet count for a named channel.
            uint32_t Packets(string name);

            //! \brief Return the total packet count for a channel by number.
            uint32_t Packets(uint8_t number);

            //! \brief Update the last-activity timestamp for a named channel.
            //! \param name    Channel name.
            //! \param seconds If > 0, set the age offset to this many seconds; default 0.
            //! \return Seconds since last activity.
            double Touch(string name, double seconds = 0.);

            //! \brief Update the last-activity timestamp for a channel by number.
            double Touch(uint8_t number, double seconds = 0.);

            //! \brief Increment a named channel's byte and packet counters.
            //! \param name         Channel name.
            //! \param byte_count   Bytes to add.
            //! \param packet_count Packets to add; default 1.
            //! \return New byte total, or negative error code.
            ssize_t Increment(string name, size_t byte_count, uint32_t packet_count=1);

            //! \brief Increment a channel's byte and packet counters by number.
            ssize_t Increment(uint8_t number, size_t byte_count, uint32_t packet_count=1);

            //! \brief Start a loopback radio test on a named channel.
            int32_t TestStart(string name, string radio, uint32_t id, uint8_t orig, uint8_t dest, uint8_t start, uint8_t step, uint8_t stop, uint32_t total);

            //! \brief Start a loopback radio test on a channel by number.
            int32_t TestStart(uint8_t number, uint8_t radio, uint32_t id, uint8_t orig, uint8_t dest, uint8_t start, uint8_t step, uint8_t stop, uint32_t total);

            //! \brief Stop a running radio test on a named channel.
            //! \param name    Channel name.
            //! \param seconds Maximum seconds to wait for the test thread to finish; default 5.
            //! \return 0 on success.
            int32_t TestStop(string name, float seconds=5.);

            //! \brief Stop a running radio test on a channel by number.
            int32_t TestStop(uint8_t number, float seconds=5.);

            //! \brief Return the remaining test duration for a named channel in seconds.
            float TestRemaining(string name);

            //! \brief Return the remaining test duration for a channel by number in seconds.
            float TestRemaining(uint8_t number);

            //! \brief Return the measured throughput of the last test on a named channel (bytes/s).
            float TestSpeed(string name);

            //! \brief Return the measured throughput of the last test on a channel by number.
            float TestSpeed(uint8_t number);

            //! \brief Execute one iteration of the radio test loop for a channel by number.
            //! \return Updated packet count.
            uint32_t TestLoop(uint8_t number, uint8_t radio, uint32_t id, uint8_t orig, uint8_t dest, uint8_t start, uint8_t step, uint8_t stop, uint32_t total);

            uint32_t verification = 0x352e;

        private:
        };
    }
}

#endif // CHANNELLIB_H
