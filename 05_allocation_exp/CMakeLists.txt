get_filename_component(targetName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" targetName ${targetName})

add_executable(${targetName} main.cpp)


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${targetName} PRIVATE Threads::Threads)