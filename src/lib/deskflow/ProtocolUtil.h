/*
 * Deskflow -- mouse and keyboard sharing utility
 * Copyright (C) 2012-2016 Symless Ltd.
 * Copyright (C) 2002 Chris Schoeneman
 *
 * This package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * found in the file LICENSE that should have accompanied this file.
 *
 * This package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "base/EventTypes.h"
#include "io/XIO.h"

#include <stdarg.h>

namespace deskflow {
class IStream;
}

//! Deskflow protocol utilities
/*!
This class provides various functions for implementing the deskflow
protocol.
*/
class ProtocolUtil
{
public:
  //! Write formatted data
  /*!
  Write formatted binary data to a stream.  \c fmt consists of
  regular characters and format specifiers.  Format specifiers
  begin with \%.  All characters not part of a format specifier
  are regular and are transmitted unchanged.

  Format specifiers are:
  - \%\%   -- literal `\%'
  - \%1i  -- converts integer argument to 1 byte integer
  - \%2i  -- converts integer argument to 2 byte integer in NBO
  - \%4i  -- converts integer argument to 4 byte integer in NBO
  - \%1I  -- converts std::vector<UInt8>* to 1 byte integers
  - \%2I  -- converts std::vector<UInt16>* to 2 byte integers in NBO
  - \%4I  -- converts std::vector<UInt32>* to 4 byte integers in NBO
  - \%s   -- converts String* to stream of bytes
  - \%S   -- converts integer N and const UInt8* to stream of N bytes
  */
  static void writef(deskflow::IStream *, const char *fmt, ...);

  //! Read formatted data
  /*!
  Read formatted binary data from a buffer.  This performs the
  reverse operation of writef().  Returns true if the entire
  format was successfully parsed, false otherwise.

  Format specifiers are:
  - \%\%   -- read (and discard) a literal `\%'
  - \%1i  -- reads a 1 byte integer; argument is a SInt32* or UInt32*
  - \%2i  -- reads an NBO 2 byte integer;  arg is SInt32* or UInt32*
  - \%4i  -- reads an NBO 4 byte integer;  arg is SInt32* or UInt32*
  - \%1I  -- reads 1 byte integers;  arg is std::vector<UInt8>*
  - \%2I  -- reads NBO 2 byte integers;  arg is std::vector<UInt16>*
  - \%4I  -- reads NBO 4 byte integers;  arg is std::vector<UInt32>*
  - \%s   -- reads bytes;  argument must be a String*, \b not a char*
  */
  static bool readf(deskflow::IStream *, const char *fmt, ...);

private:
  static void vwritef(deskflow::IStream *, const char *fmt, UInt32 size, va_list);
  static void vreadf(deskflow::IStream *, const char *fmt, va_list);

  static UInt32 getLength(const char *fmt, va_list);
  static void writef(std::vector<UInt8> &, const char *fmt, va_list);
  static UInt32 eatLength(const char **fmt);
  static void read(deskflow::IStream *, void *, UInt32);

  /**
   * @brief Handles 1,2, or 4 byte Integers
   */
  static UInt8 read1ByteInt(deskflow::IStream *stream);
  static UInt16 read2BytesInt(deskflow::IStream *stream);
  static UInt32 read4BytesInt(deskflow::IStream *stream);

  /**
   * @brief Handles a Vector of integers
   */
  static void readVector1ByteInt(deskflow::IStream *, std::vector<UInt8> &);
  static void readVector2BytesInt(deskflow::IStream *, std::vector<UInt16> &);
  static void readVector4BytesInt(deskflow::IStream *, std::vector<UInt32> &);
  static UInt32 readVectorSize(deskflow::IStream *stream);

  /**
   * @brief Handles an array of bytes
   */
  static void readBytes(deskflow::IStream *, UInt32, String *);
};

//! Mismatched read exception
/*!
Thrown by ProtocolUtil::readf() when the data being read does not
match the format.
*/
class XIOReadMismatch : public XIO
{
public:
  // XBase overrides
  virtual String getWhat() const throw();
};
