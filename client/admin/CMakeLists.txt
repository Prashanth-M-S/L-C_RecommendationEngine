add_library(admin src/admin.cpp)

target_include_directories(admin PUBLIC inc)
target_link_libraries(admin serverConnection util)