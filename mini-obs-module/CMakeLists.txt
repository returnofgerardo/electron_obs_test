# minimal CMakeLists per https://github.com/cmake-js/cmake-js

cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0091 NEW)
cmake_policy(SET CMP0042 NEW)

project (mini-obs)

# https://nodejs.org/docs/latest/api/n-api.html#node-api-version-matrix
add_definitions(-DNAPI_VERSION=9)

include_directories(${CMAKE_JS_INC})

add_library(${PROJECT_NAME} SHARED ${CMAKE_JS_SRC})
target_sources(${PROJECT_NAME} PRIVATE src/mini-obs.cc)

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})

if(MSVC AND CMAKE_JS_NODELIB_DEF AND CMAKE_JS_NODELIB_TARGET)
  # Generate node.lib
  execute_process(COMMAND ${CMAKE_AR} /def:${CMAKE_JS_NODELIB_DEF} /out:${CMAKE_JS_NODELIB_TARGET} ${CMAKE_STATIC_LINKER_FLAGS})
endif()
