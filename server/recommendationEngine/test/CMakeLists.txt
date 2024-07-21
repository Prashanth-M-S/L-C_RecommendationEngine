project(recommendationEngineTest)
cmake_minimum_required(VERSION 3.22)

add_subdirectory(../../database ../../database/build)

add_library(recommendationEngine
    ../src/recommendationEngine.cpp)

target_include_directories(recommendationEngine PUBLIC 
    inc 
    ../../database/inc 
    ../../database/test 
    ../inc/ 
    ../../utils/inc/)

target_link_libraries(recommendationEngine PUBLIC database)

add_executable(recommendationEngineTest 
    ./recommendationEngineGtest.cpp)

target_include_directories(recommendationEngineTest PUBLIC 
    inc 
    ../inc)

target_link_libraries(recommendationEngineTest 
    PUBLIC recommendationEngine 
    database 
    gtest 
    gmock)
