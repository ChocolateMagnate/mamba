/*+================================================================================================
  File:        exceptions.hh

  Summary:     Contains all C++ exceptions used in Mamba for exceptional error cases.

  Classes:     UnicodeError, InvalidUTF8Composition, InvalidUTF16Composition, InvalidUTF32Composition

  Functions:   None

  Available under Apache License v2. Mamba Authors (2023)
================================================================================================+*/
#pragma once

#include <stdexcept>
#include <cstdint>

namespace mamba {
  class UnicodeError : public std::exception {
   public:
    explicit UnicodeError(const char* errorText);
    [[nodiscard]] const char* what() const noexcept override;
   protected:
    const char* errorMessage;
  };

  class UnicodeASCIIConversion : public UnicodeError {
   public:
    UnicodeASCIIConversion(const char* errorText, uint32_t _invalidUnicodeCodePoint);
    [[nodiscard]] const char* what() const noexcept override;
    [[nodiscard]] uint32_t getInvalidUnicodeCodePoint() const noexcept;
   private:
    uint32_t invalidUnicodeCodePoint;
  };

  class InvalidUTF8Composition : public UnicodeError {
   public:
    explicit InvalidUTF8Composition(const char* errorText);
    [[nodiscard]] const char* what() const noexcept override;
  };

  class InvalidUTF16Composition : public UnicodeError {
   public:
    explicit InvalidUTF16Composition(const char* text, uint16_t surrogatePair);
    [[nodiscard]] const char* what() const noexcept override;
    [[nodiscard]] uint16_t getInvalidSurrogatePair() const noexcept;
   private:
    uint16_t invalidSurrogatePair = 0;
  };

  class InvalidUTF32Composition : public UnicodeError {
   public:
    explicit InvalidUTF32Composition(const char* errorText, uint32_t codePoint);
    [[nodiscard]] const char* what() const noexcept override;
    [[nodiscard]] uint32_t getInvalidUnicodeCodePoint() const noexcept;
   private:
    uint32_t invalidUnicodeCodePoint = 0;
  };
}