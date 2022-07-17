#include "TeamFromForum.hpp"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <iterator>
#include <locale>
#include <sstream>
#include <string_view>
#include <vector>

#include <cpr/api.h>
#include <cpr/cprtypes.h>
#include <fmt/core.h>

#include "Player.hpp"
#include "PlayerFromForum.hpp"
#include "Read.hpp"
#include "Team.hpp"
#include "Tidy.hpp"
#include "Xml.hpp"


namespace fs = std::filesystem;


auto makeLowercase(std::string_view str) -> std::string {
    std::string result { str };
    std::transform(std::begin(result), std::end(result), std::begin(result),
                   [](char c) { return std::tolower(c, std::locale {}); });
    return result;
}

auto makeSnakeCase(std::string_view str) -> std::string {
    std::istringstream in { std::string { str } };
    std::ostringstream out;

    std::transform(std::istream_iterator<std::string> { in }, std::istream_iterator<std::string> {},
                   std::ostream_iterator<std::string> { out, "_" }, makeLowercase);
    auto result = out.str();
    result.pop_back();
    return result;
}

auto readTopicId(pugi::xpath_node const& node) -> int {
    auto const text = node.node().attribute("href").value();
    auto const filename = fs::path { text }.filename().string();
    return read<int>(filename);
}

auto readTopicIds(pugi::xpath_node_set const& nodes) -> std::vector<int> {
    std::vector<int> result {};
    std::transform(std::cbegin(nodes), std::cend(nodes), std::back_inserter(result), readTopicId);
    return result;
}

auto readTopicIds(pugi::xml_document const& trackerPage) -> std::vector<int> {
    using namespace std::literals;

    // trackerPage.print(std::cout);

    static constexpr auto path = "//table[@id='playerTable']/tbody/tr/td[3]/a"sv;
    auto const nodes = trackerPage.select_nodes(path.data());
    auto result = readTopicIds(nodes);
    return result;

    // return {};
}

auto readTopicIds(cpr::Url const& url) -> std::vector<int> {
    auto const response = cpr::Get(url);
    auto const text = tidy(response.text);
    auto const doc = parseXmlString(text);

    auto result = readTopicIds(doc);
    return result;
}

auto readTopicIds(std::string_view name) -> std::vector<int> {
    using namespace std::literals;

    static constexpr auto trackerUrlPattern = "https://pbe-tpe-tracker.herokuapp.com/teams/{}"sv;

    auto result = readTopicIds(cpr::Url { fmt::format(trackerUrlPattern, name) });
    return result;
}

auto readTopicIds(Franchise franchise) -> std::vector<int> {
    auto const name = nameOf(franchise);
    auto result = readTopicIds(std::string_view { makeSnakeCase(name) });
    return result;
}

auto readPlayers(Franchise franchise) -> std::vector<Player> {
    auto const topicIds = readTopicIds(franchise);

    std::vector<Player> result {};
    std::transform(std::cbegin(topicIds), std::cend(topicIds), std::back_inserter(result),
                   [&](auto const topicId) { return readPlayer(topicId); });
    return result;
}

auto readTeam(Franchise franchise) -> Team {
    Team result {
        .franchise = franchise,
        .players = readPlayers(franchise),
    };
    return result;
}

auto readTeam(std::string_view abbreviation) -> Team {
    auto result = readTeam(read<Franchise>(abbreviation));
    return result;
}
