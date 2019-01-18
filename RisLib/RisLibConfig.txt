cmake_minimum_required (VERSION 3.8)

project (MyLibrary1 CXX)
file(GLOB SRC_FILES RELATIVE ${PROJECT_SOURCE_DIR} *.cpp)
file(GLOB INC_FILES RELATIVE ${PROJECT_SOURCE_DIR} *.h)
add_library (MyLibrary1 STATIC ${SRC_FILES})
my_set_compile_options(MyLibrary1)
my_add_precompiled_header(MyLibrary1 stdafx.h)

target_include_directories(MyLibrary1 PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(MyLibrary1 ws2_32)

# debug messages
get_target_property(PROP_SOURCE MyLibrary1 SOURCES)
message(STATUS "MyLibrary1 SourceDir*********************" ${PROJECT_SOURCE_DIR})
message(STATUS "MyLibrary1 Source************************" ${SRC_FILES})
message(STATUS "MyLibrary1 Source************************" ${PROP_SOURCE})





