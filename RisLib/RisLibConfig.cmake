
add_library(RisLib STATIC IMPORTED)
set_target_properties(RisLib PROPERTIES IMPORTED_LOCATION "C:\MyTools\MyLib\lib\RisLib.lib")
target_include_directories(RisLib INTERFACE "C:\MyTools\MyLib\include\RisLib")

target_link_libraries(RisLib INTERFACE ws2_32)
target_link_libraries(RisLib INTERFACE winmm)
