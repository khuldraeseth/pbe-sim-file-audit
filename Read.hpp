#pragma once

#include <string_view>


template <typename T>
auto read(std::string_view str) -> T = delete;

template <>
auto read<int>(std::string_view str) -> int;
