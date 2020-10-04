#test

include_directories($(CMAKE_PROJECT_INCLUDE))
include_directories(../core/include)
include_directories(../core/template_h)
include_directories(../CLi/include)
include_directories(../engine/include)

link_directories(../build/core)

add_library(test  test.h
                  common/test_common.cpp
                  common/test_launchers.cpp  
                  core/u_simplex.cpp 
                  core/u_cframework.cpp
                 )


