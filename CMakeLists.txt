cmake_minimum_required(VERSION 3.0)
project(text_to_anki)

#set main variables
set(SRCS "src/main.cpp")

add_executable(${PROJECT_NAME} ${SRCS})
