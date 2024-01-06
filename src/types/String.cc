
#include "types/String.hh"
namespace mamba {
    char* String::data() const noexcept {
        return stream.destination;
    }
}