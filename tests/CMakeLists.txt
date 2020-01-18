set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-incompatible-pointer-types -Wno-implicit-function-declaration")
aux_source_directory(. testsrc)
foreach(e ${testsrc})
	get_filename_component(n ${e} NAME_WE)
	add_executable(${n} ${e})
	target_link_libraries(${n} lcthw bsd m)
endforeach(e)

