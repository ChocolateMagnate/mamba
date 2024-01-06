/*+===================================================================
  File:      context.hh

  Summary:   Collects and provides accessors to manipulate runtime-gathered
             information in a thread-safe manner. This data is consumed by JIT
             compiler and exception handlers. Since exceptions are highly
             related to the runtime data, they are coupled with this file too.

  Classes:   Signal

  Functions: raise(), raise(Signal, std::string_view), raise(size_t, std::string_view),
             except (Signal, MicrocodeFunction&), except(size_t, MicrocodeFunction&), panic()

  Available under Apache Licence v2. Mamba Authors (2023)
===================================================================+*/
#pragma once

#include <string>

#include "IR.hh"
namespace mamba {
  /// Represents the optimisation level Mamba is running on.
  enum class Mode {
    O0, O1, O2, O3, O4, O5
  };

  /// Represents the standard language exceptions.
  enum class Signal {
    //Foundational exceptions
    BaseException, Exception, ArithmeticError, BufferError, LookupError,
    //Mamba-specific exceptions
    MambaExit, MambaPanic, SecurityError,
    //Built-in concrete exceptions
    AssertionError, AttributeError, EOFError, FloatingPointError, GeneratorExit, ImportError,
    ModuleNotFoundError, IndexError, KeyError, KeyboardInterrupt, MemoryError, NameError,
    NotImplementedError, OSError, OverflowError, RecursionError, ReferenceError, RuntimeError,
    StopIteration, StopAsyncIteration, SyntaxError, IndentationError, TabError, SystemError,
    SystemExit, TypeError, ViolatedTypeHintError, UnboundLocalError, UnicodeError, UnicodeEncodeError,
    UnicodeDecodeError, ValueError, ZeroDivisionError,
    //Backward-compatible exceptions
    EnvironmentError, IOError, WindowsError,
    //Operating system exceptions
    BlockingIOError, ChildProcessError, ConnectionError, BrokenPipeError, ConnectionAbortedError,
    ConnectionRefusedError, ConnectionResetError, FileExistsError, FileNotFoundError, InterruptedError,
    IsADirectoryError, PermissionError, ProcessLookupError, TimeoutError
  };

  /// Contains the reasons why builtin exceptions are raised. Reasons are useful because callers
  /// often times do not have enough information to formulate a meaningful error message and
  /// include all details, hence this responsibility is shifted over to the raise() function.
  /// If exception is not handled, then raise will gather runtime data and generate a meaningful
  /// error message for the supplied reason.
  enum class ExceptionReason {
    //Memory-related reasons
    HostDoesNotHaveEnoughMemoryToStart, HostRanOutOfMemory, AttemptToAllocateBeyondLargestSize,
    // Unicode errors
    ASCIIToUTF8ConversionFailure, UTF8ToUTF8ConversionFailure, UTF16ToUTF8ConversionFailure,
    UTF32ToUTF8ConversionFailure,
    // Miscellaneous
    IntegerToStringConversionLimitViolation,
  };

  /// Raises a built-in exception.
  /// @param error The exception enum class value to raise.
  /// @param reason The reason why the exception was raised.
  void raise(Signal error, ExceptionReason reason);

  /// Raises an exception.
  /// @param error The exception id value. In Mamba, all exceptions are represented with unique
  /// integer values. While the built-in exceptions are mapped with the Signal enum class, other
  /// custom exceptions are associated with a unique generated ID number. It is also possible to
  /// raise a built-in exception with this overload, but the other one exists to avoid casting an
  /// Signal enum value into size_t.
  /// @param message (optional) The message to print to the terminal in case if the exception will
  /// not be handled. While not necessary, it is advisable to include helpful error message to
  /// indicate reason, circumstances, and potential solutions for developer.
  void raise(size_t error, const std::string_view& message);

  /// Sets up an exception handler for a built-in exception.
  /// @param error The enum value corresponding to the exception to catch.
  /// @param handle The exception handler to execute when the exception is encountered.
  void except(Signal error, const MicrocodeSection& handle);

  /// Sets up an exception handler.
  /// @param error The numeric value corresponding to the exception to catch.
  /// @param handle The exception handler to execute when the exception is encountered.
  void except(size_t error, const MicrocodeSection& handle);

  /// Gives the currently set limitation on integer-to-string conversion. By default, it is
  /// set to 4000 digit, but can be further changed with sys.get_int_max_str_digits().
  /// @return Integer that defines how many digits can be converted into a string.
  size_t getIntegerToStringConversionLimit();

  /// Changes the maximum number of digits on integer-to-string conversion. By default, it
  /// is set to 4000 digits, and anything beyond it raises ValueError. This function is
  /// equivalent to sys.set_int_max_str_digits().
  /// @param limit The new limit to assign the value to.
  void setIntegerToStringConversionLimit(size_t limit);

  /// Pushes a new frame into call stack. This function should be called every time when
  /// a new function is called, even if inlined, and it provides the information for call
  /// stack when printing the error message when an exception is not handled.
  /// @param traceback Traceback object to push.
  void pushCallStack(Traceback traceback);

  /// Deletes an item from a call stack. This function should be used every time when a
  /// function returns, even if it's inlined.
  void popCallStack();



}