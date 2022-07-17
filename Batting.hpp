#pragma once


enum class GbType {
    Spray,
    Normal,
    Pull,
    ExtremePull,
};

enum class FbType {
    Spray,
    Normal,
    Pull,
};

struct BattingAttributes {
    int babipL { 0 };
    int babipR { 0 };
    int babipPotential { 0 };
    int avoidKL { 0 };
    int avoidKR { 0 };
    int avoidKPotential { 0 };
    int gapL { 0 };
    int gapR { 0 };
    int gapPotential { 0 };
    int powerL { 0 };
    int powerR { 0 };
    int powerPotential { 0 };
    int eyeL { 0 };
    int eyeR { 0 };
    int eyePotential { 0 };
    int hbp { 0 };
    int speed { 0 };
    int steal { 0 };
    int baserunning { 0 };
    int buntSac { 0 };
    int buntHit { 0 };

    GbType gbType { GbType::Normal };
    FbType fbType { FbType::Normal };
};


static constexpr BattingAttributes pitcherBatting {
    .babipL = 1,
    .babipR = 1,
    .babipPotential = 1,
    .avoidKL = 1,
    .avoidKR = 1,
    .avoidKPotential = 1,
    .gapL = 1,
    .gapR = 1,
    .gapPotential = 1,
    .powerL = 1,
    .powerR = 1,
    .powerPotential = 1,
    .eyeL = 1,
    .eyeR = 1,
    .eyePotential = 1,
    .hbp = 1,

    .speed = 1,
    .steal = 1,
    .baserunning = 1,
    .buntSac = 1,
    .buntHit = 1,

    .gbType = GbType::Normal,
    .fbType = FbType::Normal,
};
