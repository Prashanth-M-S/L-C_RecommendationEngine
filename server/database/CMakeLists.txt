add_library(database
    src/databaseController.cpp
)

target_include_directories(database PUBLIC inc)
link_libraries(database PUBLIC mysqlcppconn)
