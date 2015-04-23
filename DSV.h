// libpt: Plain Text Manipulation
// Written in 2015 by Jordan Vaughan

// To the extent possible under law, the author(s) have dedicated all copyright
// and related and neighboring rights to this software to the public domain
// worldwide. This software is distributed without any warranty.

// You should have received a copy of the CC0 Public Domain Dedication along
// with this software. If not, see
// <http://creativecommons.org/publicdomain/zero/1.0/>.

// This is a parser for DSV files as described in chapter five, "Textuality",
// of Eric Steven Raymond, "The Art of Unix Programming" (Boston: Addison-
// Wesley), 2003.

#pragma once

#include <algorithm>
#include <cassert>
#include <string>
#include <Exceptions.h>

namespace libpt {

// This class parses delimiter-separated values in text streams.
// The format is specified in the text mentioned at the beginning
// of this file.
//
// This class uses the curiously recurring template pattern (CRTP).
// Derived must be a descendant class that defines the following:
//
//     * a method called OnRecordStart taking no parameters,
//       which is invoked when a new record is being parsed
//     * a method called OnFieldCharacter taking a single _CharT,
//       which is invoked when the parser parses a character within
//       a record's field
//     * a method called OnFieldEnd taking no parameters,
//       which is invoked when the parser is finished parsing a field
//     * a method called OnRecordEnd taking no parameters,
//       which is invoked when the parser is done parsing a record
//     * a method called OnReset taking no parameters,
//       which is invoked when the parser is reset
//     * a method called GetSeparator taking no parameters, which
//       returns the DSV separator character as a _CharT
//     * a method called GetEscape taking no parameters, which
//       returns the DSV escape character as a _CharT
//
// All of these methods must be publicly accessible unless Derived declares
// DSVParser<Derived> a friend class.
template <typename Derived, typename _CharT = char>
class DSVParser {

public:
    typedef _CharT CharT;

    DSVParser() : Escaping(false), InRecord(false) {
        assert(this);
    }

    DSVParser(const DSVParser &that) {
        assert(this);
        assert(&that);
        Copy(that);
    }

    ~DSVParser() {
        assert(this);
        FinishParsing();
        Escaping = InRecord = false;
    }

    DSVParser &operator=(const DSVParser &that) {
        assert(this);
        assert(&that);
        Copy(that);
        return *this;
    }

    // Feed the parser a single character.
    void FeedCharacter(CharT c) {
        assert(this);
        HandleParsedCharacter(c);
    }

    // Call this when you finish parsing.  The destructor will do this
    // for you automatically.
    void FinishParsing() {
        assert(this);
        if (InRecord) {
            AsDerived()->OnFieldEnd();
            Escaping = InRecord = false;
            AsDerived()->OnRecordEnd();
        }
    }

    // Parse the specified stream.  This does not call FinishParsing.
    // Reader must be a reader class that implements at least ReadChar
    // and IsEOF.  FileReader is an example of an acceptable reader.
    template <typename Reader>
    void Parse(Reader &reader) {
        assert(this);
        assert(&reader);
        CharT c;
        try {
            while (!reader.IsEOF()) {
                HandleParsedCharacter(reader.ReadChar());
            }
        } catch (EOFException e) {}
    }

    // Parse the specified stream and call FinishParsing.
    // See Parse for Reader requirements.
    template <typename Reader>
    void ParseOnly(Reader &reader) {
        assert(this);
        assert(&reader);
        Parse(reader);
        FinishParsing();
    }

    // Make the parser think it's at the beginning of a stream
    // of data.
    void Reset() {
        assert(this);
        Escaping = InRecord = false;
        AsDerived()->OnReset();
    }

private:
    Derived *AsDerived() { return static_cast<Derived *>(this); }

    void HandleParsedCharacter(CharT c) {
        assert(this);
        if (Escaping) {
            AsDerived()->OnFieldCharacter(c);
            Escaping = false;
        } else {
            if (!InRecord && c != '\n') {
                InRecord = true;
                AsDerived()->OnRecordStart();
            }
            if (c == AsDerived()->GetSeparator()) {
                AsDerived()->OnFieldEnd();
            } else if (c == AsDerived()->GetEscape()) {
                Escaping = true;
            } else if (c == '\n') {
                if (InRecord) {
                    AsDerived()->OnFieldEnd();
                    InRecord = false;
                    AsDerived()->OnRecordEnd();
                }
            } else {
                AsDerived()->OnFieldCharacter(c);
            }
        }
    }

    void Copy(const DSVParser &that) {
        assert(this);
        assert(&that);
        Escaping = that.Escaping;
        InRecord = that.InRecord;
    }

    // true if the parser just parsed an unescaped escape character
    bool Escaping;

    // true if the parser is in the middle of a record
    bool InRecord;

};    // class DSVParser

// This class provides the methods required by DSVParser as pure virtual
// functions.  Derive from this class if you need run-time polymorphism.
template <typename _CharT = char>
class DynamicDSVParser : public DSVParser<DynamicDSVParser<_CharT>, _CharT> {

public:
    virtual void OnRecordStart() = 0;
    virtual void OnFieldCharacter(_CharT c) = 0;
    virtual void OnFieldEnd() = 0;
    virtual void OnRecordEnd() = 0;
    virtual void OnReset() = 0;
    virtual _CharT GetEscape() const = 0;
    virtual _CharT GetSeparator() const = 0;

};    // class DynamicDSVParser

// This class provides GetEscape and GetSeparator methods that return
// '\\' and ':', respectively.  These are the escape and separator
// characters that are most commonly used in UNIX DSV files.
template <typename _CharT = char>
class UnixDSVParser {

public:
    static const _CharT Escape = '\\';
    static const _CharT Separator = ':';

    _CharT GetEscape() const { assert(this); return Escape; }
    _CharT GetSeparator() const { assert(this); return Separator; }

};    // class UnixDSVParser

}    // namespace PlainText
