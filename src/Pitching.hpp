#pragma once

#include <string>
#include <string_view>

#include "Read.hpp"


enum class ArmSlot {
    OverTheTop,
    Normal,
    Sidearm,
    Submarine,
};

auto show(ArmSlot slot) -> std::string;

template <>
auto read<ArmSlot>(std::string_view str) -> ArmSlot;


struct PitchingAttributes {
    int velo { 0 };
    int stamina { 0 };
    int hold { 0 };
    ArmSlot armSlot { ArmSlot::Normal };

    int movementL { 0 };
    int movementR { 0 };
    int movementPotential { 0 };
    int controlL { 0 };
    int controlR { 0 };
    int controlPotential { 0 };
    int hbp { 0 };
    int wp { 0 };
    int balk { 0 };
    int gb { 0 };


    int fastball { 0 };
    int sinker { 0 };
    int cutter { 0 };
    int curveball { 0 };
    int slider { 0 };
    int changeup { 0 };
    int splitter { 0 };
    int forkball { 0 };
    int circleChange { 0 };
    int screwball { 0 };
    int knuckleCurve { 0 };
    int knuckleball { 0 };
};


static constexpr PitchingAttributes batterPitching {
    .velo = 80,
    .stamina = 1,
    .hold = 1,
    .armSlot = ArmSlot::Normal,

    .movementL = 1,
    .movementR = 1,
    .movementPotential = 1,
    .controlL = 1,
    .controlR = 1,
    .controlPotential = 1,
    .hbp = 21,
    .wp = 15,
    .balk = 30,
    .gb = 44,

    .fastball = 1,
    .sinker = 0,
    .cutter = 0,
    .curveball = 0,
    .slider = 0,
    .changeup = 0,
    .splitter = 0,
    .forkball = 0,
    .circleChange = 0,
    .screwball = 0,
    .knuckleCurve = 0,
    .knuckleball = 0,
};
