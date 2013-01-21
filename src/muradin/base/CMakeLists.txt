set(base_SRCS
log_warper.cpp
timestamp.cpp
blocking_counter.cpp
 )

add_library(muradin_base ${base_SRCS})
target_link_libraries(muradin_base boost_thread pthread rt)


