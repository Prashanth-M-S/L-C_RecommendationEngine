add_library(auth src/authentication.cpp)

target_include_directories(auth PUBLIC inc ../serverConnection/inc ../utils/inc)
target_link_libraries(util serverConnection)
link_libraries(util serverConnection)