/*+===================================================================
  SyncFile:      filesystem.hh

  Summary:   Provides the cross-platform interface for allocating, reading,
             writing and otherwise managing the files. Under the hood, this
             code uses the native system calls to achieve consistent file
             interface.

  Classes:   SyncFile, AsyncFile

  Functions: open,

  Available under Apache License v2. Mamba Authors (2023)
===================================================================+*/
#pragma once

#include <fstream>
#include <variant>
#include <string_view>

#include "../context.hh"
#include "../types/String.hh"

namespace os {
  /**
   * Represents all modes the file can be opened in Python.
   */
  enum class FileMode {
    Read, Write, ReadAndWrite, Append, AppendAndRead
  };

  /**
   * Represents blocking synchronous file object to read, write, append
   * and otherwise manipulate fies. An object of this class is returned
   * from the built-in open function and blocks on most requests.
   */
  class SyncFile {
   public:
    SyncFile() = delete;

    /**
     * Opens and loads the file in background from the specified path.
     * @param path String containing the path to the file.
     * @param mode The mode to open the file in.
     */
    SyncFile(mamba::String& path, FileMode mode);

    /**
     * Opens and loads the file in background from the specified path.
     * @param path String view representing the path to the file.
     * @param mode The mode to open file in.
     */
    SyncFile(const mamba::String& path, FileMode mode);

    /**
     * Loads the entire file as a GarbageCollectedStack-mapped file. This feature is
     * useful for efficient random-access and string slicing in the file.
     * @return GarbageCollected to self.
     */
    SyncFile& mapToMemory();

    /**
     * Tells the current state of the file during call.
     * @return 0 if file is opened and can be used, 1 if it's pending,
     * 2 if file was not found, 3 if the path is a directory, 4 if
     * permission error was raised and 5 for any other arbitrary OSError.
     */
    int status();

    /**
     * Reads the entire file into a string object. Available in the "r" mode.
     * @return A new string containing the entire content of the file.
     */
    mamba::String read();

    /**
     * Reads the file up to the specified length into a String.
     * @param size The maximum length of the string. If file is larger than this
     * limit, it will stop at this number of characters, otherwise the entire file
     * will be loaded into the string.
     * @return The string containing the file with the prompted sizeInBytes.
     */
    mamba::String read(size_t size);

    /**
     * Returns the first line of the file.
     * @return String containing the first line until the newline.
     */
    mamba::String readline();

    /**
     * Prints a line in the file. Each subsequent call prints the next line in order.
     * @return String containing the next line.
     */
    mamba::String next();

    /**
     * Writes the string text into the file. Only available in "w" mode.
     * @param content The Python string to write to the file.
     */
    void write(const mamba::String& content);

    /**
     * Writes bytes to the file. Only available in "wb" mode.
     * @param content The bytes to write to the file.
     */
    void write(const mamba::Bytes& content);

    /**
     * Writes multiple lines to the end of the file.
     * @param lines The enumerable (list, tuple, etc.) containing the lines to write.
     */
    void writelines(mamba::Enumerable lines);

    void close();

    ~SyncFile();

   private:
    int _status;
    std::byte* content;
    std::fstream file;
  };

  /**
   * The built-in open context manager used to open files in Python.
   * @param filename The name of the file to open.
   * @param mode (optional) The mode to open the file in, by default readonly.
   * @param isAsync (optional) The type of file manager, asynchronous or synchronous.
   * By default, it is false (synchronous) and requires the async context manager to be invoked.
   * @note Raises: FileNotFound, PermissionError, IsADirectoryError, OSError.
   * @return File object, SyncFile or AsyncFile.
   */
  std::variant<SyncFile> open(mamba::String filename, FileMode mode = FileMode::Read, bool isAsync = false);
}