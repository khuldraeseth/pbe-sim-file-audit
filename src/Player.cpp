#include "Player.hpp"

#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>

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

    static std::unordered_map<std::string_view, ThrowingHandedness> const handednesses {
        {"Right"sv, ThrowingHandedness::Right},
        { "Left"sv,  ThrowingHandedness::Left},

        {    "R"sv, ThrowingHandedness::Right},
        {    "L"sv,  ThrowingHandedness::Left},
    };

    if (not handednesses.contains(str)) {
        throw std::runtime_error { fmt::format("Failed to parse throwing handedness: {}"sv, str) };
    }

    return handednesses.at(str);
}
