/*+================================================================================================
  File:        String.hh

  Summary:     Represents Pythonic Unicode-aware strings.

  Constants:   NumberOfOutposts (10)

  Classes:     StringEncodingPolicy, Outpost, UTF8Walker, String

  Functions:   None

  Available under Apache License v2. Mamba Authors (2023)
================================================================================================+*/
#pragma once

#include <cstddef>
#include <array>

#include "interfaces.hh"
#include "Dictionary.hh"
#include "Integer.hh"
#include "Tuple.hh"
#include "Bytes.hh"
#include "List.hh"
#include "../include/givers/memory.hh"
#include "help/unicode.hh"

namespace mamba {
  const auto NumberOfOutposts = 10;

  /// Outposts reference points used by string to accelerate the performance of random-access
  /// in the variable-length encoding. They can be thought as tables that map contiguous intervals
  /// of grapheme indexes with their corresponding indexes in the byte array relative from the start.
  /// @note As a part of the optimisation efforts, this structure uses unsigned 32-bit integers to represent
  /// both indexes. As result, outposts are efficient and effectively reduce the random-access overhead for
  /// the 32-bit destination space, which is slightly more than 4 gigabytes. For strings larger than that size,
  /// outposts are useless and random-access is performed by sequentially walking until the specified index.
  struct Outpost {
    uint32_t index, destination;

    Outpost();
    explicit Outpost(uint32_t inputIndex);
    Outpost& operator=(int base) noexcept;
    bool operator==(const Outpost& other) const noexcept;
  };

  /// The iterator type for the UTF-8 Unicode characters.
  class UTF8Walker {
   public:
    UTF8Walker() = delete;
    explicit(false) UTF8Walker(const char* destination); //NOLINT

    void operator++();
    bool operator==(const UTF8Walker& other);
    bool operator<(const UTF8Walker& other);
    char operator*();

    /// Writes the current grapheme into the buffer.
    /// @param destination The pointer to the character buffer where to copy
    /// the grapheme to. It should contain at least 4 free char pool.
    void write(char* destination);

   private:
    const char* data;
  };

  /// Pythonic strings as denoted by the str datatype. mamba::String objects
  /// act as the containers that store dynamically-resizable arrays of char
  /// values on the heap GarbageCollectedStack pools, while also providing glyph-based random
  /// access. Strings are always guaranteed to be UTF-8. String objects own
  /// the data and each object manages its own underlying buffer. Therefore,
  /// this type should always be passed by reference to avoid unnecessary
  /// growths and shrinks.
  class String {
   public:
    String();
    String(const String& other);
    String(String&& other) noexcept;

    /// Initialises the string with an array of characters trailed with \\0.
    /// This constructor is used to initialise strings from default compile-time
    /// known values and to initialise a string from a buffer given by C interface.
    /// @param character The pointer to the start of the character array.
    /// @warning The constructor assumes valid pointer. If the constructor is invoked
    /// from a pointer returned from C interface, it should be checked and the appropriate
    /// exception (OSError, ConnectionAbortedError, MemoryError, etc.) should be raised.
    explicit(false) String(const char* character) noexcept;                //NOLINT

    /// Initialises the string from another existing sequence of characters or its slice.
    /// This overload primarily exists to construct strings from hard-coded values from the
    /// source file in assignment like `text = "Hello world!"` since the char* constructor
    /// requires a zero-byte terminator in the end.
    /// @param slice The source (sub)string to initialise.
    explicit(false) String(const std::string_view& slice) noexcept;       //NOLINT

    explicit(false) String(const GarbageCollected<char>& address) noexcept; //NOLINT

    /// Converts the integer into its text representation.
    /// @note Following the CPython implementation, Mamba restricts int-to-str
    /// conversion up to 4300 digits and raises ValueError otherwise. The limit
    /// is used to prevent DOS attacks when converting large integers and can be
    /// configured with sys.set_int_max_str_digits() or with the PYTHONINTMAXSTRDIGITS
    /// environmental variable. Refer to the link for more details.
    /// @see https://docs.python.org/3/library/stdtypes.html#integer-string-conversion-length-limitation
    /// @param integer The integer to convert.
    explicit String(const Integer& integer);

    String&  operator=(const char* character) noexcept;
    String&  operator=(const std::string_view& other) noexcept;
    String&  operator=(const String& other) noexcept = default;
    String&  operator=(String&& other) noexcept;
    String   operator+(const String& other) const noexcept;
    String&  operator+=(const String& other) noexcept;
    String   operator*(size_t times) const noexcept;
    String   operator[](size_t index) noexcept;
    String   operator[](const Range& range) noexcept;
    String   operator%(const Enumerable& placeholders) const noexcept;

    /// The boolean comparison operators evaluate 2 strings for being lexicographically ordered.
    /// If strings have different sizes, then lexicographically greater is the longer size and vice versa,
    /// and for strings of even size, we need to find the first differing pair of graphemes and cast them into
    /// Unicode code points and compare them. This behaviour is consistent with the equals operator as it checks
    /// if the strings are the same.
    bool operator==(const String& other) const noexcept;
    bool operator>(const String& other) const noexcept;
    bool operator<(const String& other) const noexcept;
    bool operator<=(const String& other) const noexcept;
    bool operator>=(const String& other) const noexcept;

    /// Provides the gateway to the underlying array of characters
    /// that compose the string. The getter should be used carefully
    /// and thread-safely as direct modifications on the string may
    /// corrupt the entire container and produce incorrect behaviour.
    /// @return Pointer to the character array (char*) managed by string.
    [[nodiscard]] char* data() const noexcept;

    [[nodiscard]] const char* begin() const noexcept;
    [[nodiscard]] const char* end() const noexcept;

    [[nodiscard]] bool in(const String& haystack) const noexcept;
    [[nodiscard]] size_t len() const;

    /// Reserves designated size for the strings.
    /// @param size The expected size of the string in bytes.
    /// @return GarbageCollected to self.
    String& reserve(size_t size) noexcept;

    /// Unicode character iterator through the string. It yields
    /// glyphs as long as there are glyphs in the string and
    /// support conversion between different Unicode formats.
    /// @return Unicode iterator over the string object.
    [[nodiscard]] UTF8Walker iter() const;

    /// Converts all characters to the upper case and returns a copy.
    /// @return A copy of the string with all applicable characters in upper case.
    String capitalize() const noexcept;

    /// Converts all characters in lower case. This method is used as opposed to
    /// lowercase() in order to signal the intention to compare multiple strings
    /// with varying cases.
    /// @return A new string with all characters lowercase.
    String casefold() const noexcept;

    /// Generates a new string with this one being in the center
    /// and surrounded by padding characters.
    /// @param count The size of the final string.
    /// @param padding (optional) The character to fill the space.
    /// By default, whitespaces.
    /// @return A new centered string.
    String center(const size_t count, const String& padding = " ") const noexcept;

    /// Encodes the string into an array of bytes.
    /// @param encoding (optional) The string specifying the encoding to be used, default UTF-8.
    /// @param policy (optional) The policy to use if errors occurred during encoding, default strict.
    /// @return Bytes array representing the string encoded into the specified encoding.
    Bytes encode(const String& encoding = "UTF-8",
                 const StringEncodingPolicy policy = StringEncodingPolicy::Strict) const noexcept;

    /// Tells if the string ends with the specified string.
    /// @param ending The string to check if this string ends with.
    /// @param start (optional) The index at which to begin searching in the string.
    /// @param end (optional) The index at which to end searching the string.
    /// @return True if the string fragment ends with the ending string, false otherwise.
    bool endswith(const String& ending, const size_t start = 0, const size_t end = SIZE_MAX) const noexcept;

    /// Generates a new string where the tab characters \\t are replaced with whitespaces.
    /// @param size (optional) The number of whitespaces to insert for a single tab, default 8.
    /// @return Expanded string. If no tab characters are present, a copy-on-write reference is returned.
    String expandtabs(size_t size = 8) const noexcept;

    /// Searches for the specified value in the string if it's present.
    /// @param needle The string to search for in this string.
    /// @param start (optional) The position to start searching the string from.
    /// @param end (optional) The position to end searching string to.
    /// @return The index where the seeked string is located, -1 if it's not present.
    [[nodiscard]] ssize_t find(const String& needle, size_t start = 0, const size_t end = SIZE_MAX) const noexcept;

    /// Substitutes the placeholders in the string ({name}, {0}, {}, etc.) with their corresponding
    /// values in the enumerable. Placeholders can be inserted by index or keyword.
    /// @param placeholders The values to insert in the string.
    /// @return New formatted string.
    String format(const Enumerable& placeholders) const noexcept;

    /// Searches for the value in the string and raises ValueError if not found.
    /// @param value The string to search for.
    /// @param start (optional) The index from which to search the string.
    /// @param end (optional) The index from which to end searching string.
    /// @return The index where the value begins, or raises ValueError if not found.
    size_t index(const String& value, const size_t start = 0, const size_t end = SIZE_MAX) const noexcept;

    /// Tells if the string only contains ASCII letters (A-Za-z) and digits (0-9).
    /// @return True if all glyphs in the string are alphanumeric, false otherwise.
    bool isalum() const noexcept;

    /// Tells if the string only contains ASCII alphabet characters (A-Za-z).
    /// @return True if all glyphs in the string are alphabetic, false otherwise.
    bool isalpha() const noexcept;

    /// Tells if the string only contains ASCII characters.
    /// @return True if all glyphs in the string are ASCII, false otherwise.
    bool isascii() const noexcept;

    /// Tells if all characters in the string represent valid digits in any
    /// Unicode notation. Can be used on ASCII digits 0-9 and other Unicode characters.
    /// @return True if all characters represent a valid decimal value, false otherwise.
    bool isdecimal() const noexcept;

    /// Tells if all characters in the string are ASCII digits from 0 to 9.
    /// @return True if all characters are 0-9, false otherwise.
    bool isdigit() const noexcept;

    /// Tells if the string can be used as an identifier across most programming languages.
    /// Ids can only consist from alphabetic characters, digits, and underscores, but cannot
    /// contain spaces or begin with digits. Unicode characters are also allowed.
    /// @return True if the entire string is a valid identifier, false otherwise.
    bool isidentifier() const noexcept;

     /// Tells if all characters in the string are either lowercase or does not have cases.
     /// @return False if the string contains a single uppercase glyph, true otherwise.
     bool islower() const noexcept;

     /// Tells if all the characters in the string represent a number in one way or another.
     /// Any Unicode decimals, fractions like ¾ or exponents like ² are considered numeric,
     /// but fractions like 2.5 are not, since they contain the dot (.), which does not denote
     /// a number.
     /// @return True if all characters are numeric, false otherwise.
    bool isnumeric() const noexcept;

    /// Tells if the entire string can be displayed as it's written in the source code.
    /// String is printable if it does not contain special characters that transform it,
    /// such as newline (\\n), carriage return (\\r) or tabs (\\t). Raw strings are always
    /// printable.
    /// @return True if the whole string is printable, false otherwise.
    bool isprintable() const noexcept;

    /// Tells if the whole string is only composed from space characters like whitespace,
    /// tabs, newlines or carriage returns.
    /// @return False if the string contains a single non-space character, true otherwise.
    bool isspace() const noexcept;

    /// Tells if the string follows the title naming convention where all words are lower-case
    /// but start with an uppercase letter.
    /// @return True if the string is a title, false otherwise.
    bool istitle() const noexcept;

    /// Tells if the string is either upper-case or contains non-case characters.
    /// @return False if the string a single lower-case character, true otherwise.
    bool isupper() const noexcept;

    /// Generates a new string where each item is converted into a string, concatenated
    /// together and separated with this string as a separator.
    /// @param items The enumerable of items to join.
    /// @return A new joint string.
    String join(Enumerable items) const noexcept;

    /// Aligns the string to the left and fills with gap with the padding string.
    /// @param size The size of the final string.
    /// @param padding (optional) The character to fill the excessive size with, default whitespace.
    /// @return New left-adjusted string.
    String ljust(const size_t size, const String& padding = " ") const noexcept;

    /// Converts the whole string into lowercase.
    /// @return A new string with all characters into the lowercase. If the string did not
    /// contain lowercase characters, a copy-on-write reference is returned.
    String lower() const noexcept;

    /// Removes the set of characters from the start of the string until a character
    /// that is not present in the set is encountered.
    /// @param glyphsToRemoveSet Set of characters to remove from the string's start.
    /// Each glyph in the string is treated as a separate character and it does not match
    /// the string directly but may combine the glyphs from the set in arbitrary way.
    /// @return A new left-stripped string.
    String lstrip(const String& glyphsToRemoveSet = " ") const noexcept;

    /// Generates a translation table that can be used to replace characters in the translate().
    /// @param conversion The translation table containing the key-value pairs describing the replacement.
    /// @return Translation table as a dictionary.
    static Dictionary maketrans(const Dictionary& conversion);

    /// Generates a translation table that can be used to replace characters in the translate().
    /// @param replacementSet The set of characters to replace from the string.
    /// @param replacementString A string as long as the first parameter. Every character in
    /// the replacement set will be replaced with the corresponding character in this argument by index.
    /// @param removeSet (optional) The set of characters to not include in the final string.
    /// @return Translation table as a dictionary.
    static Dictionary maketrans(const String& replacementSet,
                                const String& replacementString, const String& removeSet = "");

    /// Searches for the specified string and returns a tuple containing 3 substrings: the part before the
    /// separator, separator itself and part after it. In case if the string either begins or ends with
    /// the separator, the corresponding item in the tuple is an empty string. If separator is not found in
    /// the string, the entire original string is the first value in the tuple followed by 2 empty strings.
    /// @param separator The string to partition by.
    /// @return 3-string tuple.
    Tuple partition(const String& separator) const noexcept;

    /// Generates a new string replacing the old value with a new one specified number of times.
    /// @param target The string occurrence to replace.
    /// @param replacement The string to replace the target with.
    /// @param count (optional) The number or occurrences to replace. By default, all.
    /// @return New substituted string.
    String replace(String& target, String& replacement, size_t count = SIZE_MAX) const noexcept;

    /// Searches for the last occurrence of the string in the specified interval.
    /// @param value The value to find in the string.
    /// @param start (optional) The position from where to start searching.
    /// @param end (optional) The position from where to stop searching.
    /// @return The index of the last occurrence of the value, -1 if not found.
    size_t rfind(const String& value, const size_t start = 0, const size_t end = SIZE_MAX) const noexcept;

    /// Searches for the last occurrence of the string in the specified interval or raises ValueError.
    /// @param value The value to find in the string.
    /// @param start (optional) The position from where to start searching.
    /// @param end (optional) The position from where to stop searching.
    /// @return The index of the last occurrence of the value, raises ValueError is value was not found.
    size_t rindex(const String& value, const size_t start = 0, const size_t end = SIZE_MAX) const noexcept;

    /// Aligns the string to the right filling excessive space with the padding character.
    /// @param size The size of the final string.
    /// @param padding The character to replace the extra space on the left.
    /// @return A new right-aligned string.
    String rjust(const size_t size, const String& padding = " ") const noexcept;

    /// Searches for the last occurrence of the separator string and returns a tuple
    /// consisting from 3 strings: the part before the separator, separator itself and
    /// the part after it. If the string either begins or ends with the separator, the
    /// respective tuple element is empty string. If separator is not found in the string,
    /// the entire original string is the first value in the tuple followed by 2 empty strings.
    /// @param separator The string to search by.
    /// @return A 3-string tuple.
    Tuple rpartition(const String& separator) const noexcept;

    /// Splits the value specified number of times from the end.
    /// @param separator (optional) The value to split the string by, default whitespace.
    /// If separator cannot be found in the string, a list containing the whole string is returned.
    /// @param occurrences (optional) The number of occurrences to split. By default,
    /// it splits by all occurrences, in which case there is no difference than splitting
    /// the string with normal split(). However, when the count is specified, the behaviour
    /// diverges and splits from the end.
    /// @return A list containing substrings.
    List rsplit(const String& separator = " ", const ssize_t occurrences = -1) const noexcept;

    /// Removes all characters from the set from the right until a character that is not
    /// present in the set is encountered. All glyphs in the character set are treated as
    /// independent characters that can be freely mixed.
    /// @param glyphsToRemoveSet (optional) String containing the set of character to strip
    /// from, by default whitespace.
    /// @return A new string stripped from the right.
    String rstrip(const String& glyphsToRemoveSet = " ") const noexcept;

    /// Splits the string into multiple substrings by the separator.
    /// @param separator (optional) The string to separate the string from the start.
    /// If separator cannot be found, a list containing the whole string is returned.
    /// @param occurrences (optional) The number of separator occurrences to split by.
    /// @return A list containing split string.
    List split(const String& separator = " ", const ssize_t occurrences = -1);

    /// Splits the string into strings divided by newline.
    /// @param shouldKeepBreaklines (optional) The flag to include the newline characters
    /// (\\n) in the end of the split lines, default false.
    /// @return List of lines in the string.
    List splitlines(const bool shouldKeepBreaklines = false) const noexcept;

    /// Tells if the string begins with the specified value.
    /// @param value The string to compare the start with.
    /// @param start (optional) The position to compare from.
    /// @param end (optional) The position to compare to.
    /// @return True if the string begins with the given value, false otherwise.
    bool startswith(const String& value, const size_t start = 0, const size_t end = SIZE_MAX) const noexcept;

    /// Removes the specified glyphs from both start and end from the string.
    /// @param trailingSet (optional) The glyphs to remove, by default whitespace.
    /// @return New stripped string.
    String strip(const String& glyphsToRemoveSet = " ") const noexcept;

    /// Generates a new string where all lowercase characters are turned uppercase and vice versa.
    /// @return A new string with its case swapped.
    String swapcase() const noexcept;

    /// For each key in the translation table, all occurrences in the string are replaced with the
    /// corresponding key value. The translation table can be generated with the maketrans method.
    /// @param table The dictionary or other mapping object that describes the translation to perform.
    /// @return A new translated string.
    String translate(const Dictionary& table) const noexcept;

    /// Generates a new string with all characters turned into uppercase.
    /// @return A new string where all characters are uppercase.
    String upper() const noexcept;

    /// Inserts zero digit characters (0) in the start of the string until it's length equals
    /// the specified sizeInBytes. If the size of the string is larger than the length, no effect happens.
    /// @param length The size of the final string.
    /// @return A new zero-filled string.
    String zfill(const size_t length) const noexcept;

    ~String() = default;
   private:
    /// Metadata is used as the bitset containing 2 pieces of data. The last bit tells if the whole
    /// string only consists from the ASCII characters and is used in the isascii() method. The remaining
    /// bits are used to tell the last recently updated outpost and is used in updateOutpostCache method.
    uint8_t lastRecentlyUpdatedOutpost = 0;
    bool isOnlyAscii = false;
    GarbageCollected<char> stream;
    size_t graphemes = 0;
    std::array<Outpost, NumberOfOutposts> outposts;

    /// For the input array of characters, this method verifies if it is valid UTF-8 encoded string. If yes,
    /// it counts and sets the related fields such as the size, graphemes and the is ASCII-only flag, but if not,
    /// it raises the UnicodeDecodeError. Support for more Unicode encodings may come in the future, albeit
    /// it takes much greater efforts to detect and convert.
    /// @param data The pointer to the start of the string to verify.
    void verifyEncodingAndConfigureString(const char* data) noexcept;

    /// For the input string slice, this method verifies if it is a valid UTF-8 encoded string. If yes, it counts
    /// and sets the related fields such as the size, graphemes and the is ASCII-only flag, but if not, it raises
    /// the UnicodeDecodeError. Support for more Unicode encodings may come in the future, albeit it takes greater
    /// efforts to detect and convert.
    /// @param slice The string slice to verify.
    void verifyEncodingAndConfigureString(const std::string_view& slice) noexcept;

    /// Selects an outpost to evict and overwrites it with new topical data.
    /// @param index The new grapheme index.
    /// @param destination The new relative offset where the grapheme begins.
    void updateOutpostCache(size_t index, size_t destination) noexcept;

    /// Searches for the index of the char value where the specified grapheme begins.
    /// @param index The index of the grapheme in the string.
    /// @return The index of the byte where the grapheme begins.
    size_t getGrapheme(size_t index);

    /// Creates a new substring from this string.
    /// @param beginning The index to the first byte to begin from.
    /// @param ending The index to the last byte to include.
    /// @param step (Optional) How many graphemes skip after each iteration. By default, 0.
    /// @note This is a primitive method that is used in the process of taking substrings and
    /// creates substrings with known indexes, hence the request must be pre-processed to contain
    /// Unicode-aware graphemes.
    /// @return A new substring.
    String generateSubstring(size_t beginning, size_t ending, size_t step = 0);

    friend std::ostream& operator<<(std::ostream& os, const String& text) noexcept;
  };


}