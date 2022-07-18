#include "Batting.hpp"

#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>

#include <fmt/core.h>

#include "Read.hpp"


auto show(GbTendency tendency) -> std::string {
    using namespace std::literals;

    switch (tendency) {
    case GbTendency::Spray:
        return "Spray"s;
    case GbTendency::Normal:
        return "Normal"s;
    case GbTendency::Pull:
        return "Pull"s;
    case GbTendency::ExtremePull:
        return "Extreme Pull"s;
    }

    throw std::runtime_error { fmt::format("Unknown ground ball tendency: {}"sv, static_cast<int>(tendency)) };
}

template <>
auto read<GbTendency>(std::string_view str) -> GbTendency {
    using namespace std::literals;

    static std::unordered_map<std::string_view, GbTendency> const gbTendencies {
        {       "Spray"sv,       GbTendency::Spray},
        {      "Normal"sv,      GbTendency::Normal},
        {        "Pull"sv,        GbTendency::Pull},
        {"Extreme Pull"sv, GbTendency::ExtremePull},

        {"Extreme pull"sv, GbTendency::ExtremePull},

        {       "spray"sv,       GbTendency::Spray},
        {      "normal"sv,      GbTendency::Normal},
        {        "pull"sv,        GbTendency::Pull},
        {"extreme pull"sv, GbTendency::ExtremePull},
    };

    if (not gbTendencies.contains(str)) {
        throw std::runtime_error { fmt::format("Failed to parse ground ball tendency: {}"sv, str) };
    }
    return gbTendencies.at(str);
}


auto show(FbTendency tendency) -> std::string {
    using namespace std::literals;

    switch (tendency) {
    case FbTendency::Spray:
        return "Spray"s;
    case FbTendency::Normal:
        return "Normal"s;
    case FbTendency::Pull:
        return "Pull"s;
    }

    throw std::runtime_error { fmt::format("Unknown fly ball tendency: {}"sv, static_cast<int>(tendency)) };
}

template <>
auto read<FbTendency>(std::string_view str) -> FbTendency {
    using namespace std::literals;

    static std::unordered_map<std::string_view, FbTendency> const fbTendencies {
        {       "Spray"sv,  FbTendency::Spray},
        {      "Normal"sv, FbTendency::Normal},
        {        "Pull"sv,   FbTendency::Pull},

        {       "spray"sv,  FbTendency::Spray},
        {      "normal"sv, FbTendency::Normal},
        {        "pull"sv,   FbTendency::Pull},

 // Weird case—Extreme Pull hitters have Pull fly ball tendency because Extreme Pull is not an option.
        {"Extreme Pull"sv,   FbTendency::Pull},
        {"Extreme pull"sv,   FbTendency::Pull},
        {"extreme pull"sv,   FbTendency::Pull},
    };

    if (not fbTendencies.contains(str)) {
        throw std::runtime_error { fmt::format("Failed to parse fly ball tendency: {}"sv, str) };
    }
    return fbTendencies.at(str);
}


auto show(BattingHandedness handedness) -> std::string {
    using namespace std::literals;

    switch (handedness) {
    case BattingHandedness::Right:
        return "Right"s;
    case BattingHandedness::Left:
        return "Left"s;
    case BattingHandedness::Switch:
        return "Switch"s;
    }

    throw std::runtime_error { fmt::format("Unknown batting handedness: {}"sv, static_cast<int>(handedness)) };
}

template <>
auto read<BattingHandedness>(std::string_view str) -> BattingHandedness {
    using namespace std::literals;

    static std::unordered_map<std::string_view, BattingHandedness> const handednesses {
        { "Right"sv, BattingHandedness::Right},
        {  "Left"sv,  BattingHandedness::Left},
        {"Switch"sv,  BattingHandedness::Left},

        {     "R"sv, BattingHandedness::Right},
        {     "L"sv,  BattingHandedness::Left},
        {     "S"sv,  BattingHandedness::Left},
    };

    if (not handednesses.contains(str)) {
        throw std::runtime_error { fmt::format("Failed to parse batting handedness: {}"sv, str) };
    }

    return handednesses.at(str);
}
