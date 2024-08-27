file(GLOB EMPLOYEE_SOURCES "src/*.cpp")

add_library(employee ${EMPLOYEE_SOURCES})

target_include_directories(employee PUBLIC inc)
target_link_libraries(employee serverConnection util)