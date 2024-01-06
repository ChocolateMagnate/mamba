
#include "givers/GarbageCollectedStack/ActiveSetMemory.hh"
#include "givers/multithreading/store.hh"
namespace mamba {
  ActiveSetMemory memory;
  template<> ActiveSetMemory& select<ActiveSetMemory>() {
    return memory;
  }
}





