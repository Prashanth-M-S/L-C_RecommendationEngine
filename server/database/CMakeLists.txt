add_library(database SHARED
    src/src/menuDatabase.cpp
)

target_include_directories(database PUBLIC src/inc)
include_directories(pub)
link_libraries(database PUBLIC mysqlcppconn)
