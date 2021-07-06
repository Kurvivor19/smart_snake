# building of lisp library

add_library(embedded-console STATIC IMPORTED GLOBAL)
set_target_properties(embedded-console
  PROPERTIES IMPORTED_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/embedded-console--all-systems${CMAKE_STATIC_LIBRARY_SUFFIX}")
target_link_directories(embedded-console
  INTERFACE "/usr/local/lib/ecl-21.2.1")
target_link_libraries(embedded-console
  INTERFACE asdf ecl-quicklisp)


add_custom_target(build_lisp
  DEPENDS embedded-console--all-systems${CMAKE_STATIC_LIBRARY_SUFFIX})

add_custom_command(OUTPUT embedded-console--all-systems${CMAKE_STATIC_LIBRARY_SUFFIX}
  MAIN_DEPENDENCY embedded-console.lisp
  DEPENDS embedded-console.asd
  COMMAND ecl -norc -eval '(require :ecl-quicklisp)'
                    -eval '(push \"${CMAKE_CURRENT_LIST_DIR}/\" asdf:*central-registry*)'
                    -eval '(asdf:make-build :embedded-console :type :static-library :move-here \#P\"${CMAKE_CURRENT_BINARY_DIR}\" :monolithic t :init-name \"init_embedded_console\")'
                    -eval '(quit)')
