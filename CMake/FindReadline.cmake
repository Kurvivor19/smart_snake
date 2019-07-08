# based on https://github.com/yandex/ClickHouse/blob/master/cmake/find_readline_edit.cmake                                                  

include (CMakePushCheckState)
cmake_push_check_state ()

set (READLINE_PATHS "/usr/lib")
# First try find custom lib for macos users (default lib without history support)
find_library (READLINE_LIBRARY NAMES readline PATHS ${READLINE_PATHS} NO_DEFAULT_PATH)
if (NOT READLINE_LIBRARY)
    find_library (READLINE_LIBRARY NAMES readline PATHS ${READLINE_PATHS})
endif ()

set(READLINE_INCLUDE_PATHS "/usr/include")
if (READLINE_LIBRARY)
    find_path (READLINE_INCLUDE_DIR NAMES readline/readline.h PATHS ${READLINE_INCLUDE_PATHS} NO_DEFAULT_PATH)
    if (NOT READLINE_INCLUDE_DIR)
        find_path (READLINE_INCLUDE_DIR NAMES readline/readline.h PATHS ${READLINE_INCLUDE_PATHS})
    endif ()
endif ()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Readline DEFAULT_MSG
  READLINE_LIBRARY READLINE_INCLUDE_DIR)

cmake_pop_check_state ()
