#pragma once

#include <string_view>

#include "Team.hpp"


auto readTeam(std::string_view abbreviation) -> Team;
