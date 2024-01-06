/*+================================================================================================
  File:        store.hh

  Summary:     Inter-thread storage used to hold both thread-local and global data.

  Functions:   select<T>

  Available under Apache Licence v2. Mamba Authors (2023)
=================================================================================================+*/
#pragma once

namespace mamba {
  /// Retrieves the instance of the shared resource from the thread-local storage for the set of data.
  /// @return Mutable reference to the requested resource.
  template<typename T> T& select();
}