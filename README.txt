libpt

Introduction:

    libpt is a collection of C++ parsers for some of the most common
    plain text formats.  Instructions are in the individual header files.

Available Parsers:

    Delimiter-Separated Values (DSV) -- DSV.h

        This provides a parser and auxiliary classes for DSV files as
        described in chapter five, "Textuality", of Eric Steven Raymond,
        "The Art of Unix Programming" (Boston: Addison-Wesley), 2003.

        DSV is a simple record format similar to comma-separated values (CSV)
        files.  Escaping is much simpler and fields containing whitespace
        don't need to be quoted.  UNIX's /etc/passwd is an example of a
        DSV file.

Copyright:

    Copyright?  Hah!  Here's my "copyright":

        This library was written in 2013 by Jordan Vaughan.

        To the extent possible under law, the author(s) have dedicated all
        copyright and related and neighboring rights to this software to the
        public domain worldwide. This software is distributed without any
        warranty.

        You should have received a copy of the CC0 Public Domain Dedication
        along with this software. If not, see
        http://creativecommons.org/publicdomain/zero/1.0/.

    A full copy of the public domain dedication is in COPYING.

