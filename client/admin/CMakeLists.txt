file(GLOB ADMIN_SOURCES "src/*.cpp")
add_library(admin ${ADMIN_SOURCES})

target_include_directories(admin PUBLIC inc)
target_link_libraries(admin serverConnection util)