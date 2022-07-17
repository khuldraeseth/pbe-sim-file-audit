#include <iostream>

#include "FromForum.hpp"


auto main() -> int {
    std::cout << "Entry point not yet implemented" << std::endl;

    static constexpr auto topicId = 18379;
    auto evok = readPlayer(topicId);

    return 0;
}
