# CMakeLists.txt:
cmake_minimum_required(VERSION 3.1) 

set(SOURCES
  snake.c main.c dir.c)

if(CMAKE_HOST_WIN32)
  list(APPEND SOURCES util_w32.c)
else(CMAKE_HOST_WIN32)
  list(APPEND SOURCES util_unix.c)
endif(CMAKE_HOST_WIN32)

add_executable(smart_snake
  ${SOURCES})

target_include_directories(smart_snake
  PUBLIC "../include"
  "${CURSES_INCLUDE_DIRS}"
  "${GLIB_INCLUDE_DIRS}")

target_link_libraries(smart_snake
  "${CURSES_LIBRARIES}"
  "${GLIB_LIBRARIES}")
