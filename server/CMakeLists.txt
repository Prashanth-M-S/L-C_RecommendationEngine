Cmake_minimum_required(VERSION 3.22.1)

project("RecommendationEngine")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(utils/inc)

add_subdirectory(database)
add_subdirectory(recommendationEngine)
add_subdirectory(serverHandler)
add_subdirectory(requestHandler)
add_subdirectory(utils)