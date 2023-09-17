cmake_minimum_required(VERSION 3.23)

project(vs-windows-llvm)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(debugEnable ON)
else()
  set(debugEnable OFF)
endif()

option(ENABLE_TESTS "Enable tests" ${debugEnable})
option(BUILD_UTILS "Build utilities" ${debugEnable})

# Find LLVM
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}: ${LLVM_INSTALL_PREFIX}")
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")

include(AddLLVM)

# Plugins have to be explicitly enabled for Windows.
set(LLVM_ENABLE_PLUGINS ON)

# Disable RTTI if LLVM was build without it.
if (NOT LLVM_ENABLE_RTTI)
  if (MSVC)
    string(APPEND CMAKE_CXX_FLAGS " /GR-")
  else()
    string(APPEND CMAKE_CXX_FLAGS " -fno-rtti")
  endif()
endif()

include_directories(${LLVM_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/include)
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

if (APPLE)
  link_libraries("$<$<PLATFORM_ID:Darwin>:-undefined deynamic_lookup>")
endif()

add_subdirectory(lib)
if (ENABLE_TESTS)
  add_subdirectory(test)
endif()

if (BUILD_UTILS)
  #add_subdirectory(util)
endif()
