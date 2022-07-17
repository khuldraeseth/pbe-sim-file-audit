#include "Height.hpp"

#include <string_view>

#include <boost/regex.hpp>
#include <fmt/core.h>

#include "Read.hpp"


static constexpr double inchesPerFoot = 12.0;
static constexpr double cmPerInch = 2.54;


auto show(Height height) -> std::string {
    return fmt::format("{} cm", height.cm);
}

template <>
auto read<Height>(std::string_view str) -> Height {
    using namespace std::literals;

    static boost::regex const feetInchesTicks { R"pcre((\d+)\s*'\s*(\d+)\s*"?)pcre" };
    static boost::regex const feetInchesWords { R"pcre((\d+)\s*f(?:ee)?t\s*(\d+)\s*in(?:ches)?)pcre" };
    static boost::regex const ernieHarwell { R"pcre((\d+))pcre" };
    boost::smatch what {};

    if (boost::regex_match(std::string { str }, what, feetInchesTicks)) {
        auto const feet = std::stoi(what[1]);
        auto const inches = std::stoi(what[2]);
        auto const cm = (feet * inchesPerFoot + inches) * cmPerInch;
        return Height { static_cast<int>(cm) };
    }

    if (boost::regex_match(std::string { str }, what, feetInchesWords)) {
        auto const feet = std::stoi(what[1]);
        auto const inches = std::stoi(what[2]);
        auto const cm = (feet * inchesPerFoot + inches) * cmPerInch;
        return Height { static_cast<int>(cm) };
    }

    if (boost::regex_match(std::string { str }, what, ernieHarwell)) {
        auto const feet = std::stoi(what[1]);
        auto const inches = 0;
        auto const cm = (feet * inchesPerFoot + inches) * cmPerInch;
        return Height { static_cast<int>(cm) };
    }

    throw std::runtime_error { fmt::format("Failed to parse height: {}"sv, str) };
}
