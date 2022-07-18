#include "PlayerFromForum.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <iterator>
#include <pugixml.hpp>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <unordered_set>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/find_iterator.hpp>
#include <boost/algorithm/string/finder.hpp>
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
#include "Xml.hpp"


using AttributeMap = std::unordered_map<std::string, std::string>;

template <typename Container, typename Key = typename Container::key_type,
          typename Value = typename Container::mapped_type>
auto valueOr(Container const& container, Key const& key, Value const& defaultValue) -> Value {
    if (not container.contains(key)) {
        return defaultValue;
    }

    return container.at(key);
}

template <typename Container, typename Value = typename Container::mapped_type>
auto valueOfAny(Container const&) -> Value {
    using namespace std::literals;

    throw std::out_of_range { "valueOfAny: no matching key found"s };
}

template <typename Container, typename Key, typename... Keys, typename Value = typename Container::mapped_type>
auto valueOfAny(Container const& container, Key const& key, Keys const&... keys) -> Value {
    if (not container.contains(key)) {
        return valueOfAny(container, keys...);
    }

    return container.at(key);
}


auto deparenthesize(std::string const& str) -> std::string {
    static boost::regex const pattern { R"pcre(\((?:.|(?R))*?\))pcre" };
    static boost::regex const patternUnbalanced { R"pcre(^[^(]*?\))pcre" };
    return boost::erase_all_regex_copy(boost::erase_all_regex_copy(str, pattern), patternUnbalanced);
}

auto debracketize(std::string const& str) -> std::string {
    static boost::regex const pattern { R"pcre(\[(?:.|(?R))*?\])pcre" };
    return boost::erase_all_regex_copy(str, pattern);
}

auto readAttributes(pugi::xpath_node_set const& nodes) -> AttributeMap {
    using namespace std::literals;

    AttributeMap attributes;

    for (auto it = std::begin(nodes); it != std::end(nodes); ++it) {
        auto const line = it->node().text().as_string();
        auto text = boost::trim_copy(debracketize(deparenthesize(line)));
        if (std::empty(text)) {
            continue;
        }

        static constexpr auto toIgnore = std::to_array({
          "College"sv,
          "Recruited by"sv,
          "Username"sv,
          "Number"sv,
          "Player Render"sv,
          "Discord name"sv,
          "Bank"sv,
          "Banked"sv,
          "Banked TPE"sv,
        });
        if (std::any_of(std::cbegin(toIgnore), std::cend(toIgnore),
                        [&text](auto const& s) { return boost::contains(text, s); })) {
            continue;
        }

        if (text.back() == ':') {
            static constexpr auto toIgnoreIfAlone = std::to_array({
              "Attributes"sv,
              "Positions"sv,
              "Birthdate"sv,
              "Recruited"sv,
            });
            if (std::any_of(std::cbegin(toIgnoreIfAlone), std::cend(toIgnoreIfAlone),
                            [&text](auto const& s) { return boost::contains(text, s); })) {
                continue;
            }

            ++it;
            if (it == std::end(nodes)) {
                text.pop_back();
                throw std::runtime_error { fmt::format("Failed to parse player page: key has no associated value: {}"sv,
                                                       text) };
            }
            auto const nextLine = it->node().text().as_string();
            text += boost::trim_copy(debracketize(deparenthesize(nextLine)));
        }

        static constexpr auto delimiter = ":"sv;
        if (not boost::contains(text, delimiter)) {
            continue;
        }

        auto const finder = boost::first_finder(delimiter);
        auto pieces = boost::make_split_iterator(text, finder);
        while (std::cbegin(*pieces) == std::cend(*pieces)) {
            ++pieces;
        }

        auto const key = boost::trim_copy(std::string { std::cbegin(*pieces), std::cend(*pieces) });
        ++pieces;
        while (std::cbegin(*pieces) == std::cend(*pieces)) {
            ++pieces;
        }

        auto const value = boost::trim_copy(std::string { std::cbegin(*pieces), std::cend(*pieces) });

        attributes[key] = value;
    }

    return attributes;
}

auto readName(AttributeMap const& info) -> std::string {
    using namespace std::literals;

    if (info.contains("Player Name"s)) {
        return info.at("Player Name"s);
    }

    if (info.contains("Name"s)) {
        return info.at("Name"s);
    }

    if (info.contains("First Name"s) and info.contains("Last Name"s)) {
        return fmt::format("{} {}", info.at("First Name"s), info.at("Last Name"s));
    }

    throw std::runtime_error { "Failed to read player name" };
}

auto isPitcher(Position position) -> bool {
    switch (position) {
    case Position::StartingPitcher:
    case Position::ReliefPitcher:
    case Position::ClosingPitcher:
        return true;
    default:
        return false;
    }
}

auto isPitcher(AttributeMap const& info) -> bool {
    using namespace std::literals;

    if (info.contains("Position"s)) {
        return isPitcher(read<Position>(info.at("Position"s)));
    }
    if (info.contains("Hitting Archetype"s)) {
        return false;
    }
    if (info.contains("Bats"s)) {
        return false;
    }
    if (info.contains("Hitting"s)) {
        return false;
    }
    if (info.contains("Velocity"s)) {
        return true;
    }
    if (info.contains("Speed"s)) {
        return false;
    }
    if (info.contains("Player Archetype"s)) {
        return true;
    }
    if (info.contains("Stamina"s)) {
        return true;
    }

    throw std::runtime_error { fmt::format("Failed to determine if player is a pitcher: {}"sv, readName(info)) };
}

auto isDayOfWeek(std::string_view str) -> bool {
    using namespace std::literals;

    static std::unordered_set const days {
        "Mon"sv, "Tue"sv, "Wed"sv, "Thu"sv, "Fri"sv, "Sat"sv, "Sun"sv,
    };
    return days.contains(str);
}


auto readBirthdate(std::string_view str) -> std::chrono::month_day {
    std::istringstream in { std::string { str } };
    std::string word {};
    in >> word;

    if (isDayOfWeek(word)) {
        std::string text {};
        std::getline(in, text);
        return read<std::chrono::month_day>(text);
    }

    return read<std::chrono::month_day>(str);
}

auto readCommon(AttributeMap const& info) -> CommonAttributes {
    using namespace std::literals;

    // Things break if I `return CommonAttributes { ... }` instead of creating a local variable.
    // idk why, but that's the reason I'm doing it this way here and in a few other functions.
    CommonAttributes result {
        .name = readName(info),
        .birthdate = readBirthdate(valueOr(info, "Birthdate"s, "1970-01-01"s)),
        .birthplace = info.at("Birthplace"s),
        .heightCm = read<Height>(info.at("Height"s)).cm,
        .weightLb = read<int>(info.at("Weight"s)),
        .throwingHandedness = read<ThrowingHandedness>(info.at("Throws"s)),
        // TODO: something better than this needed
        .position = read<Position>(valueOr(info, "Position"s, "SS"s)),
    };

    return result;
}

auto readBatting(AttributeMap const& info) -> BattingAttributes {
    using namespace std::literals;

    static constexpr auto defaultHbp = 5;

    auto const hittingTendency = valueOfAny(info, "Hitting"s, "Hitter Type"s);

    BattingAttributes result {
        .battingHandedness = read<BattingHandedness>(info.at("Bats"s)),

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

        .gbTendency = read<GbTendency>(hittingTendency),
        .fbTendency = read<FbTendency>(hittingTendency),
    };

    return result;
}

auto readBaseFieldingExperience(AttributeMap const& info) -> FieldingExperience {
    using namespace std::literals;

    auto const archetype = info.at("Hitting Archetype"s);
    if (archetype == "Mr. Utility"sv) {
        return mrUtilityFieldingExp;
    }
    return FieldingExperience {};
}

auto readPositionsTogether(AttributeMap const& info) -> std::tuple<Position, Position, Position> {
    using namespace std::literals;

    auto const text = info.at("Positions"s);

    static constexpr auto delimiter = ";"sv;
    auto const finder = boost::first_finder(delimiter);
    auto it = boost::make_split_iterator(text, finder);

    auto const position1 = read<Position>(boost::trim_copy(std::string { std::cbegin(*it), std::cend(*it) }));
    ++it;
    auto const position2 = read<Position>(boost::trim_copy(std::string { std::cbegin(*it), std::cend(*it) }));
    ++it;
    auto const position3 = read<Position>(boost::trim_copy(std::string { std::cbegin(*it), std::cend(*it) }));
    return std::make_tuple(position1, position2, position3);
}

auto readPositions(AttributeMap const& info) -> std::tuple<Position, Position, Position> {
    using namespace std::literals;

    std::vector<AttributeMap::value_type> const attributes(std::cbegin(info), std::cend(info));

    if (info.contains("Positions"s)) {
        return readPositionsTogether(info);
    }

    auto const position1 = read<Position>(info.at("1st Position"s));
    auto const position2 = read<Position>(info.at("2nd Position"s));
    auto const position3 = read<Position>(info.at("3rd Position"s));
    return std::make_tuple(position1, position2, position3);
}

auto readFieldingExperience(AttributeMap const& info) -> FieldingExperience {
    using namespace std::literals;

    static constexpr auto position1Exp = 200;
    static constexpr auto position2Exp = 150;
    static constexpr auto position3Exp = 100;

    auto result = readBaseFieldingExperience(info);

    auto const [position1, position2, position3] = readPositions(info);
    result[position1] = position1Exp;
    result[position2] = position2Exp;
    result[position3] = position3Exp;

    return result;
}

auto readFielding(AttributeMap const& info) -> FieldingAttributes {
    using namespace std::literals;

    FieldingAttributes result {
        .rangeInfield = read<int>(valueOfAny(info, "Fielding Range"s, "Range"s)),
        .rangeOutfield = result.rangeInfield,
        .errorInfield = read<int>(valueOfAny(info, "Fielding Error"s, "Error"s)),
        .errorOutfield = result.errorInfield,
        .armInfield = read<int>(valueOfAny(info, "Fielding/Catching Arm"s, "Arm"s)),
        .armOutfield = result.armInfield,
        .armCatcher = result.armInfield,
        .abilityCatcher = read<int>(info.at("Catcher Ability"s)),
        .tdp = read<int>(info.at("Turn Double Play"s)),

        .experience = readFieldingExperience(info),
    };

    return result;
}

auto readPitching(AttributeMap const& info) -> PitchingAttributes {
    using namespace std::literals;

    static constexpr auto defaultHbp = 5;
    static constexpr auto defaultWp = 5;
    static constexpr auto defaultBalk = 1;

    PitchingAttributes result {
        .velo = read<int>(info.at("Velocity"s)),
        .stamina = read<int>(info.at("Stamina"s)),
        .hold = read<int>(info.at("Holding Runners"s)),
        .armSlot = read<ArmSlot>(info.at("Arm Slot"s)),

        .movementL = read<int>(valueOfAny(info, "Movement vs LHB"s, "Movement vs. LHB"s)),
        .movementR = read<int>(valueOfAny(info, "Movement vs RHB"s, "Movement vs. RHB"s)),
        .movementPotential = std::min(result.movementL, result.movementR),
        .controlL = read<int>(valueOfAny(info, "Control vs LHB"s, "Control vs. LHB"s)),
        .controlR = read<int>(valueOfAny(info, "Control vs RHB"s, "Control vs. RHB"s)),
        .controlPotential = std::min(result.controlL, result.controlR),
        .hbp = defaultHbp,
        .wp = defaultWp,
        .balk = defaultBalk,
        .gb = read<int>(valueOfAny(info, "GB%"s, "Groundball Percentage"s)),

        .fastball = read<int>(valueOr(info, "Fastball"s, "0"s)),
        .sinker = read<int>(valueOr(info, "Sinker"s, "0"s)),
        .cutter = read<int>(valueOr(info, "Cutter"s, "0"s)),
        .curveball = read<int>(valueOr(info, "Curveball"s, "0"s)),
        .slider = read<int>(valueOr(info, "Slider"s, "0"s)),
        .changeup = read<int>(valueOr(info, "Changeup"s, "0"s)),
        .splitter = read<int>(valueOr(info, "Splitter"s, "0"s)),
        .forkball = read<int>(valueOr(info, "Forkball"s, "0"s)),
        .circleChange = read<int>(valueOr(info, "Circle Change"s, "0"s)),
        .screwball = read<int>(valueOr(info, "Screwball"s, "0"s)),
        .knuckleCurve = read<int>(valueOr(info, "Knuckle Curve"s, "0"s)),
        .knuckleball = read<int>(valueOr(info, "Knuckleball"s, "0"s)),
    };

    return result;
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
    if (isPitcher(info)) {
        auto result = readPitcher(info);
        return result;
    }

    auto result = readBatter(info);
    return result;
}

auto readPlayer(pugi::xpath_node_set const& post) -> Player {
    auto result = readPlayer(readAttributes(post));
    return result;
}

auto readPlayer(pugi::xml_document const& rosterPage) -> Player {
    using namespace std::literals;

    static constexpr auto path = "//table[@border='1'][1]/tr[2]//text()"sv;
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

    static constexpr auto urlPattern
      = "https://probaseballexperience.jcink.net/index.php?act=Print&client=printer&t={}"sv;
    auto result = readPlayer(fmt::format(urlPattern, topicId));
    return result;
}
