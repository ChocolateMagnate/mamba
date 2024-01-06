/*+===================================================================
  SyncFile:    Integer.hh

  Summary:    Defines the arbitrary large integer type in Python.

  Classes:    Integer

  Functions:  None

  Available under Apache Licence v2. Mamba Authors (2023)
===================================================================+*/
#pragma once

#include <cstdint>
#include <utility>
#include "../givers/memory.hh"

namespace mamba {
  /// Represents the built-in integer type in Python. One of its key aspects
  /// is that integers in Python are arbitrary-length and must be able to grow
  /// as long as the machine has enough GarbageCollectedStack to store them, hence we utilise
  /// ActiveSetMemory allocator to store the whole numbers. Under the hood, we
  /// store integers as a contiguous sequence of units called cells where each
  /// cell stores 32 bits of data. In case if the integer is sufficiently small
  /// and does not grow out of the 32-bit range, the integer value acts as the
  /// primitive value and benefits from its efficiency. However, when it grows
  /// out of that sizeInBytes, we use multiple cells for representing a larger value.
  /// Conventionally, we then treat each cell as 2 blocks of 16-bit values.
  /// When performing arithmetic on multiple wide integers, we add block by
  /// cell. The second block acts as the padding where any necessary overflow
  /// or underflow propagates ensuring the proper state of the integer. As the
  /// consequence, values of these class may take more GarbageCollectedStack to be represented,
  /// but they benefit from SIMD and can be highly efficiently processed: on SSE2,
  /// integers up to 128 bits are as cheap as the primitives (340282366920938463463374607431768211456),
  /// while on AVX 256 bits (115792089237316195423570985008687907853269984665640564039457584007913129639936).
  class Integer {
   public:
    Integer();
    explicit(false) Integer(const int primitiveInteger);          //NOLINT
    explicit(false) Integer(const GarbageCollected<uint32_t>& address); //NOLINT
    Integer(const Integer& other);
    Integer(Integer&& other) noexcept;

    Integer&  operator=(int primitiveInteger) noexcept;
    Integer&  operator=(const Integer& other) noexcept = default;
    Integer&  operator=(Integer&& other) noexcept;

    Integer operator+(const size_t other) const noexcept;
    Integer&& operator=(const size_t other) const noexcept;
    Integer&& operator*(const size_t other) const noexcept;
    Integer&& operator/(const size_t other) const noexcept;
    Integer&& operator%(const size_t other) const noexcept;
    Integer&  operator+=(const size_t other) noexcept;
    Integer&  operator-=(const size_t other) noexcept;
    Integer&  operator*=(const size_t other) noexcept;
    Integer&  operator/=(const size_t other) noexcept;
    Integer&  operator%=(const size_t other) noexcept;

    Integer&& operator+(const Integer& other) const noexcept;
    Integer&& operator-(const Integer& other) const noexcept;
    Integer&& operator*(const Integer& other) const noexcept;
    Integer&& operator/(const Integer& other) const noexcept;
    Integer&& operator%(const Integer& other) const noexcept;
    Integer&  operator+=(const Integer& other) noexcept;
    Integer&  operator-=(const Integer& other) noexcept;
    Integer&  operator*=(const Integer& other) noexcept;
    Integer&  operator/=(const Integer& other) noexcept;
    Integer&  operator%=(const Integer& other) noexcept;

    bool operator==(const Integer& other) const noexcept;
    bool operator>(const Integer& other) const noexcept;
    bool operator<(const Integer& other) const noexcept;
    bool operator>=(const Integer& other) const noexcept;
    bool operator<=(const Integer& other) const noexcept;

    Integer&& operator&(const Integer& other) const noexcept;
    Integer&& operator|(const Integer& other) const noexcept;
    Integer&& operator^(const Integer& other) const noexcept;
    Integer&& operator~() const noexcept;
    Integer&& operator<<(const Integer& other) const noexcept;
    Integer&& operator>>(const Integer& other) const noexcept;
    Integer&  operator&=(const Integer& other) noexcept;
    Integer&  operator|=(const Integer& other) noexcept;
    Integer&  operator^=(const Integer& other) noexcept;
    Integer&  operator<<=(const Integer& other) noexcept;
    Integer&  operator>>=(const Integer& other) noexcept;

    [[nodiscard]] GarbageCollected<char>&& str();

    /// Tells how many bits are needed to represent this integer in binary
    /// excluding the sign bit and leading bits.
    /// @see https://docs.python.org/3/library/stdtypes.html#int.bit_length
    /// @return The number of bits needed to represent this integer.
    [[nodiscard]] size_t bit_length() const noexcept;

    /// Tells how many bits are set (true) in the integer. Conversely, the
    /// number of unset bits can be found by subtracting the total number
    /// of bits and the number of set bits.
    /// @return The number of set bits in the integer binary representation.
    [[nodiscard]] size_t bit_count() const noexcept;

    /// Converts the integer into a byte array.
    /// @param length (optional) The number of bytes in the final byte array. If this value is larger
    /// than the integer is represented with, then the leading bytes are initialised to zero, but if
    /// the integer does not fit into this range, OverflowError is raised. Defaults to 1.
    /// @param isBigEndian (optional) The flag that tells which binary encoding is used. If it is true,
    /// the integer is encoded as big endian, but if false, as little ending. Defaults to true.
    /// @param isSigned (optional) The flag that tells if to interpret the integer as signed or unsigned. It
    /// is recommended to never touch it and always treat integers as signed, but this flag can be unset if
    /// caller is sure the integer is used throughout its lifetime as unsigned. If a negative value is supplied
    /// but the flag is set to false, OverflowError is raised.
    /// @note When using the default overload, this method can be useful to cast an integer into a single byte
    /// and then be interpreted as an ASCII character, but do not do this with values larger than 255 (beware
    /// OverflowError).
    /// @throw OverflowError if integer is too big or some of the arguments are misled with the actual integer.
    /// @return Movable GarbageCollected object containing the raw bytes.
    static GarbageCollected<std::byte>&& to_bytes(unsigned int length = 1, bool isBigEndian = true,
                                                  bool isSigned = true) noexcept;

    /// Constructs an integer from the given byte array.
    /// @param bytes The GarbageCollected to the byte values.
    /// @param isBigEndian (optional) The most significant bit to interpret the bytes with. When set to true
    /// (default), the bytes are interpreted as big endian, otherwise as little endian.
    /// @param isSigned (optional) The flag that tells if the bytes must be interpreted as signed (default) or unsigned.
    static Integer&& from_bytes(GarbageCollected<std::byte>& bytes, bool isBigEndian = true, bool isSigned = true) noexcept;

    /// Finds out the pair of integers, numerator and denominator. When divided, they will yield the same
    /// integer as held by the object. Since there are infinite combinations of such pairs, this method will
    /// only return one of them, the most basic one, that is useful in cases when caller needs to represent
    /// whole numbers as fractions, in which case which pair gets returned exactly is not relevant.
    /// @return Pair of numerator and denominator that represent the ratio.
    std::pair<Integer**, Integer&&> as_integer_ratio() const noexcept;

    /// Returns true. This method exists for compatibility with float's equivalent is_integer when the types are mixed.
    /// @see https://docs.python.org/3/library/stdtypes.html#float.is_integer
    static bool is_integer() noexcept;

   private:
    GarbageCollected<uint32_t> cells;
    size_t numberOfCells = 1;
  };
}