cmake_minimum_required(VERSION 3.0)
project(text_to_anki)

#set the c++ version to c++14
set(CMAKE_CXX_STANDARD 14)

#link boost lib
find_package(Boost REQUIRED)
find_library(sqlite3 REQUIRED)
find_library(Poppler REQUIRED)

#set main variables
set(SOURCES "src/main.cpp"
            "src/t2a_engine.cpp"
            "src/t2a_utils.cpp"
            "src/t2a_anki.cpp")
set(HEADERS "src/t2a_engine.h"
            "src/t2a_utils.h"
            "src/t2a_anki.h"
            "src/t2a_constants.h")

#set dependencies
set(INCLUDE_DIRS
    ${Boost_INCLUDE_DIRS}
    ${SQLite3_INCLUDE_DIRS}
    ${Poppler_INCLUDE_DIRS})
set(LIBS
    ${Boost_LIBS} -lboost_system -lboost_filesystem -lboost_program_options
    ${SQLite3_LIBS} -lsqlite3
    ${Poppler_LIBS} -lpoppler-cpp)

#Set the final executable
include_directories(${INCLUDE_DIRS})
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} ${LIBS})
