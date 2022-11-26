/* Mamba Python by Vladyslav Korol | Apache License 2
 * This file represents the string standard module that
 * processes textual data in Python. 
 * Source: docs.python.org/3/library/string.html */
#pragma once
#include <vector>
#include "./Interpreter/Base.hh"
namespace mamba::string {
    const char ascii_letters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char ascii_lowercase[] = "abcdefghijklmnopqrstuvwxyz";
    const char ascii_uppercase[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char digits[] = "0123456789";
    const char hexdigits[] = "0123456789abcdefABCDEF";
    const char octdigits[] = "01234567";
    const char punctuation[] = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
    const char printable[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~ \t";
    const char whitespace[] = " \t";

    class Formatter {
        public:
            void format(mamba::String& formatString);
            void vformat(mamba::String& formatString);
            std::vector<std::tuple<const char*>> parse(mamba::String& formatString);
            std::pair<mamba::Bitset, mamba::String> getField(mamba::String& formatString);
            const char* getValue(int key);
            const char* getValue(mamba::String& key);
            //To add: convertField, formatField
    };

    class Template {
        public:
            mamba::String templateString;
            Template(mamba::String& templateString);
            void substitute(mamba::Dictionary& mapping);
            void safeSubstitute(mamba::Dictionary& mapping);
            bool isValid();
            mamba::Bitset getIdentifiers();
    };

    /// @brief Splits the string, capitialises every word and joins them together.
    /// @param string The input string to process.
    /// @param sep The seperator to divide by.
    /// @return The amalgamation of all cap words.
    mamba::String capwords(mamba::String& string, mamba::String& sep = " ");

};