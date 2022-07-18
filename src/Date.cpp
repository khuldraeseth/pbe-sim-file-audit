#include "Date.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <iterator>
#include <sstream>
#include <string_view>
#include <unordered_map>

#include <boost/regex.hpp>
#include <boost/regex/v5/regex.hpp>
#include <boost/regex/v5/regex_match.hpp>
#include <fmt/core.h>


template <>
struct fmt::formatter<std::chrono::month_day> {
    static constexpr auto parse(format_parse_context& ctx) { return std::end(ctx); }

    template <typename FormatContext>
    auto format(std::chrono::month_day const& date, FormatContext& ctx) {
        return format_to(ctx.out(), "{:02}-{:02}", static_cast<unsigned int>(date.month()),
                         static_cast<unsigned int>(date.day()));
    }
};

auto show(std::chrono::month_day date) -> std::string {
    using namespace std::literals;

    return fmt::format("{}"sv, date);
}

template <>
auto read<std::chrono::year>(std::string_view str) -> std::chrono::year {
    using namespace std::literals;

    return std::chrono::year { std::stoi(std::string { str }) };
}

auto readMonthAbbreviation(std::string_view str) -> std::chrono::month {
    using namespace std::literals;

    static std::unordered_map<std::string_view, std::chrono::month> months {
        {      "Jan"sv,   std::chrono::January},
        {      "Feb"sv,  std::chrono::February},
        {      "Mar"sv,     std::chrono::March},
        {      "Apr"sv,     std::chrono::April},
        {      "May"sv,       std::chrono::May},
        {      "Jun"sv,      std::chrono::June},
        {      "Jul"sv,      std::chrono::July},
        {      "Aug"sv,    std::chrono::August},
        {      "Sep"sv, std::chrono::September},
        {      "Oct"sv,   std::chrono::October},
        {      "Nov"sv,  std::chrono::November},
        {      "Dec"sv,  std::chrono::December},

        {  "January"sv,   std::chrono::January},
        { "February"sv,  std::chrono::February},
        {    "March"sv,     std::chrono::March},
        {    "April"sv,     std::chrono::April},
        {      "May"sv,       std::chrono::May},
        {     "June"sv,      std::chrono::June},
        {     "July"sv,      std::chrono::July},
        {   "August"sv,    std::chrono::August},
        {"September"sv, std::chrono::September},
        {  "October"sv,   std::chrono::October},
        { "November"sv,  std::chrono::November},
        { "December"sv,  std::chrono::December},
    };

    if (not months.contains(str)) {
        throw std::runtime_error { fmt::format("Failed to parse month: {}"sv, str) };
    }
    return months.at(str);
}

template <>
auto read<std::chrono::month>(std::string_view str) -> std::chrono::month {
    using namespace std::literals;

    unsigned int constexpr monthMin = 1;
    unsigned int constexpr monthMax = 31;

    static boost::regex const monthAbbreviation { R"pcre(([A-Za-z]+))pcre"s };
    boost::smatch what {};
    if (boost::regex_match(std::string { str }, what, monthAbbreviation)) {
        return readMonthAbbreviation(str);
    }

    unsigned int const monthNumber = std::stoi(std::string { str });
    if (monthNumber < monthMin || monthNumber > monthMax) {
        throw std::runtime_error { fmt::format("Failed to parse month: {}"sv, str) };
    }
    return std::chrono::month { monthNumber };
}

template <>
auto read<std::chrono::day>(std::string_view str) -> std::chrono::day {
    using namespace std::literals;

    unsigned int constexpr dayMin = 1;
    unsigned int constexpr dayMax = 31;

    unsigned int const day = std::stoi(std::string { str });
    if (day < dayMin or day > dayMax) {
        throw std::runtime_error { fmt::format("Failed to parse day: {}"sv, str) };
    }
    return std::chrono::day { day };
}


template <>
auto read<std::chrono::month_day>(std::string_view str) -> std::chrono::month_day {
    using namespace std::literals;

    static boost::regex const wordsLong { R"pcre(\s*(\w+)\s+(\w+)(:?th)?,?\s+(\d+)\s+(\d+)\s*)pcre"s };
    static boost::regex const wordsMedium { R"pcre(\s*(\w+)\s+(\d+)(:?th)?,?\s+(\d+)\s*)pcre"s };
    static boost::regex const wordsShort { R"pcre(\s*(\w+)\s+(\d+)(:?th)?\s*)pcre"s };
    static boost::regex const ymd { R"pcre(\s*(\d{4})[-./](\d{1,2})[-./](\d{1,2})\s*)pcre"s };
    static boost::regex const mdy { R"pcre(\s*(\d{1,2})[-./](\d{1,2})[-./](\d{4})\s*)pcre"s };
    boost::smatch what;

    if (boost::regex_match(std::string { str }, what, wordsLong)) {
        auto const month = read<std::chrono::month>(std::string { what[2] });
        auto const day = read<std::chrono::day>(std::string { what[3] });
        return std::chrono::month_day { month, day };
    }
    if (boost::regex_match(std::string { str }, what, wordsMedium)) {
        auto const month = read<std::chrono::month>(std::string { what[1] });
        auto const day = read<std::chrono::day>(std::string { what[2] });
        return std::chrono::month_day { month, day };
    }
    if (boost::regex_match(std::string { str }, what, wordsShort)) {
        auto const month = read<std::chrono::month>(std::string { what[1] });
        auto const day = read<std::chrono::day>(std::string { what[2] });
        return std::chrono::month_day { month, day };
    }
    if (boost::regex_match(std::string { str }, what, ymd)) {
        auto const month = read<std::chrono::month>(std::string { what[2] });
        auto const day = read<std::chrono::day>(std::string { what[3] });
        return std::chrono::month_day { month, day };
    }
    if (boost::regex_match(std::string { str }, what, mdy)) {
        auto const month = read<std::chrono::month>(std::string { what[1] });
        auto const day = read<std::chrono::day>(std::string { what[2] });
        return std::chrono::month_day { month, day };
    }

    throw std::runtime_error { fmt::format("Failed to parse month_day: {}"sv, str) };
}
