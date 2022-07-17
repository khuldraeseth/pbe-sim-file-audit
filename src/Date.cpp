#include "Date.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <iterator>
#include <sstream>
#include <string_view>

#include <fmt/core.h>


template <>
struct fmt::formatter<std::chrono::year_month_day> {
    static constexpr auto parse(format_parse_context& ctx) { return std::end(ctx); }

    template <typename FormatContext>
    auto format(std::chrono::year_month_day const& date, FormatContext& ctx) {
        return format_to(ctx.out(), "{:04}-{:02}-{:02}", static_cast<int>(date.year()),
                         static_cast<unsigned int>(date.month()), static_cast<unsigned int>(date.day()));
    }
};

auto show(std::chrono::year_month_day date) -> std::string {
    using namespace std::literals;

    return fmt::format("{}"sv, date);
}


template <>
auto read<std::chrono::year>(std::string_view str) -> std::chrono::year {
    using namespace std::literals;

    return std::chrono::year { std::stoi(std::string { str }) };
}

template <>
auto read<std::chrono::month>(std::string_view str) -> std::chrono::month {
    using namespace std::literals;

    static constexpr auto names = std::to_array({
      "Jan"sv,
      "Feb"sv,
      "Mar"sv,
      "Apr"sv,
      "May"sv,
      "Jun"sv,
      "Jul"sv,
      "Aug"sv,
      "Sep"sv,
      "Oct"sv,
      "Nov"sv,
      "Dec"sv,
    });

    auto const position = std::find(std::cbegin(names), std::cend(names), str);
    if (position == std::cend(names)) {
        throw std::runtime_error { fmt::format("Failed to parse month: {}"sv, str) };
    }
    return std::chrono::month { static_cast<unsigned int>(position - std::cbegin(names) + 1) };
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
auto read<std::chrono::year_month_day>(std::string_view str) -> std::chrono::year_month_day {
    std::string dayOfWeek;
    std::string year;
    std::string month;
    std::string day;
    std::stringstream { std::string { str } } >> dayOfWeek >> month >> day >> year;

    return std::chrono::year_month_day {
        read<std::chrono::year>(year),
        read<std::chrono::month>(month),
        read<std::chrono::day>(day),
    };
}
