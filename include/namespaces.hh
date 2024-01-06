/*+===================================================================
  SyncFile:      namespaces.hh

  Summary:   Represents the module namespace object.

  Classes:   Namespace

  Functions: 

  Available under Apache License v2. Mamba Authors (2023)
===================================================================+*/
#pragma once

#include <string>
#include <unordered_map>

#include "IR.hh"
namespace mamba {
  /// Represents a single module namespace containing a set of global
  /// variables and top-level declared functions and classes in the form
  /// of dictionary.
  class Namespace {
   public:
    Namespace() = default;

    Namespace(const Namespace& other);

    Namespace(const Namespace&& other);

    Namespace(const MicrocodeStream& microcode);

    void operator=(const Namespace& other);

    /// Accesses an item from the module. It either provides direct
    /// access to its internal or restricts to those exported through
    /// the __all__ declaration.
    /// @param key String denoting the key to access the item with.
    /// @return Microcode section corresponding to the imported item.
    MicrocodeSection& operator[](const std::string_view& key);

    MicrocodeSection& begin();

    MicrocodeSection& next();

    MicrocodeSection& end();
  };

  /// Helper function used to obtain the reference to the main namespace initialised early.
  /// Calling this function is equivalent to retrieving it from namespaces["__main__"].
  /// @return GarbageCollected to the main namespace.
  Namespace& _main_();

  /// Getter for the working module. At the start, it is the main module, but as
  /// other modules are being imported inside other packages from the main, this
  /// function updates the reference to that namespace. Its primary use is in the
  /// import process to attach imported modules in the current namespace.
  /// @return The current working namespace.
  Namespace& _name_();

  /// Global namespace dictionary holding all the namespaces. The main
  /// namespace is available under the key __main__, and the rest can
  /// be accessed under their respective names or aliases if induced.
  std::unordered_map<std::string_view, Namespace> namespaces;
}