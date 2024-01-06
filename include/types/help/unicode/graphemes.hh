/*+================================================================================================
  File:        graphemes.hh

  Summary:     Contains Unicode-aware routines related to identification of single graphemes, those being
               individual human writing system units. There are multiple character encodings supported in Mamba,
               such as ASCII, UTF-8, UTF-16 and UTF-32, and the functions that extract their Unicode code points
               and size in bytes can be found here.

  Notes:       Unicode is a common standard used to convey characters from all writing systems in the world within
               a single framework. It was created to address the challenges in the variability in character encodings
               used for non-ASCII languages, such as ISO 8859, Shift JIS or KOI8-U. Under the hood, everything Unicode
               does is mapping between all units of writing systems such as letters, numbers, special characters and
               emojis to a unique number representation, called code point, commonly denoted with U+. For example,
               the English letter E is U+0045, the Slavic letter Ж is U+0416 and the French letter cedilla ç is U+00E7.
               These individual characters that are units of human writing systems are called graphemes or glyphs.
               All graphemes below U+10FFFF inclusively are called Basic Multilingual Plane (BML), where all graphemes
               belong to alphabets of natural languages: Latin alphabet, Cyrillic, Arabic script, Japanese kanji,
               number digits and some special characters like underscores or the dollar sign. Unicode also includes
               graphemes beyond language alphabets, such as exponents and emojis. Since Unicode is enormous, there are
               different character encodings that actualise it into specific schemes how numbers are converted into
               a sequence of bytes and interpreted. Hence, multiple Unicode Transport Formats (UTF) encodings emerged
               that each takes from 8 (UTF-8), 16 (UTF-16) and 32 (UTF-32) bits to represent various graphemes.
               Generally speaking, UTF-8 is the mostly widely used encoding almost exclusively used on the Internet,
               Unix systems, and has recently gained wider support on Windows. UTF-8 is completely backward-compatible
               with ASCII, is space efficient, and has universal support in terminal emulators, meaning one can print
               UTF-8-encoded strings out of the box. Despite these advantages, UTF-8 becomes space inefficient when
               dealing with a mix of different writing systems, where UTF-16 can be used as a good point of balance,
               although the space gains of UTF-16 over UTF-8 are often overestimated. UTF-32 is almost never used in
               practice. However, it has a peculiar advantage that it provides random-access of graphemes where they
               could be read directly from the index, while other encodings are variable-length and require
               sophisticated caching techniques to accelerate their access. This is why it may be used as an internal
               representation of strings and may be beneficial for certain optimisations that would benefit from it.

  Classes:     None

  Functions:   getUnicodePointForUTF8(cont char*), getSizeOfUTF8Grapheme(const char*),
               getSizeOfUTF8Grapheme(uint32_t), getUnicodePointForUTF16(const char*),
               getUnicodePointForUTF16(const char*), getSizeOfUTF16Grapheme(const char*),
               getSizeOfUTf16Grapheme(uint32_t), getUnicodeCodePointForUTF32(const char*),
               setBitmaskFlagsForCharacter(const char*, StringBitmask&)

  Available under Apache Licence v2. Mamba Authors (2023)
================================================================================================+*/
#pragma once

#include <bitset>
#include <cstdint>
namespace mamba {
  typedef std::bitset<15> StringBitmask;
  const auto AsciiBit = 0, AlphaBit = 1, AlphaNumericBit = 2, NumericBit = 3, DigitBit = 4, DecimalBit = 5,
      IdentifierBit = 6, LowerBit = 7, UpperBit = 9, PrintableBit = 10, SpaceBit = 11, TitleBit = 12,
      PreviousCharacterIsSpaceBit = 13, InitialisedBit = 14;

  /// Analyses the code point for the given sequence of characters in the UTF-8 encoding.
  /// @param source The pointer to the code unit to analyse. If the pointer is misaligned
  // (such as the pointer points to the middle of a multibyte sequence), the function can
  /// recover by analysing the leading bits and moving the character pointer accordingly
  /// as long as it is valid UTF-8.
  /// @throw InvalidUTF8Composition if the source is invalid.
  /// @see https://stackoverflow.com/a/68835029 UTF-8 hex to unicode code point (only math)
  /// @return Unsigned 32-bit integer containing the Unicode code point for the given text.
  uint32_t getUnicodePointForUTF8(const char* source);

  /// Determines how many bytes are used to determine the grapheme. In the UTF-8 encoding,
  /// if the character is ASCII, it takes a single byte to represent, and from there it
  /// grows up to 4 characters.
  /// @param data The pointer to the character where the grapheme begins.
  /// @throw InvalidUTF8Composition if the string is invalid UTF-8 character.
  /// @return Integers 1 to 4 if the grapheme points to a valid sequence, or throws the
  /// InvalidUTF8Composition exception is the string is incorrect UTF-8.
  int getSizeOfUTF8Grapheme(const char* data);

  /// Determines the number of bytes needed to hold the given Unicode code point in UTF-8.
  /// @param unicodeCodePoint The Unicode code point denoted as unsigned 32-bit integer.
  /// @return The number of bytes it takes to store the given code point in UTF-8.
  int getSizeOfUTF8Grapheme(uint32_t unicodeCodePoint);

  /// Analyses the code point for the given sequence of characters in the UTF-16 encoding.
  /// @param source The pointer to the code unit to analyse. If the pointer is misaligned,
  /// (such as pointing to the low surrogate), the function can recovery by moving it 2
  /// bytes before as long as it if valid UTF-16.
  /// @throw InvalidUTF16Composition if source points to the incorrect UTF-16.
  /// @return Unsigned 32-bit integer containing the Unicode code point for the given text.
  uint32_t getUnicodePointForUTF16(const char* source);

  /// Tells how many bytes the specified UTF-16 grapheme takes. UTF-16 always takes either 2 or 4 bytes to
  /// represent the graphemes, and if the grapheme is in the Basic Multilingual Plane (BMP, spanning values
  /// between U+0000 to U+10FFFF), the grapheme takes 2 bytes, and 4 bytes for any other graphemes beyond it.
  /// @param data The pointer to the UTF-16-encoded grapheme. The function assumes that it can read at least
  /// one more byte after this pointer.
  /// @return The number of bytes needed ot represent the provided UTF-16 grapheme.
  int getSizeOfUTF16Grapheme(const char* data);

  /// Determines the number of bytes needed to hold the given Unicode code point in UTF-16.
  /// @param unicodeCodePoint The Unicode code point denoted as unsigned 32-bit integer.
  /// @return The number of bytes it takes to store the given code point in UTF-16.
  int getSizeOfUTf16Grapheme(uint32_t unicodeCodePoint);

  /// Analyses the code point for the given sequence of bytes in the UTF-32 encoding.
  /// @param source The pointer to the code unit to analyse. UTF-32 always takes 4 bytes
  /// and the function assumes it can read 3 bytes after this pointer.
  /// @return Unsigned 32-bit integer containing the Unicode code point for the given text.
  uint32_t getUnicodeCodePointForUTF32(const char* source);

  /// Evaluates a single character and determines its flags, such as if it is
  /// ASCII, alphabetic, numeric, alphanumeric, uppercase, lowercase, etc. This
  /// function must only run for a single character and be used in a loop.
  /// @param character The pointer to the character to evaluate.
  /// @param flags The output bitmask of the string.
  void setBitmaskFlagsForCharacter(const char* character, StringBitmask& flags);
}