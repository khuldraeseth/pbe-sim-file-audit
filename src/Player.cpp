#include "Player.hpp"

#include <stdexcept>
#include <string>
#include <string_view>

#include <fmt/core.h>

#include "Read.hpp"


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


auto show(ThrowingHandedness handedness) -> std::string {
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
