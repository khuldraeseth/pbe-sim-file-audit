#pragma once

#include <chrono>
#include <string>
#include <string_view>

#include "Batting.hpp"
#include "Fielding.hpp"
#include "Pitching.hpp"
#include "Read.hpp"


enum class BattingHandedness {
    Left,
    Right,
    Switch,
};

auto show(BattingHandedness const& handedness) -> std::string;

template <>
auto read<BattingHandedness>(std::string_view str) -> BattingHandedness;


enum class ThrowingHandedness {
    Left,
    Right,
};

auto show(ThrowingHandedness const& handedness) -> std::string;

template <>
auto read<ThrowingHandedness>(std::string_view str) -> ThrowingHandedness;


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


struct CommonAttributes {
    std::string name;
    std::chrono::year_month_day birthdate;
    std::string birthplace;
    int heightCm;
    int weightLb;   // wtf, OOTP
    BattingHandedness battingHandedness;
    ThrowingHandedness throwingHandedness;
    Position position;
};

struct Player {
    CommonAttributes commonAttributes;
    BattingAttributes battingAttributes;
    FieldingAttributes fieldingAttributes;
    PitchingAttributes pitchingAttributes;
};
