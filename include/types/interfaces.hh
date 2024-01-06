/*+===================================================================
  File:         interfaces.hh

  Summary:      Represents various abstract base classes and traits used
                by default

  Classes:

  Functions:

  Available under Apache License v2. Mamba Authors (2023)
===================================================================+*/
#pragma once

#include <tuple>

namespace mamba {
  /// The built-in range type commonly used in for loops contains the first, last and step values.
  typedef std::tuple<size_t, size_t, size_t> Range;

  /// The interface used by the types that represent sequences and can
  /// be enumerated through. Enumerable can iterated in the for loop,
  /// yielding or returning values, and is the return value of iter().
  class Enumerable {};
}