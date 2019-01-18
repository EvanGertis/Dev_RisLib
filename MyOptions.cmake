function(my_set_compile_options _target)
# target_compile_options(${_target} PRIVATE "/W3")
  target_compile_options(${_target} PRIVATE "/WX-")
  target_compile_options(${_target} PRIVATE "/wd4996")

  target_compile_options(${_target} PRIVATE "/O2")
  target_compile_options(${_target} PRIVATE "/Ot")
  target_compile_options(${_target} PRIVATE "/Oi")

  target_compile_options(${_target} PRIVATE "/GS-")
  target_compile_options(${_target} PRIVATE "/Gd")

  target_compile_options(${_target} PRIVATE "/EHsc")
  target_compile_options(${_target} PRIVATE "/MD")
  target_compile_options(${_target} PRIVATE "/Zc:wchar_t")
  target_compile_options(${_target} PRIVATE "/Zc:inline")

  target_compile_options(${_target} PRIVATE "/D_MBCS")
endfunction()
