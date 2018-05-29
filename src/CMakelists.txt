# CMakeLists.txt:
cmake_minimum_required(VERSION 3.1) 

add_executable(smart_snake
  snake.c main.c dir.c)

target_include_directories(smart_snake
  PUBLIC "../include"
  "${CURSES_INCLUDE_DIRS}"
  "${GLIB_INCLUDE_DIRS}")

target_link_libraries(smart_snake
  "${CURSES_LIBRARIES}"
  "${GLIB_LIBRARIES}")
