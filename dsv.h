/* A Unix-Style Delimiter-Separated Values (DSV) Parser
 * Written in 2015 by Jordan Vaughan
 *
 * To the extent possible under law, the author(s) have dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide. This software is distributed without any warranty.
 *
 * You should have received a copy of the CC0 Public Domain Dedication along
 * with this software. If not, see
 * <http://creativecommons.org/publicdomain/zero/1.0/>.
 */

// This is a parser for DSV files as described in chapter five, "Textuality",
// of Eric Steven Raymond, "The Art of Unix Programming" (Boston: Addison-
// Wesley), 2003.

#pragma once

#include <algorithm>
#include <cassert>
#include <istream>
#include <string>

namespace PlainText {

	// This class provides Escape and Separator fields required
	// by DSVParser in its Callbacks type parameter so that '\\' is
	// the escape character and ':' is the field separator.  This is
	// widespread on UNIX systems.
	template <typename _CharT>
	class UnixDSVParser {

		public:

		static const _CharT Escape = '\\';
		static const _CharT Separator = ':';

	};	// class UnixDSVParser

	// This class implements the OnFieldCharacter method required
	// by DSVParser in its Callbacks type parameter.  This class
	// buffers the characters passed to the method in a string,
	// which subclasses can access.
	//
	// Typically, subclasses should implement all of the other
	// requirements of DSVParser's Callbacks type and use this
	// class's Field member to get a field's contents when
	// OnFieldEnd is invoked.  The same method should call ClearField
	// when it's done processing the field's text.
	//
	// For convenience, this class provides an implementation of
	// OnReset that calls ClearField.
	template <typename _CharT = char,
		  typename _CharTraits = std::char_traits<_CharT>,
		  typename _Alloc = std::allocator<_CharT> >
	class DSVStringFieldBuffer {

		public:

		// string type used to buffer field text
		typedef std::basic_string<_CharT, _CharTraits, _Alloc> TField;

		DSVStringFieldBuffer() { assert(this); }
		DSVStringFieldBuffer(const DSVStringFieldBuffer &that) :
		    Field(that.Field) {
			assert(this);
			assert(&that);
		}

		DSVStringFieldBuffer(DSVStringFieldBuffer &&that) :
		    DSVStringFieldBuffer() {
			assert(this);
			assert(&that);
			std::swap(Field, that.Field);
		}

		DSVStringFieldBuffer &operator=(
		    const DSVStringFieldBuffer &that) = delete;

		DSVStringFieldBuffer &operator=(DSVStringFieldBuffer &&that) {
			assert(this);
			assert(&that);
			std::swap(Field, that.Field);
			return *this;
		}

		// Hook invoked by DSVParser when it parses a field's character
		void OnFieldCharacter(_CharT c) {
			assert(this);
			Field.push_back(c);
		}

		// Hook invoked by DSVParser when the parser is reset.
		// This implementation simply calls ClearField.
		void OnReset() {
			assert(this);
			ClearField();
		}

		// Clear the field buffer.  Subclasses should invoke this
		// in their implementations of OnFieldEnd.
		void ClearField() {
			assert(this);
			Field.clear();
		}

		protected:

		// the buffer for field characters
		TField Field;

	};	// class DSVStringFieldBuffer

	// This class parses delimiter-separated values in text streams.
	// The format is specified in the text mentioned at the beginning
	// of this file.
	//
	// Callbacks must be a class or struct that defines the following:
	//
	//     * a default constructor
	//     * a copy constructor
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
	//     * a _CharT called Separator, which represents a single field
	//       separator character
	//     * a _CharT called Escape, which represents a single escape
	//       character
	//
	template<typename Callbacks, typename _CharT = char>
	class DSVParser {

		public:

		DSVParser() : Hooks(), Escaping(false), InField(false) {
			assert(this);
		}

		DSVParser(const Callbacks &hooks) : Hooks(hooks),
						    Escaping(false),
						    InField(false) {
			assert(this);
		}

		DSVParser(const DSVParser &) = delete;

		DSVParser(DSVParser &&that) : DSVParser() {
			assert(this);
			assert(&that);
			std::swap(Hooks, that.Hooks);
			std::swap(Escaping, that.Escaping);
			std::swap(InField, that.InField);
		}

		~DSVParser() {
			assert(this);
		}

		DSVParser operator=(const DSVParser &) = delete;

		DSVParser &operator=(DSVParser &&that) {
			assert(this);
			assert(&that);
			std::swap(Hooks, that.Hooks);
			std::swap(Escaping, that.Escaping);
			std::swap(InRecord, that.InRecord);
			return *this;
		}

		template <typename _Traits>
		void Parse(std::basic_istream<_CharT, _Traits> &stream) {
			assert(&stream);
			_CharT c;
			for (stream.get(c); !stream.fail(); stream.get(c)) {
				if (Escaping) {
					Hooks.OnFieldCharacter(c);
					Escaping = false;
				} else {
					if (!InRecord && c != '\n') {
						InRecord = true;
						Hooks.OnRecordStart();
					}
					if (c == GetSeparator()) {
						Hooks.OnFieldEnd();
					} else if (c == GetEscape()) {
						Escaping = true;
					} else if (c == '\n') {
						if (InRecord) {
							Hooks.OnFieldEnd();
							InRecord = false;
							Hooks.OnRecordEnd();
						}
					} else {
						Hooks.OnFieldCharacter(c);
					}
				}
			}
		}

		// Get the callbacks object.
		Callbacks &GetCallbacks()
		{
			assert(this);
			return Hooks;
		}

		const Callbacks &GetCallbacks() const
		{
			assert(this);
			return Hooks;
		}

		// Get the escape character.
		_CharT GetEscape() const {
			assert(this);
			return Hooks.Escape;
		}

		// Get the field separator character.
		_CharT GetSeparator() const {
			assert(this);
			return Hooks.Separator;
		}

		// Make the parser think it's at the beginning of a stream
		// of data.
		void Reset() {
			assert(this);
			Escaping = InRecord = false;
			Hooks.OnReset();
		}

		private:

		// true if the parser just parsed an unescaped escape character
		bool Escaping;

		// true if the parser is in the middle of a field
		bool InField;

		// true if the parser is in the middle of a record
		bool InRecord;

		// parser callbacks
		Callbacks Hooks;

	};	// class DSVParser

}	// namespace PlainText

