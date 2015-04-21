// libpt: Plain Text Manipulation
// Written in 2015 by Jordan Vaughan
//
// To the extent possible under law, the author(s) have dedicated all copyright
// and related and neighboring rights to this software to the public domain
// worldwide. This software is distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along
// with this software. If not, see
// <http://creativecommons.org/publicdomain/zero/1.0/>.

// Common exceptions

#pragma once

#include <cassert>

namespace libpt {

class OSException {

public:
    OSException(int errno) : Errno(errno) {}

    int GetErrno() const { assert(this); return Errno; }

private:
    int Errno;

};  // class OSException

class FileException {

public:
    FileException(const char *source) : Source(source) {}

    const char *GetSource() const { assert(this); return Source; }

private:
    const char *Source;

};  // class FileException

class IOException : public OSException, public FileException {

public:
    IOException(const char *source, int errno_value)
      : OSException(errno_value), FileException(source) {}

};  // class IOException

class FileOpenException : public IOException {

public:
    FileOpenException(const char *source, int errno_value)
      : IOException(source, errno_value) {}

};  // class FileOpenException

class EOFException : public FileException {

public:
    EOFException(const char *source) : FileException(source) {}

};  // class EOFException

}   // namespace libpt
