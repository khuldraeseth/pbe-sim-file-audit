#pragma once

#include "Read.hpp"


struct Height {
    int cm;
};


auto show(Height height) -> std::string;

template <>
auto read<Height>(std::string_view str) -> Height;
