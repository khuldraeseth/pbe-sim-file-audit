#include "Player.hpp"

#include <stdexcept>
#include <string>
#include <string_view>

#include <fmt/core.h>

#include "Read.hpp"


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
