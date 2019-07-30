#  Try to find Embeddable-Common Lisp
#  
#  ECL_FOUND - System has ECL
#  ECL_INCLUDE_DIRS - The ECL include directories
#  ECL_LIBRARIES - The libraries needed to use ECL
#  ECL_DEFINITIONS - Compiler switches required for using ECL

find_program(ECL_CONFIG_VAR ecl-config)

execute_process(COMMAND ${ECL_CONFIG_VAR} --cflags
  OUTPUT_VARIABLE ECL_DEFINITIONS)
execute_process(COMMAND ${ECL_CONFIG_VAR} --libs
  OUTPUT_VARIABLE ECL_LIBRARIES)


#find_path(ECL_INCLUDE_DIR ecl/ecl.h
#          PATH_SUFFIXES libecl)

#find_library(ECL_LIBRARY NAMES ecl)

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set ECL_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(ECL DEFAULT_MSG
                                  ECL_LIBRARIES ECL_DEFINITIONS)

# mark_as_advanced(ECL_INCLUDE_DIR ECL_LIBRARY )

# set(ECL_LIBRARIES ${ECL_LIBRARY} )
# set(ECL_INCLUDE_DIRS ${ECL_INCLUDE_DIR} )
