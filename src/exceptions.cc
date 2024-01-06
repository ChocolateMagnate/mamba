
#include "exceptions.hh"
namespace mamba {
  UnicodeError::UnicodeError(const char *errorText) {
    errorMessage = errorText;
  }

  const char *UnicodeError::what() const noexcept {
    return errorMessage;
  }


  UnicodeASCIIConversion::UnicodeASCIIConversion(const char *errorText, uint32_t _invalidUnicodeCodePoint)
                              : UnicodeError(errorText), invalidUnicodeCodePoint{_invalidUnicodeCodePoint} { }

  const char *UnicodeASCIIConversion::what() const noexcept {
    return errorMessage;
  }

  uint32_t UnicodeASCIIConversion::getInvalidUnicodeCodePoint() const noexcept {
    return invalidUnicodeCodePoint;
  }


  InvalidUTF8Composition::InvalidUTF8Composition(const char *errorText) : UnicodeError(errorText) { }

  const char *InvalidUTF8Composition::what() const noexcept {
    return errorMessage;
  }


  InvalidUTF16Composition::InvalidUTF16Composition(const char *text, uint16_t surrogatePair)
      : UnicodeError(text), invalidSurrogatePair{surrogatePair} { }


  const char *InvalidUTF16Composition::what() const noexcept {
    return errorMessage;
  }

  uint16_t InvalidUTF16Composition::getInvalidSurrogatePair() const noexcept {
    return invalidSurrogatePair;
  }


  InvalidUTF32Composition::InvalidUTF32Composition(const char *errorText, uint32_t codePoint)
      : UnicodeError(errorText), invalidUnicodeCodePoint{codePoint} { }

  const char *InvalidUTF32Composition::what() const noexcept {
    return errorMessage;
  }

  uint32_t InvalidUTF32Composition::getInvalidUnicodeCodePoint() const noexcept {
    return invalidUnicodeCodePoint;
  }


}