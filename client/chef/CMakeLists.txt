add_library(chef src/chef.cpp)

target_include_directories(chef PUBLIC inc)
target_link_libraries(chef serverConnection util)