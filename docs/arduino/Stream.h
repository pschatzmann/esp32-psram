/*
 Stream.h - base class for character-based streams.
 Copyright (c) 2010 David A. Mellis.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

 parsing functions based on TextFinder library by Michael Margolis
 */

/**
 * @file Stream.h
 * @brief Base class for character-based streams
 * 
 * This class provides a common interface for handling stream operations,
 * including timeout management, parsing, and transferring data between streams.
 */

#ifndef Stream_h
#define Stream_h

#include <debug.h>
#include <inttypes.h>
#include <Print.h>
#include <PolledTimeout.h>
#include <sys/types.h> // ssize_t

// Stream::read API returns int
#define STREAM_READ_RETURNS_INT 1

// Stream::send API is present
#define STREAMSEND_API 1

/**
 * @class Stream
 * @brief Base class for all character and binary stream operations
 * 
 * The Stream class adds buffering, timeout management, parsing operations,
 * and data transfer capabilities to the basic Print class.
 * It provides methods for reading and manipulating data from streams such as
 * Serial ports, network connections, and files.
 */
class Stream: public Print {
    protected:
        unsigned long _timeout = 1000;  ///< Number of milliseconds to wait for the next char before aborting timed read
        unsigned long _startMillis;     ///< Used for timeout measurement
        
        /**
         * @brief Read a character with timeout
         * @return The read character or -1 if timeout
         */
        int timedRead();
        
        /**
         * @brief Peek at the next character with timeout
         * @return The next character or -1 if timeout
         */
        int timedPeek();
        
        /**
         * @brief Peek at the next digit in the stream
         * @param detectDecimal Whether to consider decimal points as valid parts of numbers
         * @return The next numeric digit or -1 if timeout or non-digit
         */
        int peekNextDigit(bool detectDecimal = false);

    public:
        /**
         * @brief Get the number of bytes available to read
         * @return Number of bytes available
         */
        virtual int available() = 0;
        
        /**
         * @brief Read a single byte from the stream
         * @return The next byte, or -1 if no data available
         */
        virtual int read() = 0;
        
        /**
         * @brief Peek at the next byte without removing it from the stream
         * @return The next byte, or -1 if no data available
         */
        virtual int peek() = 0;

        /**
         * @brief Default constructor
         */
        Stream() {}
        
        /**
         * @brief Virtual destructor
         */
        virtual ~Stream() {}

        // Timeout management

        /**
         * @brief Set the timeout for stream operations
         * @param timeout Maximum milliseconds to wait for stream data
         */
        void setTimeout(unsigned long timeout);
        
        /**
         * @brief Get the current timeout setting
         * @return Current timeout in milliseconds
         */
        unsigned long getTimeout() const { return _timeout; }

        // Find methods

        /**
         * @brief Search for a string in the stream
         * @param target The string to search for
         * @return true if found, false if timed out
         */
        bool find(const char *target);
        
        /**
         * @brief Search for a string in the stream (uint8_t variant)
         * @param target The string to search for
         * @return true if found, false if timed out
         */
        bool find(uint8_t *target) {
            return find((char *) target);
        }

        /**
         * @brief Search for a string of given length in the stream
         * @param target The string to search for
         * @param length Length of the target string
         * @return true if found, false if timed out
         */
        bool find(const char *target, size_t length);
        
        /**
         * @brief Search for a string of given length in the stream (uint8_t variant)
         * @param target The string to search for
         * @param length Length of the target string
         * @return true if found, false if timed out
         */
        bool find(const uint8_t *target, size_t length) {
            return find((char *) target, length);
        }

        /**
         * @brief Search for a single character in the stream
         * @param target Character to search for
         * @return true if found, false if timed out
         */
        bool find(char target) { return find(&target, 1); }

        /**
         * @brief Search for a string until a terminator is found
         * @param target The string to search for
         * @param terminator String that terminates the search
         * @return true if target found, false if timed out or terminator found first
         */
        bool findUntil(const char *target, const char *terminator);
        
        /**
         * @brief Search for a string until a terminator is found (uint8_t variant)
         * @param target The string to search for
         * @param terminator String that terminates the search
         * @return true if target found, false if timed out or terminator found first
         */
        bool findUntil(const uint8_t *target, const char *terminator) {
            return findUntil((char *) target, terminator);
        }

        /**
         * @brief Search for a string of given length until a terminator is found
         * @param target The string to search for
         * @param targetLen Length of the target string
         * @param terminate String that terminates the search
         * @param termLen Length of the terminator string
         * @return true if target found, false if timed out or terminator found first
         */
        bool findUntil(const char *target, size_t targetLen, const char *terminate, size_t termLen);
        
        /**
         * @brief Search for a string of given length until a terminator is found (uint8_t variant)
         * @param target The string to search for
         * @param targetLen Length of the target string
         * @param terminate String that terminates the search
         * @param termLen Length of the terminator string
         * @return true if target found, false if timed out or terminator found first
         */
        bool findUntil(const uint8_t *target, size_t targetLen, const char *terminate, size_t termLen) {
            return findUntil((char *) target, targetLen, terminate, termLen);
        }

        // Parsing methods

        /**
         * @brief Parse an integer from the stream
         * @return The first valid integer value from the current position
         * @note Skips non-digit characters and stops at first non-digit after a number
         */
        long parseInt();

        /**
         * @brief Parse a float from the stream
         * @return The first valid floating point value from the current position
         * @note Skips non-digit characters and stops at first non-digit after a number
         */
        float parseFloat();

        // Reading methods

        /**
         * @brief Read bytes from the stream into a buffer
         * @param buffer Buffer to store the data
         * @param length Maximum number of bytes to read
         * @return Number of bytes read (0 means no valid data found)
         */
        virtual size_t readBytes(char *buffer, size_t length);
        
        /**
         * @brief Read bytes from the stream into a buffer (uint8_t variant)
         * @param buffer Buffer to store the data
         * @param length Maximum number of bytes to read
         * @return Number of bytes read (0 means no valid data found)
         */
        virtual size_t readBytes(uint8_t *buffer, size_t length) {
            return readBytes((char *) buffer, length);
        }

        /**
         * @brief Read bytes until a terminator character is found
         * @param terminator Character that terminates the read
         * @param buffer Buffer to store the data
         * @param length Maximum number of bytes to read
         * @return Number of bytes read (0 means no valid data found)
         */
        size_t readBytesUntil(char terminator, char *buffer, size_t length);
        
        /**
         * @brief Read bytes until a terminator character is found (uint8_t variant)
         * @param terminator Character that terminates the read
         * @param buffer Buffer to store the data
         * @param length Maximum number of bytes to read
         * @return Number of bytes read (0 means no valid data found)
         */
        size_t readBytesUntil(char terminator, uint8_t *buffer, size_t length) {
            return readBytesUntil(terminator, (char *) buffer, length);
        }

        /**
         * @brief Read a string from the stream
         * @return String containing the data read
         */
        virtual String readString();
        
        /**
         * @brief Read a string until a terminator character is found
         * @param terminator Character that terminates the read
         * @return String containing the data read
         */
        String readStringUntil(char terminator);
        
        /**
         * @brief Read a string until a terminator string is found a specific number of times
         * @param terminator String that terminates the read
         * @param untilTotalNumberOfOccurrences Number of times the terminator must be found
         * @return String containing the data read
         */
        String readStringUntil(const char* terminator, uint32_t untilTotalNumberOfOccurrences = 1);

        /**
         * @brief Read multiple bytes from the stream
         * @param buffer Buffer to store the data
         * @param len Maximum number of bytes to read
         * @return Number of bytes read
         */
        virtual int read(uint8_t* buffer, size_t len);
        
        /**
         * @brief Read multiple bytes from the stream (char variant)
         * @param buffer Buffer to store the data
         * @param len Maximum number of bytes to read
         * @return Number of bytes read
         */
        int read(char* buffer, size_t len) { return read((uint8_t*)buffer, len); }

        //////////////////// Direct buffer access API

        /**
         * @brief Check if this stream implements the peek buffer API
         * @return true if peek buffer API is available, false otherwise
         */
        virtual bool hasPeekBufferAPI() const { return false; }

        /**
         * @brief Get the number of bytes available in the peek buffer
         * @return Number of bytes available
         */
        virtual size_t peekAvailable() { return 0; }

        /**
         * @brief Get a pointer to the peek buffer
         * @return Pointer to the peek buffer, or nullptr if not available
         * @note Don't call read() between peekBuffer() and peekConsume()
         */
        virtual const char* peekBuffer() { return nullptr; }

        /**
         * @brief Consume bytes from the peek buffer
         * @param consume Number of bytes to consume
         */
        virtual void peekConsume(size_t consume) { (void)consume; }

        /**
         * @brief Check if input operations can timeout
         * @return true if input operations can timeout, false otherwise
         */
        virtual bool inputCanTimeout() { return true; }

        //////////////////// Stream-to-stream transfer API

        /**
         * @brief Time management for stream operations
         */
        using oneShotMs = esp8266::polledTimeout::oneShotFastMs;
        
        /**
         * @brief Size of temporary stack buffer for stream transfers
         */
        static constexpr int temporaryStackBufferSize = 64;

        /**
         * @brief Status codes for stream transfer operations
         */
        enum class Report
        {
            Success = 0,       ///< Operation completed successfully
            TimedOut,          ///< Operation timed out
            ReadError,         ///< Error reading from source
            WriteError,        ///< Error writing to destination
            ShortOperation,    ///< Operation completed but transferred less than requested
        };

        // Deprecated methods - retained for compatibility

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

        /**
         * @brief Transfer available data without waiting (deprecated)
         * @param to Destination stream
         * @return Number of bytes transferred
         * @deprecated Use the Stream* variant instead
         */
        [[deprecated]] size_t sendAvailable(Print* to) { return sendGeneric(to, -1, -1, oneShotMs::alwaysExpired); }
        
        /**
         * @brief Transfer available data without waiting (deprecated)
         * @param to Destination stream
         * @return Number of bytes transferred
         * @deprecated Use the Stream* variant instead
         */
        [[deprecated]] size_t sendAvailable(Print& to) { return sendAvailable(&to); }

        /**
         * @brief Transfer all data with timeout (deprecated)
         * @param to Destination stream
         * @param timeoutMs Timeout in milliseconds
         * @return Number of bytes transferred
         * @deprecated Use the Stream* variant instead
         */
        [[deprecated]] size_t sendAll(Print* to, const oneShotMs::timeType timeoutMs = oneShotMs::neverExpires) { return sendGeneric(to, -1, -1, timeoutMs); }
        
        /**
         * @brief Transfer all data with timeout (deprecated)
         * @param to Destination stream
         * @param timeoutMs Timeout in milliseconds
         * @return Number of bytes transferred
         * @deprecated Use the Stream* variant instead
         */
        [[deprecated]] size_t sendAll(Print& to, const oneShotMs::timeType timeoutMs = oneShotMs::neverExpires) { return sendAll(&to, timeoutMs); }

        /**
         * @brief Transfer data until a specific character is found (deprecated)
         * @param to Destination stream
         * @param readUntilChar Character to stop at
         * @param timeoutMs Timeout in milliseconds
         * @return Number of bytes transferred
         * @deprecated Use the Stream* variant instead
         */
        [[deprecated]] size_t sendUntil(Print* to, const int readUntilChar, const oneShotMs::timeType timeoutMs = oneShotMs::neverExpires) { return sendGeneric(to, -1, readUntilChar, timeoutMs); }
        
        /**
         * @brief Transfer data until a specific character is found (deprecated)
         * @param to Destination stream
         * @param readUntilChar Character to stop at
         * @param timeoutMs Timeout in milliseconds
         * @return Number of bytes transferred
         * @deprecated Use the Stream* variant instead
         */
        [[deprecated]] size_t sendUntil(Print& to, const int readUntilChar, const oneShotMs::timeType timeoutMs = oneShotMs::neverExpires) { return sendUntil(&to, readUntilChar, timeoutMs); }

        /**
         * @brief Transfer a specific amount of data with timeout (deprecated)
         * @param to Destination stream
         * @param maxLen Maximum number of bytes to transfer
         * @param timeoutMs Timeout in milliseconds
         * @return Number of bytes transferred
         * @deprecated Use the Stream* variant instead
         */
        [[deprecated]] size_t sendSize(Print* to, const ssize_t maxLen, const oneShotMs::timeType timeoutMs = oneShotMs::neverExpires) { return sendGeneric(to, maxLen, -1, timeoutMs); }
        
        /**
         * @brief Transfer a specific amount of data with timeout (deprecated)
         * @param to Destination stream
         * @param maxLen Maximum number of bytes to transfer
         * @param timeoutMs Timeout in milliseconds
         * @return Number of bytes transferred
         * @deprecated Use the Stream* variant instead
         */
        [[deprecated]] size_t sendSize(Print& to, const ssize_t maxLen, const oneShotMs::timeType timeoutMs = oneShotMs::neverExpires) { return sendSize(&to, maxLen, timeoutMs); }

#pragma GCC diagnostic pop

        // Current Stream-to-Stream methods

        /**
         * @brief Transfer available data without waiting
         * @param to Destination stream
         * @return Number of bytes transferred
         */
        size_t sendAvailable(Stream* to) { return sendGeneric(to, -1, -1, oneShotMs::alwaysExpired); }
        
        /**
         * @brief Transfer available data without waiting
         * @param to Destination stream
         * @return Number of bytes transferred
         */
        size_t sendAvailable(Stream& to) { return sendAvailable(&to); }
        
        /**
         * @brief Transfer available data without waiting
         * @param to Destination stream
         * @return Number of bytes transferred
         */
        size_t sendAvailable(Stream&& to) { return sendAvailable(&to); }

        /**
         * @brief Transfer all data with timeout
         * @param to Destination stream
         * @param timeoutMs Timeout in milliseconds
         * @return Number of bytes transferred
         */
        size_t sendAll(Stream* to, const oneShotMs::timeType timeoutMs = oneShotMs::neverExpires) { return sendGeneric(to, -1, -1, timeoutMs); }
        
        /**
         * @brief Transfer all data with timeout
         * @param to Destination stream
         * @param timeoutMs Timeout in milliseconds
         * @return Number of bytes transferred
         */
        size_t sendAll(Stream& to, const oneShotMs::timeType timeoutMs = oneShotMs::neverExpires) { return sendAll(&to, timeoutMs); }
        
        /**
         * @brief Transfer all data with timeout
         * @param to Destination stream
         * @param timeoutMs Timeout in milliseconds
         * @return Number of bytes transferred
         */
        size_t sendAll(Stream&& to, const oneShotMs::timeType timeoutMs = oneShotMs::neverExpires) { return sendAll(&to, timeoutMs); }

        /**
         * @brief Transfer data until a specific character is found
         * @param to Destination stream
         * @param readUntilChar Character to stop at (the character is read but not transferred)
         * @param timeoutMs Timeout in milliseconds
         * @return Number of bytes transferred
         */
        size_t sendUntil(Stream* to, const int readUntilChar, const oneShotMs::timeType timeoutMs = oneShotMs::neverExpires) { return sendGeneric(to, -1, readUntilChar, timeoutMs); }
        
        /**
         * @brief Transfer data until a specific character is found
         * @param to Destination stream
         * @param readUntilChar Character to stop at (the character is read but not transferred)
         * @param timeoutMs Timeout in milliseconds
         * @return Number of bytes transferred
         */
        size_t sendUntil(Stream& to, const int readUntilChar, const oneShotMs::timeType timeoutMs = oneShotMs::neverExpires) { return sendUntil(&to, readUntilChar, timeoutMs); }
        
        /**
         * @brief Transfer data until a specific character is found
         * @param to Destination stream
         * @param readUntilChar Character to stop at (the character is read but not transferred)
         * @param timeoutMs Timeout in milliseconds
         * @return Number of bytes transferred
         */
        size_t sendUntil(Stream&& to, const int readUntilChar, const oneShotMs::timeType timeoutMs = oneShotMs::neverExpires) { return sendUntil(&to, readUntilChar, timeoutMs); }

        /**
         * @brief Transfer a specific amount of data with timeout
         * @param to Destination stream
         * @param maxLen Maximum number of bytes to transfer
         * @param timeoutMs Timeout in milliseconds
         * @return Number of bytes transferred
         */
        size_t sendSize(Stream* to, const ssize_t maxLen, const oneShotMs::timeType timeoutMs = oneShotMs::neverExpires) { return sendGeneric(to, maxLen, -1, timeoutMs); }
        
        /**
         * @brief Transfer a specific amount of data with timeout
         * @param to Destination stream
         * @param maxLen Maximum number of bytes to transfer
         * @param timeoutMs Timeout in milliseconds
         * @return Number of bytes transferred
         */
        size_t sendSize(Stream& to, const ssize_t maxLen, const oneShotMs::timeType timeoutMs = oneShotMs::neverExpires) { return sendSize(&to, maxLen, timeoutMs); }
        
        /**
         * @brief Transfer a specific amount of data with timeout
         * @param to Destination stream
         * @param maxLen Maximum number of bytes to transfer
         * @param timeoutMs Timeout in milliseconds
         * @return Number of bytes transferred
         */
        size_t sendSize(Stream&& to, const ssize_t maxLen, const oneShotMs::timeType timeoutMs = oneShotMs::neverExpires) { return sendSize(&to, maxLen, timeoutMs); }

        /**
         * @brief Get the number of bytes remaining in the stream
         * @return Number of bytes remaining, or -1 if unknown
         */
        virtual ssize_t streamRemaining() { return -1; }

        /**
         * @brief Get the result of the last send operation
         * @return Status code indicating success or failure reason
         */
        Report getLastSendReport() const { return _sendReport; }

    protected:
        /**
         * @brief Generic transfer method for Print objects (deprecated)
         * @param to Destination stream
         * @param len Maximum number of bytes to transfer (-1 for unlimited)
         * @param readUntilChar Character to stop at (-1 for none)
         * @param timeoutMs Timeout in milliseconds
         * @return Number of bytes transferred
         */
        [[deprecated]]
        size_t sendGeneric(Print* to,
                          const ssize_t len = -1,
                          const int readUntilChar = -1,
                          oneShotMs::timeType timeoutMs = oneShotMs::neverExpires);

        /**
         * @brief Generic transfer method for Stream objects
         * @param to Destination stream
         * @param len Maximum number of bytes to transfer (-1 for unlimited)
         * @param readUntilChar Character to stop at (-1 for none)
         * @param timeoutMs Timeout in milliseconds
         * @return Number of bytes transferred
         */
        size_t sendGeneric(Stream* to,
                          const ssize_t len = -1,
                          const int readUntilChar = -1,
                          oneShotMs::timeType timeoutMs = oneShotMs::neverExpires);

        /**
         * @brief Transfer using peek buffer API
         * @param to Destination stream
         * @param len Maximum number of bytes to transfer
         * @param readUntilChar Character to stop at
         * @param timeoutMs Timeout in milliseconds
         * @return Number of bytes transferred
         */
        size_t SendGenericPeekBuffer(Print* to, const ssize_t len, const int readUntilChar, const oneShotMs::timeType timeoutMs);
        
        /**
         * @brief Transfer using regular read until character
         * @param to Destination stream
         * @param len Maximum number of bytes to transfer
         * @param readUntilChar Character to stop at
         * @param timeoutMs Timeout in milliseconds
         * @return Number of bytes transferred
         */
        size_t SendGenericRegularUntil(Print* to, const ssize_t len, const int readUntilChar, const oneShotMs::timeType timeoutMs);
        
        /**
         * @brief Transfer using regular read
         * @param to Destination stream
         * @param len Maximum number of bytes to transfer
         * @param timeoutMs Timeout in milliseconds
         * @return Number of bytes transferred
         */
        size_t SendGenericRegular(Print* to, const ssize_t len, const oneShotMs::timeType timeoutMs);

        /**
         * @brief Set the result of a send operation
         * @param report Status code to set
         */
        void setReport(Report report) { _sendReport = report; }

    private:
        /**
         * @brief Status of the last send operation
         */
        Report _sendReport = Report::Success;

    protected:
        /**
         * @brief Parse an integer with custom skip character
         * @param skipChar Character to ignore during parsing
         * @return The parsed integer value
         */
        long parseInt(char skipChar);
        
        /**
         * @brief Parse a float with custom skip character
         * @param skipChar Character to ignore during parsing
         * @return The parsed float value
         */
        float parseFloat(char skipChar);
};

#endif