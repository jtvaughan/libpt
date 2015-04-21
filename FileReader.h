// libpt: Plain Text Manipulation
// Written in 2015 by Jordan Vaughan

// To the extent possible under law, the author(s) have dedicated all copyright
// and related and neighboring rights to this software to the public domain
// worldwide. This software is distributed without any warranty.

// You should have received a copy of the CC0 Public Domain Dedication along
// with this software. If not, see
// <http://creativecommons.org/publicdomain/zero/1.0/>.

// This header defines a class for reading characters from C FILEs.
// libpt parsers can use it in templated parsing functions.

#pragma once

#include <cstdio>
#include <Exceptions.h>

namespace libpt {

// Instances of this class read bytes from stdio FILEs.  They don't own the
// FILEs (i.e., they don't open or close them): Their management is left
// to others.
class FileReader {

public:
    FileReader() = delete;
    FileReader(FILE *file) : File(file) {
        assert(file);
    }

    inline char ReadChar() {
        assert(this);
        auto c = getc(File);
        if (c == -1) {
            throw EOFException(nullptr);
        }
        return c;
    }

    inline bool IsEOF() { assert(this); return feof(File) != 0; }
    inline bool Error() { assert(this); return ferror(File) != 0; }

private:
    FILE *File;

};  // class FileReader

}   // namespace libpt
