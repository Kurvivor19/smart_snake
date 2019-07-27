# building of lisp library

add_library(embedded-console STATIC IMPORTED GLOBAL)
set_target_properties(embedded-console
  PROPERTIES IMPORTED_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/embedded-console${CMAKE_STATIC_LIBRARY_SUFFIX}")

add_custom_target(build_lisp
  DEPENDS embedded-console${CMAKE_STATIC_LIBRARY_SUFFIX})

add_custom_command(OUTPUT embedded-console${CMAKE_STATIC_LIBRARY_SUFFIX}
  MAIN_DEPENDENCY embedded-console.lisp
  DEPENDS embedded-console.asd
  COMMAND ecl -norc -eval '(require :asdf)'
                    -eval '(push \"${CMAKE_CURRENT_LIST_DIR}/\" asdf:*central-registry*)'
                    -eval '(asdf:make-build :embedded-console :type :static-library  :move-here \#P\"${CMAKE_CURRENT_BINARY_DIR}\")'
                    -eval '(quit)')
