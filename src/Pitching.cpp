#include "Pitching.hpp"

#include <string>
#include <string_view>

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

    if (str == "Over the top"sv) {
        return ArmSlot::OverTheTop;
    }
    if (str == "Normal"sv) {
        return ArmSlot::Normal;
    }
    if (str == "Sidearm"sv) {
        return ArmSlot::Sidearm;
    }
    if (str == "Submarine"sv) {
        return ArmSlot::Submarine;
    }
    throw std::runtime_error(fmt::format("Failed to parse arm slot: {}"sv, str));
}
