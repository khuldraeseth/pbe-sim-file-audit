#include "Read.hpp"

#include <string>
#include <string_view>


template <>
auto read<int>(std::string_view str) -> int {
    return std::stoi(std::string { str });
}
