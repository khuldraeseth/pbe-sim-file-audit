#include "Pitching.hpp"

#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>

#include <fmt/core.h>

#include "Read.hpp"


auto show(ArmSlot slot) -> std::string {
    using namespace std::literals;

    switch (slot) {
    case ArmSlot::OverTheTop:
        return "Over the top"s;
    case ArmSlot::Normal:
        return "Normal"s;
    case ArmSlot::Sidearm:
        return "Sidearm"s;
    case ArmSlot::Submarine:
        return "Submarine"s;
    }

    throw std::runtime_error(fmt::format("Unknown arm slot: {}"sv, static_cast<int>(slot)));
}

template <>
auto read<ArmSlot>(std::string_view str) -> ArmSlot {
    using namespace std::literals;

    static std::unordered_map<std::string_view, ArmSlot> const armSlots {
        {"Over the top"sv, ArmSlot::OverTheTop},
        {      "Normal"sv,     ArmSlot::Normal},
        {     "Sidearm"sv,    ArmSlot::Sidearm},
        {   "Submarine"sv,  ArmSlot::Submarine},

        {"Over the Top"sv, ArmSlot::OverTheTop},
        {"Over The Top"sv, ArmSlot::OverTheTop},
    };

    if (not armSlots.contains(str)) {
        throw std::runtime_error(fmt::format("Failed to parse arm slot: {}"sv, str));
    }
    return armSlots.at(str);
}
