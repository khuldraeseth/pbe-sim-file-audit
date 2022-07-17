#include "Batting.hpp"

#include <stdexcept>
#include <string>
#include <string_view>

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

    if (str == "Spray"sv) {
        return GbTendency::Spray;
    }
    if (str == "Normal"sv) {
        return GbTendency::Normal;
    }
    if (str == "Pull"sv) {
        return GbTendency::Pull;
    }
    if (str == "Extreme Pull"sv) {
        return GbTendency::ExtremePull;
    }
    throw std::runtime_error { fmt::format("Failed to parse ground ball tendency: {}"sv, str) };
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

    if (str == "Spray"sv) {
        return FbTendency::Spray;
    }
    if (str == "Normal"sv) {
        return FbTendency::Normal;
    }
    if (str == "Pull"sv) {
        return FbTendency::Pull;
    }
    // Weird case—Extreme Pull hitters have Pull fly ball tendency because Extreme Pull is not an option.
    if (str == "Extreme Pull"sv) {
        return FbTendency::Pull;
    }
    throw std::runtime_error { fmt::format("Failed to parse fly ball tendency: {}"sv, str) };
}
