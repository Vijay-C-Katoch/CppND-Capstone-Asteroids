cmake_minimum_required(VERSION 3.7)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

project(AsteroidsGame)

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

include_directories(${SDL2_INCLUDE_DIRS} src src/math)

add_executable(
  ${PROJECT_NAME} 
  src/main.cpp 
  src/mediaLibrarySDL.cpp 
  src/ndGameEngine.cpp 
  )

string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})

install (
  TARGETS
  ${PROJECT_NAME}
  DESTINATION .
  )

