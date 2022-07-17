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
#include "Fielding.hpp"
#include "Height.hpp"
#include "Pitching.hpp"
#include "Player.hpp"
#include "Tidy.hpp"


using AttributeMap = std::unordered_map<std::string, std::string>;


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

    // Things break if I `return CommonAttributes { ... }` instead of creating a local variable.
    // idk why, but that's the reason I'm doing it this way here and in a few other functions.
    CommonAttributes result {
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

auto readBatting(AttributeMap const& info) -> BattingAttributes {
    using namespace std::literals;

    static constexpr auto defaultHbp = 5;

    BattingAttributes result {
        .babipL = read<int>(info.at("BABIP vs LHP"s)),
        .babipR = read<int>(info.at("BABIP vs RHP"s)),
        .babipPotential = std::min(result.babipL, result.babipR),
        .avoidKL = read<int>(info.at("Avoid K's vs LHP"s)),
        .avoidKR = read<int>(info.at("Avoid K's vs RHP"s)),
        .avoidKPotential = std::min(result.avoidKL, result.avoidKR),
        .gapL = read<int>(info.at("Gap vs LHP"s)),
        .gapR = read<int>(info.at("Gap vs RHP"s)),
        .gapPotential = std::min(result.gapL, result.gapR),
        .powerL = read<int>(info.at("Power vs LHP"s)),
        .powerR = read<int>(info.at("Power vs RHP"s)),
        .powerPotential = std::min(result.powerL, result.powerR),
        .eyeL = read<int>(info.at("Eye/Patience vs LHP"s)),
        .eyeR = read<int>(info.at("Eye/Patience vs RHP"s)),
        .eyePotential = std::min(result.eyeL, result.eyeR),
        .hbp = defaultHbp,
        .speed = read<int>(info.at("Speed"s)),
        .steal = read<int>(info.at("Stealing Ability"s)),
        .baserunning = result.speed,
        .buntSac = read<int>(info.at("Bunting"s)),
        .buntHit = result.buntSac,

        .gbTendency = read<GbTendency>(info.at("Hitting"s)),
        .fbTendency = read<FbTendency>(info.at("Hitting"s)),
    };

    return result;
}

auto readBaseFieldingExperience(AttributeMap const& info) -> FieldingExperience {
    using namespace std::literals;

    auto const archetype = info.at("Archetype"s);
    if (archetype == "Mr. Utility"sv) {
        return mrUtilityFieldingExp;
    }
    return FieldingExperience {};
}

auto readFieldingExperience(AttributeMap const& info) -> FieldingExperience {
    using namespace std::literals;

    static constexpr auto position1Exp = 200;
    static constexpr auto position2Exp = 150;
    static constexpr auto position3Exp = 100;

    auto result = readBaseFieldingExperience(info);

    auto const position1 = read<Position>(info.at("1st Position"s));
    result[position1] = position1Exp;

    auto const position2 = read<Position>(info.at("2nd Position"s));
    result[position2] = position2Exp;

    auto const position3 = read<Position>(info.at("3rd Position"s));
    result[position3] = position3Exp;

    return result;
}

auto readFielding(AttributeMap const& info) -> FieldingAttributes {
    using namespace std::literals;

    FieldingAttributes result {
        .rangeInfield = read<int>(info.at("Fielding Range"s)),
        .rangeOutfield = result.rangeInfield,
        .errorInfield = read<int>(info.at("Fielding Error"s)),
        .errorOutfield = result.errorInfield,
        .armInfield = read<int>(info.at("Fielding/Catching Arm"s)),
        .armOutfield = result.armInfield,
        .armCatcher = result.armInfield,
        .abilityCatcher = read<int>(info.at("Catcher Ability"s)),
        .tdp = read<int>(info.at("Turn Double Play"s)),

        .experience = readFieldingExperience(info),
    };

    return result;
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
    auto result = Player {
        .commonAttributes = readCommon(info),
        .battingAttributes = pitcherBatting,
        .fieldingAttributes = pitcherFielding,
        .pitchingAttributes = readPitching(info),
    };

    return result;
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
