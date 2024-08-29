add_library(employee src/employee.cpp)

target_include_directories(employee PUBLIC inc)
target_link_libraries(employee serverConnection util)