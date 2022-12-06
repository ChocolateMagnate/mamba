/* Mamb Python by Vladyslav Korol | Apache License 2
 * This file represents the regular expression module available
 * in Python standard library.
 * Source: docs.python.org/3/library/re.html */
#pragma once
#include <regex>
#include "./Interpreter/Base.hh"
namespace mamba::re {
    /// @brief The forwaded regex constants.
    enum class RegexFlag {
        basic = std::regex_constants::basic,
        extended = std::regex_constants::extended,
        icase = std::regex_constants::icase,
        nosubs = std::regex_constants::nosubs,
        optimize = std::regex_constants::optimize,
        collate = std::regex_constants::collate,
        ECMAScript = std::regex_constants::ECMAScript,
        awk = std::regex_constants::awk,
        grep = std::regex_constants::grep,
        egrep = std::regex_constants::egrep
    };

    class Match {
        public:
            mamba::String string; //The string to match.

            mamba::String expand(mamba::String& templateString);
            mamba::String group(int index);
            std::tuple<mamba::String> group(unsigned int* groups, int groupCount);
            mamba::String operator[](int index);
            std::tuple<const char*, const char*> groups(bool _default = false);
            mamba::Dictionary groupdict(mamba::Dictionary& _default);
            int start(int group);
            int end(int group);
            std::pair<int, int> span(int group);
            unsigned int pos();
            unsigned int endpos();
            mamba::Bitset lastindex();
    };

    /// @brief The pattern class used to represent a single regex pattern.
    class Pattern {
        public:
            std::regex pattern;
            std::vector<const char*> flags, groups;
            mamba::Dictionary groupindex;

            Pattern(mamba::String& pattern, RegexFlag flags = RegexFlag::ECMAScript);
            bool match(mamba::String& string);
            mamba::re::Match search(mamba::String& string);
            mamba::re::Match fullmatch(mamba::String& string);
            mamba::String sub(mamba::String& repl, mamba::String& string, int count = 0);
            mamba::String subn(mamba::String& repl, mamba::String& string, int count = 0);
            mamba::String split(mamba::String& string, int maxsplit = 0);
            mamba::String findall(mamba::String& string, unsigned int pos, unsigned int endpos);
            mamba::String finditer(mamba::String& string);
            mamba::String escape(mamba::String& string);
    };
    /// @brief Generates a regex pattern.
    /// @param pattern The regular expression pattern.
    /// @param flags Various flags to use.
    /// @return The Match object.
    mamba::re::Match compile(mamba::String& pattern, RegexFlag flags = RegexFlag::ECMAScript);
    
    unsigned int search(mamba::String& pattern, mamba::String& string, RegexFlag flags = RegexFlag::ECMAScript);
    
    mamba::re::Match match(mamba::String& pattern, mamba::String& string, RegexFlag flags = RegexFlag::ECMAScript);
    
    mamba::re::Match fullmatch(mamba::String& pattern, mamba::String& string, RegexFlag flags = RegexFlag::ECMAScript);
    
    mamba::String* split(mamba::String& pattern, mamba::String& string, int maxsplit = 0, RegexFlag flags = RegexFlag::ECMAScript);
    
    mamba::String* findall(mamba::String& pattern, mamba::String& string, RegexFlag flags = RegexFlag::ECMAScript);
    /// @brief Yields the matches of the pattern in the string.
    /// @param pattern The pattern to search for.
    /// @param string The input string to inspect.
    /// @param flags Various flags to use.
    /// @return Python generator containing all the matches as chararrays.
    mamba::Generator finditer(mamba::String& pattern, mamba::String& string, RegexFlag flags = RegexFlag::ECMAScript);
    
    mamba::String sub(mamba::String& pattern, mamba::String& repl, mamba::String& string, int count = 0, RegexFlag flags = RegexFlag::ECMAScript);
    /// @brief 
    /// @param pattern 
    /// @param repl 
    /// @param string 
    /// @param count 
    /// @param flags 
    /// @return 
    std::pair<mamba::String, unsigned int> subn(mamba::String& pattern, mamba::String& repl, mamba::String& string, int count = 0, RegexFlag flags = RegexFlag::ECMAScript);
    
    /// @brief Escape special characters in the pattern.
    /// @param pattern The pattern to escape.
    void espace(mamba::String& pattern);

    /// @brief Clear the regular expression cache.
    void purge();
};