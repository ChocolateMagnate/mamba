#pragma once
#include <string>

namespace mamba {
    void warn(const char* message, const char* line, int offset);
    void raiseError(const char* message, const char* line, int offset);
};