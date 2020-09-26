#test
include_directories(include)
include_directories(../core/include)
include_directories(../core/template_h)
include_directories(../CLi/include)
include_directories(../engine/include)

link_directories(../build/core)

add_library(test source/test.cpp)

#target_link_libraries(test core)

