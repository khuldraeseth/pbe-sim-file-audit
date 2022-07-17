#pragma once

#include <string>
#include <string_view>

#include "Read.hpp"


enum class Position {
    StartingPitcher,
    ReliefPitcher,
    ClosingPitcher,
    Catcher,
    FirstBase,
    SecondBase,
    ThirdBase,
    Shortstop,
    LeftField,
    CenterField,
    RightField,
    DesignatedHitter,
};

auto show(Position position) -> std::string;

template <>
auto read<Position>(std::string_view str) -> Position;
