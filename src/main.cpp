#include <iostream>

#include "PlayerFromForum.hpp"
#include "Team.hpp"
#include "TeamFromForum.hpp"


auto main() -> int {
    using namespace std::literals;

    std::cout << "Entry point not yet implemented"sv << std::endl;

    // static constexpr auto topicId = 24107;
    // auto const player = readPlayer(topicId);

    static constexpr auto franchise = "BCB"sv;
    auto const team = readTeam(franchise);

    return 0;
}
