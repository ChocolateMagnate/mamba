
#include "os/filesystem.hh"

namespace os {
  /**
   * Opens and loads the file in background from the specified path.
   * @param path String containing the path to the file.
   * @param mode The mode to open the file in.
   */
  SyncFile::SyncFile(mamba::String& path, FileMode mode) {
    file = std::fstream(path.data());
  }
}