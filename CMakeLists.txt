cmake_policy(SET CMP0048 NEW)
project(BattleShip)
cmake_minimum_required(VERSION 3.5)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

find_package(Boost COMPONENTS system)
INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})
LINK_LIBRARIES(${Boost_LIBRARIES})

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic ") # -Wconversion
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -fcoroutines")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g0 -DNDEBUG -O3 -pthread -march=native")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined,address")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -fno-omit-frame-pointer -O0 -pthread -march=x86-64")

ADD_EXECUTABLE(BattleShip Battle_Ships.cpp Input_Coordinates.cpp startClient.cpp Game.cpp)
#ADD_CUSTOM_TARGET(notify ALL DEPENDS .notified sotest)

SET(TIDYCHECKS "-*")
SET(TIDYCHECKS "${TIDYCHECKS},readability-*")
SET(TIDYCHECKS "${TIDYCHECKS},modernize-*,-modernize-use-trailing-return-type")
SET(TIDYCHECKS "${TIDYCHECKS},hicpp-*")
SET(TIDYCHECKS "${TIDYCHECKS},bugprone-*")
SET(TIDYCHECKS "${TIDYCHECKS},cppcoreguidelines-*")
SET(TIDYCHECKS "${TIDYCHECKS},google-*")
SET(TIDYCHECKS "${TIDYCHECKS},performance-*,")

ADD_CUSTOM_COMMAND(OUTPUT .stamp
    COMMAND run-clang-tidy-11
    -fix
    -checks='-*,readability-*,modernize-*,-modernize-use-trailing-return-type'
    test.cpp
)

ADD_CUSTOM_TARGET(tidy DEPENDS .stamp)

