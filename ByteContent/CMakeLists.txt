﻿cmake_minimum_required (VERSION 3.8)

project (ByteContent CXX)

my_find_src_files (src_files ByteContent)
add_executable (ByteContent ${src_files})
my_add_compile_options (ByteContent)
my_add_pch (ByteContent)

my_inc_import_RisLib(ByteContent)
my_lib_import_RisLib(ByteContent)

