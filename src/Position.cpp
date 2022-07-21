#include "Position.hpp"

#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>

#include <fmt/core.h>

#include "Read.hpp"


auto show(Position position) -> std::string {
    using namespace std::literals;

    switch (position) {
    case Position::StartingPitcher:
        return "SP"s;
    case Position::ReliefPitcher:
        return "RP"s;
    case Position::ClosingPitcher:
        return "CP"s;
    case Position::Catcher:
        return "C"s;
    case Position::FirstBase:
        return "1B"s;
    case Position::SecondBase:
        return "2B"s;
    case Position::ThirdBase:
        return "3B"s;
    case Position::Shortstop:
        return "SS"s;
    case Position::LeftField:
        return "LF"s;
    case Position::CenterField:
        return "CF"s;
    case Position::RightField:
        return "RF"s;
    case Position::DesignatedHitter:
        return "DH"s;
    }

    throw std::runtime_error { fmt::format("Unknown position: {}"sv, static_cast<int>(position)) };
}

template <>
auto read<Position>(std::string_view str) -> Position {
    using namespace std::literals;

    static std::unordered_map<std::string_view, Position> const positions {
        {               "SP"sv,  Position::StartingPitcher},
        {               "RP"sv,    Position::ReliefPitcher},
        {               "CL"sv,   Position::ClosingPitcher},
        {                "C"sv,          Position::Catcher},
        {               "1B"sv,        Position::FirstBase},
        {               "2B"sv,       Position::SecondBase},
        {               "3B"sv,        Position::ThirdBase},
        {               "SS"sv,        Position::Shortstop},
        {               "LF"sv,        Position::LeftField},
        {               "CF"sv,      Position::CenterField},
        {               "RF"sv,       Position::RightField},
        {               "DH"sv, Position::DesignatedHitter},

        { "Starting Pitcher"sv,  Position::StartingPitcher},
        {   "Relief Pitcher"sv,    Position::ReliefPitcher},
        {  "Closing Pitcher"sv,   Position::ClosingPitcher},
        {          "Catcher"sv,          Position::Catcher},
        {       "First Base"sv,        Position::FirstBase},
        {    "First Baseman"sv,        Position::FirstBase},
        {         "1st Base"sv,        Position::FirstBase},
        {      "Second Base"sv,       Position::SecondBase},
        {   "Second Baseman"sv,       Position::SecondBase},
        {         "2nd Base"sv,       Position::SecondBase},
        {       "Third Base"sv,        Position::ThirdBase},
        {    "Third Baseman"sv,        Position::ThirdBase},
        {         "3rd Base"sv,        Position::ThirdBase},
        {        "Shortstop"sv,        Position::Shortstop},
        {       "Left Field"sv,        Position::LeftField},
        {     "Left Fielder"sv,        Position::LeftField},
        {     "Center Field"sv,      Position::CenterField},
        {   "Center Fielder"sv,      Position::CenterField},
        {      "Right Field"sv,       Position::RightField},
        {    "Right Fielder"sv,       Position::RightField},
        {"Designated Hitter"sv, Position::DesignatedHitter},

        {            "RP/CL"sv,   Position::ClosingPitcher},
    };

    if (not positions.contains(str)) {
        throw std::runtime_error { fmt::format("Failed to parse position: {}"sv, str) };
    }
    return positions.at(str);
}
