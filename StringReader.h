// libpt: Plain Text Manipulation
// Written in 2015 by Jordan Vaughan

// To the extent possible under law, the author(s) have dedicated all copyright
// and related and neighboring rights to this software to the public domain
// worldwide. This software is distributed without any warranty.

// You should have received a copy of the CC0 Public Domain Dedication along
// with this software. If not, see
// <http://creativecommons.org/publicdomain/zero/1.0/>.

// This header defines classes for reading characters from C and C++ strings.
// libpt can use these in templated parsing functions.

#pragma once

#include <Exceptions.h>

namespace libpt {

// Instances of this class read characters from C++ strings.  They don't own
// the strings: Others handle their destruction.
//
// String must be a class that has the following public fields and members,
// which should be accessible when String objects are constant:
//
//     * value_type, which is the type of characters in the String
//     * const_iterator, the type of forward iterators over the String
//       (supports iterator comparison via operator==, retrieving the
//       current value via unary operator*, and advancing the iterator
//       via postincrement operator++)
//     * begin, a method returning a const_iterator starting at the beginning
//       of the String
//     * end, a method returning a const_iterator representing the end of
//       the String
//
// The lengths of Strings shouldn't be modified while StringReaders are
// using them.
template <typename String>
class StringReader {

public:
    StringReader() = delete;
    StringReader(const String &backing_string) : BackingString(backing_string) {
        assert(this);
        assert(&backing_string);
        Current = backing_string.begin();
        End = backing_string.end();
    }

    inline typename String::value_type ReadChar() {
        assert(this);
        if (Current == End) {
            throw EOFException(nullptr);
        }
        return *Current++;
    }

    inline bool IsEOF() { assert(this); return Current == End; }
    inline bool Error() { assert(this); return false; }

    inline void Rewind() {
        assert(this);
        Current = BackingString.begin();
    }

private:
    const String &BackingString;
    typename String::const_iterator Current;
    typename String::const_iterator End;

};  // class StringReader

// Instances of this class read characters from C strings.  They don't own
// the strings: Others handle their destruction.  Their interface is identical
// to that of StringReader except that their constructor takes a pointer
// instead of a reference.
class CStringReader {

public:
    CStringReader() = delete;
    CStringReader(const char *backing_string) : BackingString(backing_string),
      Current(backing_string) {
        assert(this);
        assert(backing_string);
    }

    inline char ReadChar() {
        assert(this);
        if (*Current == '\0') {
            throw EOFException(nullptr);
        }
        return *Current++;
    }

    inline bool IsEOF() { assert(this); return *Current == '\0'; }
    inline bool Error() { assert(this); return false; }

    inline void Rewind() {
        assert(this);
        Current = BackingString;
    }

private:
    const char *BackingString;
    const char *Current;

};  // class CStringReader

}   // namespace libpt
