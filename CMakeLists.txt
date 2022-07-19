cmake_minimum_required(VERSION 3.18)

project(sfa VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

cmake_policy(SET CMP0115 NEW)

# configure_file(Config.hpp.in Config.hpp)
# target_include_directories(sfa PUBLIC "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")

link_libraries(cpr fmt pugixml tidy)

add_executable(sfa
    src/sfa.cpp
    src/Batting.cpp
    src/Date.cpp
    src/Fielding.cpp
    src/Height.cpp
    src/Pitching.cpp
    src/Player.cpp
    src/PlayerFromForum.cpp
    src/Position.cpp
    src/Read.cpp
    src/Team.cpp
    src/TeamFromForum.cpp
    src/Tidy.cpp
    src/Xml.cpp
)
