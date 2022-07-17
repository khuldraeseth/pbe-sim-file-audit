#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "Player.hpp"
#include "Read.hpp"


enum class Franchise {
    NYV,
    OBX,
    PRO,
    SAS,
    DVS,
    VAN,
    ANC,
    FLA,
    SCSS,
    AMA,
    SAR,
    BOI,
    CUN,
    NSH,
    KC,
    PR,
    CAL,
    CHI,
    NO,
    DET,
    IND,
    MAU,
    BCB,
    LL,
    BUF,
    SEA,
};

auto show(Franchise franchise) -> std::string;

template <>
auto read<Franchise>(std::string_view str) -> Franchise;

auto nameOf(Franchise franchise) -> std::string;


struct Team {
    Franchise franchise {};
    std::vector<Player> players {};
};
