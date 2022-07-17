#include "Fielding.hpp"

#include <stdexcept>
#include <string>

#include <fmt/core.h>

#include "Position.hpp"


auto FieldingExperience::operator[](Position position) -> int& {
    using namespace std::literals;

    switch (position) {
    case Position::StartingPitcher:
    case Position::ReliefPitcher:
    case Position::ClosingPitcher:
        return expPitcher;
    case Position::Catcher:
        return expCatcher;
    case Position::FirstBase:
        return exp1B;
    case Position::SecondBase:
        return exp2B;
    case Position::ThirdBase:
        return exp3B;
    case Position::Shortstop:
        return expSS;
    case Position::LeftField:
        return expLF;
    case Position::CenterField:
        return expCF;
    case Position::RightField:
        return expRF;
    case Position::DesignatedHitter:
        throw std::runtime_error { "Invalid position for FieldingExperience::operator[]: DH"s };
    }

    throw std::runtime_error { fmt::format("Unknown position: {}"sv, static_cast<int>(position)) };
}
