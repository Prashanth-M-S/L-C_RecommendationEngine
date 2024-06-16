Cmake_minimum_required(VERSION 3.22.1)

project("RecommendationEngine")

include_directories(utils)
add_subdirectory(recommendationEngine)
add_subdirectory(server)
add_subdirectory(database)