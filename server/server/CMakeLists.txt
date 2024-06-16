add_executable(server src/main.cpp src/server.cpp)

target_include_directories(server PUBLIC inc ../recommendationEngine/pub ../recommendationEngine/src/inc)

link_libraries(server database recomendationEngine)