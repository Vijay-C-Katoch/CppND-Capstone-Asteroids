cmake_minimum_required(VERSION 3.7)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

SET(ASTEROID ON CACHE BOOL "Set to build Asteroid game")
SET(SNAKE OFF CACHE BOOL "Set to build Snake game")

if(SNAKE)
  project(SnakeGame)
  SET(ASTEROID 0)
  SET(SNAKE 1)
else()
  project(AsteroidsGame)
  SET(ASTEROID 1)
  SET(SNAKE 0)
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_definitions(-std=c++17)
set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

# Default to installing in build directory
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install
    CACHE PATH "Default install path" FORCE)
endif()

find_package(SDL2 REQUIRED)

# Generate version numbers
configure_file (
  user_game_config.h.in
  ${CMAKE_BINARY_DIR}/src/include/userGameConfig.h
  )

include_directories(
  ${SDL2_INCLUDE_DIRS} 
  src
  src/nd_game_engine 
  src/nd_game_engine/math 
  src/nd_game_engine/hw
  ${CMAKE_BINARY_DIR}/src/include
  )

add_executable(
  ${PROJECT_NAME} 
  src/asteroids.cpp 
  src/nd_game_engine/mediaLibrarySDL.cpp 
  src/nd_game_engine/ndGameEngine.cpp 
  )

if(SNAKE)
  target_sources(${PROJECT_NAME} PUBLIC src/snake.cpp)
else()
    target_sources(${PROJECT_NAME} PUBLIC src/asteroids.cpp)
endif ()

string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})

install (
  TARGETS
  ${PROJECT_NAME}
  DESTINATION .
  )

message(STATUS "\n\t  !!! Bulding ${PROJECT_NAME} Game !!!  \t\n")

