cmake_minimum_required (VERSION 3.8)

project (RisLib CXX)
file(GLOB SRC_FILES RELATIVE ${PROJECT_SOURCE_DIR} *.cpp)
file(GLOB INC_FILES RELATIVE ${PROJECT_SOURCE_DIR} *.h)
add_library (RisLib STATIC ${SRC_FILES})
my_set_compile_options(RisLib)
my_add_precompiled_header(RisLib stdafx.h)

target_include_directories(RisLib PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(RisLib ws2_32)

# debug messages
get_target_property(PROP_SOURCE RisLib SOURCES)
message(STATUS "RisLib SourceDir*********************" ${PROJECT_SOURCE_DIR})
message(STATUS "RisLib Source************************" ${SRC_FILES})
message(STATUS "RisLib Source************************" ${PROP_SOURCE})





