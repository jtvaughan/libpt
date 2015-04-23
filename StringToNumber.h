// libpt: Plain Text Manipulation
// Written in 2015 by Jordan Vaughan

// To the extent possible under law, the author(s) have dedicated all copyright
// and related and neighboring rights to this software to the public domain
// worldwide. This software is distributed without any warranty.

// You should have received a copy of the CC0 Public Domain Dedication along
// with this software. If not, see
// <http://creativecommons.org/publicdomain/zero/1.0/>.

// This is a collection of functions for converting strings to numbers.
// The standard C and C++11 libraries provide functions to do this, but
// none of them are templated.  This header solves that problem.

#pragma once

#include <cinttypes>
#include <cstring>

namespace libpt {

// This is a templated form of the standard C strto* functions.
// See their man pages for details.  Specializations also exist for
// short and unsigned short.  Note that floating point conversions
// ignore the base parameter.
template<typename T> T StringToNumber(const char *str,
  size_t *end_idx = nullptr, int base = 10) {
    return StringToNumber<unsigned long long>(str, end_idx, base);
}

// This is a templated form for C++ string classes that provide a
// c_str method returning a null-terminated C string.  This uses
// the C strto* functions.
template<typename T, typename String> T StringToNumber(const String &str,
  size_t *end_idx = nullptr, int base = 10) {
    return StringToNumber<T>(str.c_str(), end_idx, base);
}


// Specializations for the C string versions

template<> long double StringToNumber(const char *str, size_t *end_idx,
  int base) {
    char *end;
    auto val = strtold(str, &end);
    if (end_idx) {
        *end_idx = (size_t)(uintptr_t)(end - str);
    }
    return val;
}

template<> double StringToNumber(const char *str, size_t *end_idx, int base) {
    char *end;
    auto val = strtod(str, &end);
    if (end_idx) {
        *end_idx = (size_t)(uintptr_t)(end - str);
    }
    return val;
}

template<> float StringToNumber(const char *str, size_t *end_idx, int base) {
    char *end;
    auto val = strtof(str, &end);
    if (end_idx) {
        *end_idx = (size_t)(uintptr_t)(end - str);
    }
    return val;
}

template<> long long StringToNumber(const char *str, size_t *end_idx,
  int base) {
    char *end;
    auto val = strtoll(str, &end, base);
    if (end_idx) {
        *end_idx = (size_t)(uintptr_t)(end - str);
    }
    return val;
}

template<> unsigned long long StringToNumber(const char *str, size_t *end_idx,
  int base) {
    char *end;
    auto val = strtoull(str, &end, base);
    if (end_idx) {
        *end_idx = (size_t)(uintptr_t)(end - str);
    }
    return val;
}

template<> long StringToNumber(const char *str, size_t *end_idx, int base) {
    char *end;
    auto val = strtol(str, &end, base);
    if (end_idx) {
        *end_idx = (size_t)(uintptr_t)(end - str);
    }
    return val;
}

template<> unsigned long StringToNumber(const char *str, size_t *end_idx,
  int base) {
    char *end;
    auto val = strtoul(str, &end, base);
    if (end_idx) {
        *end_idx = (size_t)(uintptr_t)(end - str);
    }
    return val;
}

template<> int StringToNumber(const char *str, size_t *end_idx, int base) {
    return StringToNumber<long>(str, end_idx, base);
}

template<> unsigned int StringToNumber(const char *str, size_t *end_idx,
  int base) {
    return StringToNumber<unsigned long>(str, end_idx, base);
}

template<> short StringToNumber(const char *str, size_t *end_idx, int base) {
    return StringToNumber<long>(str, end_idx, base);
}

template<> unsigned short StringToNumber(const char *str, size_t *end_idx,
  int base) {
    return StringToNumber<unsigned long>(str, end_idx, base);
}

}   // namespace libpt
