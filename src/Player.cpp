#include "Player.hpp"

#include <algorithm>
#include <array>
#include <iterator>
#include <stdexcept>
#include <string>
#include <string_view>

#include <fmt/core.h>

#include "Read.hpp"


auto show(BattingHandedness const& handedness) -> std::string {
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

    if (str == "Right"sv) {
        return BattingHandedness::Right;
    }
    if (str == "Left"sv) {
        return BattingHandedness::Left;
    }
    if (str == "Switch"sv) {
        return BattingHandedness::Switch;
    }

    throw std::runtime_error { fmt::format("Failed to parse batting handedness: {}"sv, str) };
}


auto show(ThrowingHandedness const& handedness) -> std::string {
    using namespace std::literals;

    switch (handedness) {
    case ThrowingHandedness::Right:
        return "Right"s;
    case ThrowingHandedness::Left:
        return "Left"s;
    }

    throw std::runtime_error { fmt::format("Unknown throwing handedness: {}"sv, static_cast<int>(handedness)) };
}

template <>
auto read<ThrowingHandedness>(std::string_view str) -> ThrowingHandedness {
    using namespace std::literals;

    if (str == "Right"sv) {
        return ThrowingHandedness::Right;
    }
    if (str == "Left"sv) {
        return ThrowingHandedness::Left;
    }

    throw std::runtime_error { fmt::format("Failed to parse throwing handedness: {}"sv, str) };
}


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

    static constexpr auto positions = std::to_array({
      "SP"sv,
      "RP"sv,
      "CP"sv,
      "C"sv,
      "1B"sv,
      "2B"sv,
      "3B"sv,
      "SS"sv,
      "LF"sv,
      "CF"sv,
      "RF"sv,
      "DH"sv,
    });

    auto const position = std::find(std::cbegin(positions), std::cend(positions), str);
    if (position == std::cend(positions)) {
        throw std::runtime_error { fmt::format("Failed to parse position: {}"sv, str) };
    }
    return static_cast<Position>(position - std::cbegin(positions));
}
