cmake_minimum_required(VERSION 3.0)
project(text_to_anki)

#set main variables
set(SOURCES "src/main.cpp")
set(HEADERS "src/t2a_engine.h")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
