add_executable(a.out src/main.cpp src/serverHandler.cpp)

target_include_directories(a.out PUBLIC inc
                            ../database/inc
                            ../recommendationEngine/inc
                            ../requestHandler/inc)
                        
target_link_libraries(a.out PUBLIC database recommendationEngine requestHandler mysqlcppconn)
link_libraries(a.out database recommendationEngine requestHandler mysqlcppconn)
