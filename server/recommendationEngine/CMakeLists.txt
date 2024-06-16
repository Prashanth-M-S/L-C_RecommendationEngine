add_library(recomendationEngine SHARED
    src/src/recommendationEngine.cpp)
    
target_include_directories(recomendationEngine PUBLIC src/inc pub ../database/pub)
target_link_libraries(recomendationEngine PUBLIC database)