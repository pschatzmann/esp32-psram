/*
 Print.h - Base class that provides print() and println()
 Copyright (c) 2008 David A. Mellis.  All right reserved.

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
 */

#ifndef Print_h
#define Print_h

#include <stdint.h>
#include <stddef.h>

#include "WString.h"
#include "Printable.h"

#include "stdlib_noniso.h"

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2

/**
 * @class Print
 * @brief Base class that provides print() and println() functionality
 *
 * The Print class is the base class for all Arduino objects that can write output,
 * including Serial, Ethernet clients, and File objects. It provides a common
 * interface for printing various data types to these output destinations.
 */
class Print {
    private:
        int write_error = 0;
        template<typename T> size_t printNumber(T n, uint8_t base);
        template<typename T, typename... P> inline size_t _println(T v, P... args);
    protected:
        /**
         * @brief Set the write error flag
         * @param err Error code to set (default: 1)
         */
        void setWriteError(int err = 1) {
            write_error = err;
        }
    public:
        /**
         * @brief Default constructor
         */
        Print() {}

        /**
         * @brief Get the current write error state
         * @return Current error code (0 = no error)
         */
        int getWriteError() {
            return write_error;
        }
        
        /**
         * @brief Clear the write error state
         */
        void clearWriteError() {
            setWriteError(0);
        }

        /**
         * @brief Write a single byte to the output
         * @param b The byte to write
         * @return Number of bytes written (1 on success, 0 on failure)
         * @note This is a pure virtual function that must be implemented by subclasses
         */
        virtual size_t write(uint8_t) = 0;
        
        /**
         * @brief Write a null-terminated string to the output
         * @param str Pointer to the string to write
         * @return Number of bytes written
         */
        size_t write(const char *str) {
            if(str == NULL)
                return 0;
            return write((const uint8_t *) str, strlen_P(str));
        }
        
        /**
         * @brief Write a buffer of bytes to the output
         * @param buffer Pointer to the data to write
         * @param size Number of bytes to write
         * @return Number of bytes written
         */
        virtual size_t write(const uint8_t *buffer, size_t size);
        
        /**
         * @brief Write a buffer of characters to the output
         * @param buffer Pointer to the character buffer
         * @param size Number of characters to write
         * @return Number of bytes written
         */
        size_t write(const char *buffer, size_t size) {
            return write((const uint8_t *) buffer, size);
        }
        
        // These handle ambiguity for write(0) case, because (0) can be a pointer or an integer
        /**
         * @brief Write a short value as a byte
         * @param t Value to write
         * @return Number of bytes written
         */
        inline size_t write(short t) { return write((uint8_t)t); }
        
        /**
         * @brief Write an unsigned short value as a byte
         * @param t Value to write
         * @return Number of bytes written
         */
        inline size_t write(unsigned short t) { return write((uint8_t)t); }
        
        /**
         * @brief Write an int value as a byte
         * @param t Value to write
         * @return Number of bytes written
         */
        inline size_t write(int t) { return write((uint8_t)t); }
        
        /**
         * @brief Write an unsigned int value as a byte
         * @param t Value to write
         * @return Number of bytes written
         */
        inline size_t write(unsigned int t) { return write((uint8_t)t); }
        
        /**
         * @brief Write a long value as a byte
         * @param t Value to write
         * @return Number of bytes written
         */
        inline size_t write(long t) { return write((uint8_t)t); }
        
        /**
         * @brief Write an unsigned long value as a byte
         * @param t Value to write
         * @return Number of bytes written
         */
        inline size_t write(unsigned long t) { return write((uint8_t)t); }
        
        /**
         * @brief Write a long long value as a byte
         * @param t Value to write
         * @return Number of bytes written
         */
        inline size_t write(long long t) { return write((uint8_t)t); }
        
        /**
         * @brief Write an unsigned long long value as a byte
         * @param t Value to write
         * @return Number of bytes written
         */
        inline size_t write(unsigned long long t) { return write((uint8_t)t); }
        
        /**
         * @brief Write a char value as a byte
         * @param c Character to write
         * @return Number of bytes written
         */
        inline size_t write(char c) { return write((uint8_t) c); }
        
        /**
         * @brief Write a signed byte as an unsigned byte
         * @param c Byte to write
         * @return Number of bytes written
         */
        inline size_t write(int8_t c) { return write((uint8_t) c); }

        /**
         * @brief Get the number of bytes available in the write buffer
         * @return The number of bytes available in the write buffer (0 by default)
         * @note Subclasses should override this if they implement buffering
         */
        virtual int availableForWrite() { return 0; }

        /**
         * @brief Print formatted data (printf style)
         * @param format Format string
         * @param ... Variable arguments for format string
         * @return Number of bytes written
         */
        size_t printf(const char * format, ...)  __attribute__ ((format (printf, 2, 3)));
        
        /**
         * @brief Print formatted data from program memory (printf style)
         * @param format Format string in program memory
         * @param ... Variable arguments for format string
         * @return Number of bytes written
         */
        size_t printf_P(PGM_P format, ...) __attribute__((format(printf, 2, 3)));
        
        /**
         * @brief Print a flash string
         * @param str Flash string to print
         * @return Number of bytes written
         */
        size_t print(const __FlashStringHelper *);
        
        /**
         * @brief Print a String object
         * @param s String to print
         * @return Number of bytes written
         */
        size_t print(const String &);
        
        /**
         * @brief Print a null-terminated string
         * @param str String to print
         * @return Number of bytes written
         */
        size_t print(const char[]);
        
        /**
         * @brief Print a single character
         * @param c Character to print
         * @return Number of bytes written
         */
        size_t print(char);
        
        /**
         * @brief Print an unsigned byte value
         * @param b Value to print
         * @param base Number base to use (default: DEC)
         * @return Number of bytes written
         */
        size_t print(unsigned char, int = DEC);
        
        /**
         * @brief Print an integer value
         * @param n Value to print
         * @param base Number base to use (default: DEC)
         * @return Number of bytes written
         */
        size_t print(int, int = DEC);
        
        /**
         * @brief Print an unsigned integer value
         * @param n Value to print
         * @param base Number base to use (default: DEC)
         * @return Number of bytes written
         */
        size_t print(unsigned int, int = DEC);
        
        /**
         * @brief Print a long integer value
         * @param n Value to print
         * @param base Number base to use (default: DEC)
         * @return Number of bytes written
         */
        size_t print(long, int = DEC);
        
        /**
         * @brief Print an unsigned long integer value
         * @param n Value to print
         * @param base Number base to use (default: DEC)
         * @return Number of bytes written
         */
        size_t print(unsigned long, int = DEC);
        
        /**
         * @brief Print a long long integer value
         * @param n Value to print
         * @param base Number base to use (default: DEC)
         * @return Number of bytes written
         */
        size_t print(long long, int = DEC);
        
        /**
         * @brief Print an unsigned long long integer value
         * @param n Value to print
         * @param base Number base to use (default: DEC)
         * @return Number of bytes written
         */
        size_t print(unsigned long long, int = DEC);
        
        /**
         * @brief Print a floating point value
         * @param n Value to print
         * @param digits Number of decimal places to display (default: 2)
         * @return Number of bytes written
         */
        size_t print(double, int = 2);
        
        /**
         * @brief Print a Printable object
         * @param obj Object to print
         * @return Number of bytes written
         */
        size_t print(const Printable&);

        /**
         * @brief Print a flash string followed by a newline
         * @param str Flash string to print
         * @return Number of bytes written
         */
        size_t println(const __FlashStringHelper *);
        
        /**
         * @brief Print a String object followed by a newline
         * @param s String to print
         * @return Number of bytes written
         */
        size_t println(const String &s);
        
        /**
         * @brief Print a null-terminated string followed by a newline
         * @param str String to print
         * @return Number of bytes written
         */
        size_t println(const char[]);
        
        /**
         * @brief Print a single character followed by a newline
         * @param c Character to print
         * @return Number of bytes written
         */
        size_t println(char);
        
        /**
         * @brief Print an unsigned byte value followed by a newline
         * @param b Value to print
         * @param base Number base to use (default: DEC)
         * @return Number of bytes written
         */
        size_t println(unsigned char, int = DEC);
        
        /**
         * @brief Print an integer value followed by a newline
         * @param n Value to print
         * @param base Number base to use (default: DEC)
         * @return Number of bytes written
         */
        size_t println(int, int = DEC);
        
        /**
         * @brief Print an unsigned integer value followed by a newline
         * @param n Value to print
         * @param base Number base to use (default: DEC)
         * @return Number of bytes written
         */
        size_t println(unsigned int, int = DEC);
        
        /**
         * @brief Print a long integer value followed by a newline
         * @param n Value to print
         * @param base Number base to use (default: DEC)
         * @return Number of bytes written
         */
        size_t println(long, int = DEC);
        
        /**
         * @brief Print an unsigned long integer value followed by a newline
         * @param n Value to print
         * @param base Number base to use (default: DEC)
         * @return Number of bytes written
         */
        size_t println(unsigned long, int = DEC);
        
        /**
         * @brief Print a long long integer value followed by a newline
         * @param n Value to print
         * @param base Number base to use (default: DEC)
         * @return Number of bytes written
         */
        size_t println(long long, int = DEC);
        
        /**
         * @brief Print an unsigned long long integer value followed by a newline
         * @param n Value to print
         * @param base Number base to use (default: DEC)
         * @return Number of bytes written
         */
        size_t println(unsigned long long, int = DEC);
        
        /**
         * @brief Print a floating point value followed by a newline
         * @param n Value to print
         * @param digits Number of decimal places to display (default: 2)
         * @return Number of bytes written
         */
        size_t println(double, int = 2);
        
        /**
         * @brief Print a Printable object followed by a newline
         * @param obj Object to print
         * @return Number of bytes written
         */
        size_t println(const Printable&);
        
        /**
         * @brief Print just a newline
         * @return Number of bytes written
         */
        size_t println(void);

        /**
         * @brief Flush the output buffer
         * 
         * This function should wait for all outgoing characters to be sent.
         * The output buffer should be empty after this call.
         * Empty implementation by default in Print class.
         */
        virtual void flush() { }

        /**
         * @brief Check if the output can timeout
         * 
         * By default, write operations can timeout (like for network or serial).
         * Child classes can override this to false (like String).
         * 
         * @return true if output operations can timeout, false otherwise
         */
        virtual bool outputCanTimeout () { return true; }
};

/**
 * @brief Specialized template for printing floating point numbers
 * 
 * @param number The floating point number to print
 * @param digits The number of decimal places to display
 * @return Number of bytes written
 */
template<> size_t Print::printNumber(double number, uint8_t digits);

#endif