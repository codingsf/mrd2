set(base_SRCS
log_warper.cpp
 )

add_library(muradin_base ${base_SRCS})
target_link_libraries(muradin_base muduo_base)
