#include "FromForum.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <iterator>
#include <pugixml.hpp>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/regex.hpp>
#include <cpr/cpr.h>
#include <fmt/core.h>

#include "Batting.hpp"
#include "Date.hpp"
#include "Height.hpp"
#include "Pitching.hpp"
#include "Player.hpp"
#include "Tidy.hpp"


using AttributeMap = std::map<std::string, std::string>;


auto parseXmlString(std::string_view text) -> pugi::xml_document {
    using namespace std::literals;

    pugi::xml_document doc;
    auto const result = doc.load_string(text.data());
    if (!result) {
        throw std::runtime_error { fmt::format("Failed to parse XML: {}"sv, result.description()) };
    }
    return doc;
}

auto deparenthesize(std::string const& str) -> std::string {
    // possible TODO: use a Finder instead of a regex
    static boost::regex const pattern { R"pcre(\((?:.|(?R))*?\))pcre" };
    return boost::erase_all_regex_copy(str, pattern);
}

auto readAttributes(pugi::xpath_node_set const& nodes) -> AttributeMap {
    using namespace std::literals;

    AttributeMap attributes;

    for (auto it = std::begin(nodes); it != std::end(nodes); ++it) {
        auto const line = it->node().text().as_string();
        auto text = boost::trim_copy(deparenthesize(line));
        if (std::empty(text)) {
            continue;
        }

        static constexpr auto toIgnore = std::to_array({
          "Attributes"sv,
          "Positions"sv,
          "College"sv,
          "Recruited by"sv,
          "Username"sv,
          "Number"sv,
          "Player Render"sv,
          "Discord name"sv,
        });
        if (std::any_of(std::begin(toIgnore), std::end(toIgnore),
                        [&text](auto const& s) { return boost::contains(text, s); })) {
            continue;
        }

        if (text.back() == ':') {
            ++it;
            if (it == std::end(nodes)) {
                text.pop_back();
                throw std::runtime_error { fmt::format("Failed to parse player page: key has no associated value: {}"sv,
                                                       text) };
            }
            auto const nextLine = it->node().text().as_string();
            text += boost::trim_copy(deparenthesize(nextLine));
        }

        static constexpr auto delimiter { ":" };
        if (not boost::contains(text, delimiter)) {
            continue;
        }

        auto const finder = boost::first_finder(delimiter);
        auto pieces = boost::make_split_iterator(text, finder);
        auto const key = boost::trim_copy(std::string { std::begin(*pieces), std::end(*pieces) });
        ++pieces;
        auto const value = boost::trim_copy(std::string { std::begin(*pieces), std::end(*pieces) });
        attributes[key] = value;
    }

    return attributes;
}

auto readCommon(AttributeMap const& info) -> CommonAttributes {
    using namespace std::literals;

    // for (auto const& [key, value] : info) {
    //     std::cout << key << ": " << value << std::endl;
    // }

    auto result = CommonAttributes {
        .name = info.at("Player Name"s),
        .birthdate = read<std::chrono::year_month_day>(info.at("Birthdate"s)),
        .birthplace = info.at("Birthplace"s),
        .heightCm = read<Height>(info.at("Height"s)).cm,
        .weightLb = read<int>(info.at("Weight"s)),
        .battingHandedness = read<BattingHandedness>(info.at("Bats"s)),
        .throwingHandedness = read<ThrowingHandedness>(info.at("Throws"s)),
        .position = read<Position>(info.at("Position"s)),
    };

    return result;
}

auto readBatting(AttributeMap const&) -> BattingAttributes {
    // TODO
    return {};
}

auto readFielding(AttributeMap const&) -> FieldingAttributes {
    // TODO
    return {};
}

auto readPitching(AttributeMap const&) -> PitchingAttributes {
    // TODO
    return {};
}

auto readBatter(AttributeMap const& info) -> Player {
    auto result = Player {
        .commonAttributes = readCommon(info),
        .battingAttributes = readBatting(info),
        .fieldingAttributes = readFielding(info),
        .pitchingAttributes = batterPitching,
    };

    return result;
}

auto readPitcher(AttributeMap const& info) -> Player {
    return Player {
        .commonAttributes = readCommon(info),
        .battingAttributes = pitcherBatting,
        .fieldingAttributes = pitcherFielding,
        .pitchingAttributes = readPitching(info),
    };
}

auto readPlayer(AttributeMap const& info) -> Player {
    switch (read<Position>(info.at("Position"))) {
    case Position::StartingPitcher:
    case Position::ReliefPitcher:
    case Position::ClosingPitcher:
        return readPitcher(info);
    case Position::Catcher:
    case Position::FirstBase:
    case Position::SecondBase:
    case Position::ThirdBase:
    case Position::Shortstop:
    case Position::LeftField:
    case Position::CenterField:
    case Position::RightField:
    case Position::DesignatedHitter: {
        auto result = readBatter(info);
        return result;
    }
    }

    throw std::runtime_error { fmt::format("Failed to parse player page: unknown position: {}", info.at("Position")) };
}

auto readPlayer(pugi::xpath_node_set const& post) -> Player {
    auto result = readPlayer(readAttributes(post));
    return result;
}

auto readPlayer(pugi::xml_document const& rosterPage) -> Player {
    using namespace std::literals;

    static constexpr auto path { "//table[@border='1'][1]/tr[2]//text()"sv };
    auto const nodes = rosterPage.select_nodes(path.data());
    auto result = readPlayer(nodes);
    return result;
}

auto readPlayer(cpr::Url const& url) -> Player {
    auto const response = cpr::Get(url);
    auto const text = tidy(response.text);
    auto const doc = parseXmlString(text);
    auto result = readPlayer(doc);
    return result;
}

auto readPlayer(int topicId) -> Player {
    using namespace std::literals;

    static constexpr auto urlPattern {
        "https://probaseballexperience.jcink.net/index.php?act=Print&client=printer&t={}"sv
    };
    auto result = readPlayer(fmt::format(urlPattern, topicId));
    return result;
}
