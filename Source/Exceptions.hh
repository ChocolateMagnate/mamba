/* Mamba Python by Vladyslav Korol | Apache License 2
 * This file takes care of managing errors and warnings.
 * Source: docs.python.org/3/library/exceptions.html */
#pragma once
#include "./Interpreter/Base.hh"
namespace mamba {
    /// @brief Prints a warning to the console.
    /// @param warning The warning to print.
    void warn(mamba::Warnings warning);

    /// @brief Raises an exception and terminates the program.
    /// @param exception The exception to raise.
    void raise(mamba::Exceptions exception);

    enum class Exceptions {
        BaseException, GeneratorExit, KeyboardInterrupt, SystemExit, Exception,
        AssertionError, AttributeError, BufferError, EOFError, ImportError, LookupError,
        MemoryError, NameError, OSError, ReferenceError, RuntimeError,SyntaxError,
        SystemError, TypeError, ValueError, ZeroDivisionError, EnvironmentError, IOError,
        WindowsError, BlockingIOError, ChildProcessError, ConnectionError, BrokenPipeError,
        ConnectionAbortionError, ConnectionRefusedError, ConnectionResetError, FileExistsError,
        FileNotFoundError, InterruptedError, IsADirectoryError, NotADirectoryError,
        PermissionError, ProcessLookupError, TimeoutError
    };

    enum class Warnings {
        Warning, UserWarning, DeprecationWarning, PendingDeprecationWarning, SyntaxWarning,
        RuntimeWarning, FutureWarning, ImportWarning, UnicodeWarning, BytesWarning, ResourceWarning
    };

    /// @brief Adds a developer-defined exception as a post-38 integer and attached metadata.
    /// @param name The name of the exception, for example PandasError or LookupError.
    /// @param description The text to display when the exception occurs.
    /// @return The integer ID of the exception.
    unsigned int addCustomException(const char* name, const char* description);
};