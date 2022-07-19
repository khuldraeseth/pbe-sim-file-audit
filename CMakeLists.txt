cmake_minimum_required(VERSION 3.18)

project(sfa VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(DEFAULT_BUILD_TYPE "Debug")

cmake_policy(SET CMP0115 NEW)

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

target_link_libraries(sfa PRIVATE fmt pugixml tidy)

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG 871ed52d350214a034f6ef8a3b8f51c5ce1bd400)
FetchContent_MakeAvailable(cpr)

target_link_libraries(sfa PRIVATE cpr::cpr)
