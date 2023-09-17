if (NOT CMAKE_C_COMPILER_ID STREQUAL "Clang")
  message(WARNING "C compiler must be Clang to support testing.")
  return()
endif()

set(p )
if (CMAKE_C_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
  set(p "/clang:")
endif()

add_custom_command(
  OUTPUT hello.ll
  COMMAND
    ${CMAKE_C_COMPILER} ${p}-emit-llvm ${p}-S -Xclang -disable-O0-optnone
      ${CMAKE_CURRENT_LIST_DIR}/hello.c -o hello.ll)
add_custom_target(HelloTest ALL
  DEPENDS hello.ll
  COMMENT "Generating hello.ll for testing")
