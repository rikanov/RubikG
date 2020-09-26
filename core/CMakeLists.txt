#core
include_directories(include)
include_directories(template_h)

add_library(core    source/simplex.cpp 
                    source/oriented_cube.cpp
                    source/def_colors.cpp
                    source/auxiliary.cpp
            )
