add_library(tgc tgc/tgc.c)
target_compile_options(tgc PRIVATE -Wno-use-after-free)

add_library(linenoise linenoise/linenoise.c)
