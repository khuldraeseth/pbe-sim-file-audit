#pragma once

#include <chrono>
#include <string>
#include <string_view>

#include "Read.hpp"

auto show(std::chrono::year_month_day date) -> std::string;

template <>
auto read<std::chrono::year_month_day>(std::string_view str) -> std::chrono::year_month_day;
