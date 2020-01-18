set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-incompatible-pointer-types -Wno-implicit-function-declaration")
aux_source_directory(. libsrc)
add_library(lcthw ${libsrc})

