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
  find_package(Readline REQURED)

  set(EXTRA_TARGETS)
else(CMAKE_HOST_WIN32)
  find_package(Curses REQUIRED)
  set(CMAKE_THREAD_PREFER_PTHREAD ON)
  set(THREADS_PREFER_PTHREAD_FLAG ON)
  find_package(Threads REQUIRED)
  set(EXTRA_LIBS Threads::Threads rt)
endif(CMAKE_HOST_WIN32)

find_package(Glib REQUIRED)

add_subdirectory(src)
