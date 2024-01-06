/*+================================================================================================
  File:        conversions.hh

  Summary:     Provides routines related to converting Unicode formats between each other. This file
               essentially implements the str.encode and bytes.decode methods.

  Notes:       In Python, both strings and byte arrays share an underlying similarity that they store
               array of bytes. The str type in a lot of ways is similar to bytes but treats .

  Classes:     None

  Functions:

  Available under Apache Licence v2. Mamba Authors (2023)
=================================================================================================+*/
#pragma once

namespace mamba {
  /// Represents the policy to use during converting string to bytes in the str.encode() method.
  /// Despite its name, it is also used to decode bytes in the bytes.decode() method as the 2 share
  /// the vast majority of their options.
  enum class StringEncodingPolicy {
    BackslashReplace, Ignore, NameReplace, Strict, Replace, SurrogatePass, XMLCharRefReplace
  };

  /// Casts the given string into ASCII. Note that UTF-8 is a wider type and attempting to convert it into
  /// ASCII is narrowing and may result into loss of information if the UTF-8 string contains non-ASCII
  /// graphemes. Use with caution.
  /// @param string The string to encode.
  /// @param graphemes The number of graphemes in the UTF-8 string.
  /// @param policy String encoding policy that states how to handle errors.
  /// @return Converted ASCII string allocated on the ActiveSetMemory pool.
  GarbageCollected<char> encodeStringIntoASCII(const char* string, size_t graphemes, StringEncodingPolicy policy) noexcept;

  /// Casts the given string into its UTF-16 equivalent.
  /// @param string The string to encode.
  /// @param graphemes The number of graphemes in the UTF-8 string.
  /// @param policy String encoding policy that states how to handle conversion errors.
  /// @return Converted UTF-16 string allocated on the ActiveSetMemory pool.
  GarbageCollected<char> encodeStringIntoUTF16(const char* string, size_t graphemes,
                                                      StringEncodingPolicy policy) noexcept;

  /// Casts the given string into its UTF-32 equivalent.
  /// @param string The string to encode.
  /// @param graphemes The number of graphemes in the UTF-8 string.
  /// @param policy String encoding policy that states how to handle conversion errors.
  /// @return Converted UTF-32 string allocated on the ActiveSetMemory pool.
  GarbageCollected<char> encodeStringIntoUTF32(const char* string, size_t graphemes, StringEncodingPolicy policy) noexcept;

  /// Decodes the supplied UTF-8 bytes back into a string. While this operation may seem to be trivial, it
  /// exists as a separate function because UTF-8 is the internal character encoding behind string in Mamba
  /// and it is possible to encode strings into UTF-8 resulting into a bytes object. When trying to convert
  /// the bytes back to a string, this function ensures the bytes are valid UTF-8 and take the processing
  /// action as described by the policy. You may also use this function to translate ASCII bytes as UTF-8
  /// and ASCII are completely backward compatible.
  /// @param utf8Bytes The UTF-8 byte sequence to convert into a string.
  /// @param sizeInBytes The number of bytes the byte array holds.
  /// @param policy String encoding policy that states how to handle conversion errors.
  /// @return Converted string allocated on the ActiveSetMemory pool.
  GarbageCollected<char> decodeUTF8IntoString(const char* utf8Bytes, size_t sizeInBytes, StringEncodingPolicy policy) noexcept;

  /// Decodes the supplied UTF-16-encoded bytes and converts them into a valid string object. If
  /// the operation fails, the appropriate action is taken as stated by the conversion policy.
  /// @param utf16Bytes The UTF-16 byte sequence to convert into a string.
  /// @param sizeInBytes The number of bytes in the byte array.
  /// @param policy String encoding policy that states how to handle conversion errors.
  /// @return Converted string allocated on the ActiveSetMemory pool.
  GarbageCollected<char> decodeUTF16IntoString(const char* utf16Bytes, size_t sizeInBytes, StringEncodingPolicy policy) noexcept;

  /// Decodes the supplied UTF-32-encoded bytes and converts them into a valid string object. If
  /// the operation fails, the appropriate action is taken as stated by the conversion policy.
  /// @param utf32Bytes The UTF-32 byte sequence to convert into string.
  /// @param sizeInBytes The number of bytes in the byte array.
  /// @param policy String encoding policy that states how to handle conversion errors.
  GarbageCollected<char> decodeUTF32IntoString(const char* utf32Bytes, size_t sizeInBytes, StringEncodingPolicy policy) noexcept;

}