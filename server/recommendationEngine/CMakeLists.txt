add_library(recommendationEngine
    src/recommendationEngine.cpp)

target_include_directories(recommendationEngine PUBLIC inc 
                           ../database/inc)

link_libraries(recommendationEngine PUBLIC database)