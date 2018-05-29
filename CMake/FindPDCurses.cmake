#.rst:
# FindPDCurses
# ----------
#
# Find pdcurses implementation of curses library
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This module defines the following variables:
#
# ``CURSES_FOUND``
#   True if Curses is found.
# ``CURSES_INCLUDE_DIRS``
#   The include directories needed to use Curses.
# ``CURSES_LIBRARIES``
#   The libraries needed to use Curses.
# ``CURSES_HAVE_CURSES_H``
#   True if curses.h is available.
#
# Backward Compatibility
# ^^^^^^^^^^^^^^^^^^^^^^
#
# The following variable are provided for backward compatibility:
#
# ``CURSES_INCLUDE_DIR``
#   Path to Curses include.  Use ``CURSES_INCLUDE_DIRS`` instead.
# ``CURSES_LIBRARY``
#   Path to Curses library.  Use ``CURSES_LIBRARIES`` instead.

set(PDCURSES_PATH_HINT ""
  CACHE PATH
  "Probable location of the pdcurses library")

message("Looking at ${PDCURSES_PATH_HINT}")
find_library(CURSES_CURSES_LIBRARY NAMES pdcurses
  HINTS "${PDCURSES_PATH_HINT}"
  PATH_SUFFIXES lib)

get_filename_component(_cursesLibDir "${CURSES_CURSES_LIBRARY}" PATH)
get_filename_component(_cursesParentDir "${_cursesLibDir}" PATH)

find_path(CURSES_INCLUDE_PATH
  NAMES curses.h
  HINTS "${_cursesParentDir}/include"
  )

# Previous versions of FindCurses provided these values.
if(NOT DEFINED CURSES_CURSES_H_PATH)
  set(CURSES_CURSES_H_PATH "${CURSES_INCLUDE_PATH}")
endif()
if(NOT DEFINED CURSES_LIBRARY)
  set(CURSES_LIBRARY "${CURSES_CURSES_LIBRARY}")
endif()

if(NOT DEFINED CURSES_HAVE_CURSES_H)
  if(EXISTS "${CURSES_INCLUDE_PATH}/curses.h")
    set(CURSES_HAVE_CURSES_H "${CURSES_INCLUDE_PATH}/curses.h")
  else()
    set(CURSES_HAVE_CURSES_H "CURSES_HAVE_CURSES_H-NOTFOUND")
  endif()
endif()

# Provide the *_INCLUDE_DIRS result.
set(CURSES_INCLUDE_DIRS ${CURSES_INCLUDE_PATH})
set(CURSES_INCLUDE_DIR ${CURSES_INCLUDE_PATH}) # compatibility

# Need to provide the *_LIBRARIES
set(CURSES_LIBRARIES ${CURSES_LIBRARY})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PDCurses DEFAULT_MSG
  CURSES_LIBRARY CURSES_INCLUDE_PATH)

mark_as_advanced(
  CURSES_INCLUDE_PATH
  CURSES_CURSES_LIBRARY
  )
