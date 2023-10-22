get_filename_component(targetName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" targetName ${targetName})

add_executable(${targetName} main.cpp)
