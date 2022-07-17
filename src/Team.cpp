#include "Team.hpp"

#include <algorithm>
#include <array>
#include <iterator>
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

    static auto constexpr abbreviations = std::to_array({
      "NYV"sv, "OBX"sv, "PRO"sv, "SAS"sv, "DVS"sv, "VAN"sv, "ANC"sv, "FLA"sv, "SCSS"sv,
      "AMA"sv, "SAR"sv, "BOI"sv, "CUN"sv, "NSH"sv, "KC"sv,  "PR"sv,  "CAL"sv, "CHI"sv,
      "NO"sv,  "DET"sv, "IND"sv, "MAU"sv, "BCB"sv, "LL"sv,  "BUF"sv, "SEA"sv,
    });

    auto const it = std::find(std::cbegin(abbreviations), std::cend(abbreviations), str);
    if (it == std::cend(abbreviations)) {
        throw std::runtime_error(fmt::format("Failed to parse franchise: {}"sv, str));
    }

    return static_cast<Franchise>(std::distance(std::cbegin(abbreviations), it));
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
        { Franchise::CAL,       "California Firehawks"s},
        { Franchise::CHI,           "Chicago Kingpins"s},
        {  Franchise::NO,      "New Orleans Rougarous"s},
        { Franchise::DET,             "Detroit Demons"s},
        { Franchise::IND,          "Indianapolis Apex"s},
        { Franchise::MAU,                 "Maui Makos"s},
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
