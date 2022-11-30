/* Mamba Python by Vladyslav Korol | Apache License 2
 * This file introduces the standard OS module that
 * represents a verity of operating system interfaces.
 * Source: docs.python.org/3/library/os.html */
#pragma once
#include <fstream>
#include "./Interpreter/Base.hh"
namespace mamba::os {

    volatile bool supports_bytes_environ;
    volatile std::tuple<unsigned int, unsigned int> terminal_size;
    volatile bool F_OK, R_OK, W_OK, X_OK; //File access constants.

    volatile mamba::Dictionary pathconf_names; //The dictionary of pathconf names.

    /// @brief Entry used to describe the file metadata.
    struct DirEntry {
        const char *name, *path;
        bool isnode();
        bool isdir();
        bool isfile();
        bool islink();
    };
    /// @brief The entry describing the file statistics.
    struct StatResult {
        mamba::Bitset st_mode, st_ino, st_dev, st_nlink, st_uid, st_gid, st_size, 
        st_atime, st_mtime, st_ctime, st_atime_ns, st_mtime_ns, st_ctime_ns, //Timestaps
        st_blocks, st_blksize, st_rdev, st_flags, st_gen, st_birthtime, st_birthtime_ns, //Unix
        st_file_attributes, st_reparse_tag, //Windows
        st_rsize, st_creator, st_type, //MacOS
        st_fstype; //Solaris
    };

    /// @brief Returns the current working directory.
    const char* ctermid();
    /// @brief Gets the dictionary of the environment variables.
    mamba::Dictionary environ();
    /// @brief Gets the dictionary of the environment variables in bytes.
    /// @note This function is available if supports_bytes_environ is true.
    mamba::Dictionary environb();
    /// @brief Encode the path to the file with filename encoding and error handler.
    /// @return The bytes containing the encoded path.
    mamba::Bitset fsencode(const char* filename);
    /// @brief Decodes the byte path to the textual representation.
    /// @note This function calls __fspath__ for complex objects.
    const char* fsdecode(mamba::Bitset filename);
    /// @brief Retrieves the value of the environment variable.
    /// @param key The name of the environment variable.
    /// @param default The default value to be returned if the variable is not found.
    /// @return The value of the environment variable as a string.
    const char* getenv(mamba::String key, const char* default = nullptr);
    /// @brief Retrieves the byte value of the environment variable.
    /// @param key The name of the environment variable.
    /// @param default The default value to be returned if the variable is not found.
    /// @return The value of the environment variable as bytes.
    const char* getenvb(mamba::Bitset key, const char* default = nullptr);
    /// @brief Searches for named executables in the directories listed in the PATH environment variable.
    /// @param env The dictionary of the environment variables.
    /// @return The list of the paths to the executables.
    mamba::List get_exec_path(mamba::Dictionary* env = nullptr);
    /// @brief Returns the name of the current user.
    const char* getlogin();
    /// @brief Sets the environment variable.
    /// @param key The name of the environment variable. If not found, it is created.
    /// @param content The value of the environment variable as character stream.
    void putenv(const char* key, const char* content);
    /// @brief Retrieves the data about the operating system: system name (sysname), 
    /// name of machine on network (nodename), release identifier (release),
    /// version level (version), hardware details (machine).
    /// @return Iterable bitset object with the fields.
    mamba::Bitset uname();
    /// @brief Deletes the environment variable.
    /// @param key The name of the environment variable to remove.
    void unsetenv(const char* key);
    /// @brief Opens the file.
    /// @param filename The location of the file.
    /// @param flags 
    /// @param mode The mode of the operation: read, write, append, etc.
    /// @return File descriptor.
    std::fstream open(const char* filename, mamba::List flags, const char* mode = "r");
    /// @brief Returns the size of the terminal as a tuple of (columns, lines).
    std::tuple<unsigned int, unsigned int> get_terminal_size();
    /// @brief Changes the current working directory to the specified path.
    void chdir(const char* path);
    /// @brief Changes the root directory to the specified path.
    void chroot(const char* path);
    /// @brief Returns the name of the current directory as a string.
    const char* getcwd();
    /// @brief Returns the name of the current directory as bytes
    mamba::Bitset getcwdb();
    /// @brief Creates a hard link pointing to src named dst. 
    /// Available on Unix and Windows.
    /// @param src The linked file.
    /// @param dst The linking file.
    void link(const char* src, const char* dst);
    /// @brief Lists the content of the directory.
    /// @param path The location in the filesystem.
    /// @return List of strings with the names of the files and folders.
    mamba::List listdir(const char* path = ".");
    /// @brief Generates a new directory.
    /// @param path The path to the new directory.
    /// @note Raises FileExistsError if the directory already exists.
    /// Raises FileNotFoundError if an intermediate directory does not exist.
    /// Raises audit event os.mkdir.
    void mkdir(const char* path, int mode = 0);
    /// @brief Recursively generates a new directories tree.
    /// @param path The path to the directory.
    /// @param mode 
    /// @param exist_ok Indicator whether to accept if an intermediate directory already exists.
    void makedirs(const char* path, int mode = 0, bool exist_ok = false);
    /// @brief Extracts the major device number version.
    int major(unsigned int device);
    /// @brief Extracts the minor device number version.
    int minor(unsigned int device);
    /// @brief Composes the device number from the major and minor versions.
    void makedev(unsigned int major, unsigned int minor);
    /// @brief Gets the relevant system information about the file. Available on Unix.
    /// @param path The file to describe.
    /// @return Dictionary containing the data.
    mamba::Dictionary pathconf(const char* path, const char* name);
    /// @brief Returns the path to the source the link file points to.
    // Available on Unix and Windows.
    const char* readlink(const char* path);
    /// @brief Deletes the content in the specified directory. Raises OSError if 
    // the path leads to a directory and FileNotFoundError if the path does not exist.
    void remove(const char* path);
    /// @brief Recursively deletes directories. Raises OSError if the leaf directory 
    /// could been removed and FileNotFoundError if the path does not exist. Raises
    /// audit event os.remove.
    void removedirs(const char* path);
    /// @brief Renames the source file into the destinatio name. Raises OSError if 
    /// dst is already used or if dst is a non-empty dirrectory. Raises IsADirectoryError
    /// or NotADirectoryError if the source and destination are of different types. Raises
    /// audit event os.rename.
    /// @note Use replace() to overwrite the destination anyways.
    void rename(const char* src, const char* dst);
    /// @brief Renames the source file into the destinatio name generating all
    /// the intermediate directories in between avoiding FileNotFoundError.
    void renames(const char* src, const char* dst);
    /// @brief Renames the source file into the destinatio name even if the
    /// destination already exists if user has the permission. Raises OSError if
    /// the source file does not exist. Raises audit event os.rename.
    void replace(const char* src, const char* dst);
    /// @brief Deletes the directory. Raises OSError or FileNotFoundError if the
    /// directory does not exist. Raises audit event os.rmdir.
    void rmdir(const char* path);
    /// @brief Yields file information.
    /// @param path The file to describe.
    /// @return DirEntry object containing the data and getters.
    DirEntry scandir(const char* path);
    /// @brief Returns the statistics about the file.
    StatResult stat(const char* path);



};