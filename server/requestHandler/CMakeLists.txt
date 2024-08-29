add_library(requestHandler src/requestHandler.cpp)

target_include_directories(requestHandler PUBLIC inc
                            ../database/inc
                            ../recommendationEngine/inc
                            ../utils/inc)

target_link_libraries(requestHandler PUBLIC database recommendationEngine mysqlcppconn utils)
link_libraries(requestHandler database recommendationEngine mysqlcppconn utils)