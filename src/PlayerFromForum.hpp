#pragma once

#include <string_view>

#include "Player.hpp"
#include "Team.hpp"


auto readPlayer(int topicId) -> Player;

auto readTeam(std::string_view abbreviation) -> Team;
