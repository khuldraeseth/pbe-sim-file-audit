#pragma once


struct FieldingExperience {
    int expPitcher { 0 };
    int expCatcher { 0 };
    int exp1B { 0 };
    int exp2B { 0 };
    int exp3B { 0 };
    int expSS { 0 };
    int expLF { 0 };
    int expCF { 0 };
    int expRF { 0 };
};

struct FieldingAttributes {
    int rangeInfield { 0 };
    int rangeOutfield { 0 };
    int errorInfield { 0 };
    int errorOutfield { 0 };
    int armInfield { 0 };
    int armOutfield { 0 };
    int armCatcher { 0 };
    int abilityCatcher { 0 };
    int tdp { 0 };

    FieldingExperience experience {};
};


static constexpr FieldingExperience pitcherFieldingExp {
    .expPitcher = 200,
    .expCatcher = 0,
    .exp1B = 0,
    .exp2B = 0,
    .exp3B = 0,
    .expSS = 0,
    .expLF = 0,
    .expCF = 0,
    .expRF = 0,
};

static constexpr FieldingAttributes pitcherFielding {
    .rangeInfield = 75,
    .rangeOutfield = 75,
    .errorInfield = 50,
    .errorOutfield = 50,
    .armInfield = 100,
    .armOutfield = 100,
    .armCatcher = 100,
    .abilityCatcher = 10,
    .tdp = 25,

    .experience = pitcherFieldingExp,
};
