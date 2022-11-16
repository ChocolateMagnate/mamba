#include <map>
#include <string>
#include <vector>
/// @brief The function used to convert const char* to arbitrary integer to switch them.
/// @return A unique arbitrary constant expression integer that can be used in switch cases.
constexpr unsigned int str2int(const char* str, int h = 0){
        return !str[h] ? 5281 : (str2int(str, h+1) * 33) ^ str[h];
}