#add_llvm_pass_plugin(${PROJECT_NAME}
#  plugin-registration.cpp
#  string-analysis.cpp
#  string-reverser.cpp
#  
#  LINK_COMPONENTS
#    Core
#    Passes
#    Support
#  )

add_library(${PROJECT_NAME} SHARED
  plugin-registration.cpp
  string-analysis.cpp
  string-reverser.cpp
  util.cpp)

llvm_map_components_to_libnames(llvmLibs Core Passes Support)
target_link_libraries(${PROJECT_NAME} PUBLIC ${llvmLibs})

set_target_properties(${PROJECT_NAME}
  PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED TRUE)

install(TARGETS ${PROJECT_NAME})

if (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
  set_target_properties(${PROJECT_NAME}
    PROPERTIES
      ENABLE_EXPORTS TRUE
      WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
else()
  target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}.def)
endif()
