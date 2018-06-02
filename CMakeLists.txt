# CMakeLists.txt:
cmake_minimum_required(VERSION 3.1)

project(smart_snake)
# DESCRIPTION "console snake game powered by Embeddable Common Lisp")

# Add paths to custom CMake scripts.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
  "${CMAKE_SOURCE_DIR}/CMake/"
  )

if(CMAKE_HOST_WIN32)
  find_package(PDCurses REQUIRED)
else(CMAKE_HOST_WIN32)
  find_package(Curses REQUIRED)
endif(CMAKE_HOST_WIN32)

find_package(Glib REQUIRED)

add_subdirectory(src)
