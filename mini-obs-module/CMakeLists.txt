# minimal CMakeLists per https://github.com/cmake-js/cmake-js

cmake_minimum_required(VERSION 3.28)
cmake_policy(SET CMP0091 NEW)
cmake_policy(SET CMP0042 NEW)

project (mini-obs)

# https://nodejs.org/docs/latest/api/n-api.html#node-api-version-matrix
add_definitions(-DNAPI_VERSION=9)

include_directories(${CMAKE_JS_INC})

add_library(${PROJECT_NAME} SHARED ${CMAKE_JS_SRC})
target_sources(${PROJECT_NAME} PRIVATE src/mini-obs.cc)

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_JS_LIB})


# using add_subdirectory with obs-studio doesn't work out of the box, so link the stuff we need manually for now
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    SET(OBS_BUILD_PATH "${CMAKE_SOURCE_DIR}/libs/obs-studio/build_x64")
    find_library(LIB_OBS obs PATHS ${OBS_BUILD_PATH}/libobs/${CMAKE_BUILD_TYPE})

    # we will copy these to binary dir later.
    file(GLOB_RECURSE DLL_LIST ${OBS_BUILD_PATH}/*.dll)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    # not tested
    SET(OBS_BUILD_PATH "${CMAKE_SOURCE_DIR}/libs/obs-studio/build_macos")
    find_library(LIB_OBS obs PATHS ${OBS_BUILD_PATH}/libobs/${CMAKE_BUILD_TYPE})
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE ${LIB_OBS})
target_include_directories(${PROJECT_NAME} PRIVATE libs/obs-studio/libobs)

if(MSVC AND CMAKE_JS_NODELIB_DEF AND CMAKE_JS_NODELIB_TARGET)
  # Generate node.lib
  execute_process(COMMAND ${CMAKE_AR} /def:${CMAKE_JS_NODELIB_DEF} /out:${CMAKE_JS_NODELIB_TARGET} ${CMAKE_STATIC_LINKER_FLAGS})
endif()

#https://cmake.org/cmake/help/latest/manual/cmake.1.html#cmdoption-cmake-E
if(DEFINED DLL_LIST)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DLL_LIST} ${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}
    )
endif()


