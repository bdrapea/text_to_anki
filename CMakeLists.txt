cmake_minimum_required(VERSION 3.0)
project(text_to_anki)

#set the c++ version to c++14
set(CMAKE_CXX_STANDARD 14)

#link boost lib
find_package(Boost REQUIRED)
find_library(sqlite3 REQUIRED)

#set main variables
set(SOURCES "src/main.cpp"
            "src/t2a_engine.cpp")
set(HEADERS "src/t2a_engine.h")

#set dependencies
set(INCLUDE_DIRS
    ${Boost_INCLUDE_DIRS}
    ${SQLite3_INCLUDE_DIRS})
set(LIBS
    ${Boost_LIBS} -lboost_system -lboost_filesystem
    ${SQLite3_LIBS} -lsqlite3)

#Set the final executable
include_directories(${INCLUDE_DIRS})
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} ${LIBS})
