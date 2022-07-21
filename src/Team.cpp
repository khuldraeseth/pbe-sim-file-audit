#include "Team.hpp"

#include <string>
#include <string_view>
#include <unordered_map>

#include <fmt/core.h>

#include "Read.hpp"


auto show(Franchise franchise) -> std::string {
    using namespace std::literals;

    switch (franchise) {
    case Franchise::NYV:
        return "NYV"s;
    case Franchise::OBX:
        return "OBX"s;
    case Franchise::PRO:
        return "PRO"s;
    case Franchise::SAS:
        return "SAS"s;
    case Franchise::DVS:
        return "DVS"s;
    case Franchise::VAN:
        return "VAN"s;
    case Franchise::ANC:
        return "ANC"s;
    case Franchise::FLA:
        return "FLA"s;
    case Franchise::SCSS:
        return "SCSS"s;
    case Franchise::AMA:
        return "AMA"s;
    case Franchise::SAR:
        return "SAR"s;
    case Franchise::BOI:
        return "BOI"s;
    case Franchise::CUN:
        return "CUN"s;
    case Franchise::NSH:
        return "NSH"s;
    case Franchise::KC:
        return "KC"s;
    case Franchise::PR:
        return "PR"s;
    case Franchise::CAL:
        return "CAL"s;
    case Franchise::CHI:
        return "CHI"s;
    case Franchise::NO:
        return "NO"s;
    case Franchise::DET:
        return "DET"s;
    case Franchise::IND:
        return "IND"s;
    case Franchise::MAU:
        return "MAU"s;
    case Franchise::BCB:
        return "BCB"s;
    case Franchise::LL:
        return "LL"s;
    case Franchise::BUF:
        return "BUF"s;
    case Franchise::SEA:
        return "SEA"s;
    }

    throw std::runtime_error(fmt::format("Unknown franchise: {}"sv, static_cast<int>(franchise)));
}

template <>
auto read<Franchise>(std::string_view str) -> Franchise {
    using namespace std::literals;

    static std::unordered_map<std::string_view, Franchise> const franchises {
        { "NYV"sv,  Franchise::NYV},
        { "OBX"sv,  Franchise::OBX},
        { "PRO"sv,  Franchise::PRO},
        { "SAS"sv,  Franchise::SAS},
        { "DVS"sv,  Franchise::DVS},
        { "VAN"sv,  Franchise::VAN},
        { "ANC"sv,  Franchise::ANC},
        { "FLA"sv,  Franchise::FLA},
        {"SCSS"sv, Franchise::SCSS},
        { "AMA"sv,  Franchise::AMA},
        { "SAR"sv,  Franchise::SAR},
        { "BOI"sv,  Franchise::BOI},
        { "CUN"sv,  Franchise::CUN},
        { "NSH"sv,  Franchise::NSH},
        {  "KC"sv,   Franchise::KC},
        {  "PR"sv,   Franchise::PR},
        { "CAL"sv,  Franchise::CAL},
        { "CHI"sv,  Franchise::CHI},
        {  "NO"sv,   Franchise::NO},
        { "DET"sv,  Franchise::DET},
        { "IND"sv,  Franchise::IND},
        { "MAU"sv,  Franchise::MAU},
        { "BCB"sv,  Franchise::BCB},
        {  "LL"sv,   Franchise::LL},
        { "BUF"sv,  Franchise::BUF},
        { "SEA"sv,  Franchise::SEA},
    };

    if (not franchises.contains(str)) {
        throw std::runtime_error(fmt::format("Failed to parse franchise: {}"sv, str));
    }

    return franchises.at(str);
}

auto nameOf(Franchise franchise) -> std::string {
    using namespace std::literals;

    static std::unordered_map<Franchise, std::string> const names {
        { Franchise::NYV,          "New York Voyagers"s},
        { Franchise::OBX,       "Outer Banks Aviators"s},
        { Franchise::PRO,           "Providence Crabs"s},
        { Franchise::SAS,         "San Antonio Sloths"s},
        { Franchise::DVS,     "Death Valley Scorpions"s},
        { Franchise::VAN,          "Vancouver Vandals"s},
        { Franchise::ANC,         "Anchorage Wheelers"s},
        { Franchise::FLA,          "Florida Flamingos"s},
        {Franchise::SCSS, "State College Swift Steeds"s},
        { Franchise::AMA,        "Amarillo Armadillos"s},
        { Franchise::SAR,        "Sarasota Supernovas"s},
        { Franchise::BOI,              "Boise Raptors"s},
        { Franchise::CUN,               "Cancun Toros"s},
        { Franchise::NSH,            "Nashville Stars"s},
        {  Franchise::KC,        "Kansas City Hepcats"s},
        {  Franchise::PR,          "Puerto Rico Ranas"s},
        { Franchise::CAL,       "San Bernardino 66ers"s}, // bruh, Hydra
        { Franchise::CHI,           "Chicago Kingpins"s},
        {  Franchise::NO,      "New Orleans Rougarous"s},
        { Franchise::DET,             "Detroit Demons"s},
        { Franchise::IND,          "Indianapolis Apex"s},
        { Franchise::MAU,              "Kashima Foxes"s},
        { Franchise::BCB,            "Brew City Bears"s},
        {  Franchise::LL,          "Louisville Lemurs"s},
        { Franchise::BUF,              "Buffalo Surge"s},
        { Franchise::SEA,       "Seattle Sea Serpents"s},
    };

    if (not names.contains(franchise)) {
        throw std::runtime_error(fmt::format("Unknown franchise: {}"sv, static_cast<int>(franchise)));
    }

    return names.at(franchise);
}
