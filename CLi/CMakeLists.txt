#CLi
include_directories(include)
include_directories(../core/include)
include_directories(../core/template_h)

link_directories(../build/core)

add_library(cli source/cli.cpp )

target_link_libraries(cli core)
