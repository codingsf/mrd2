add_executable(echo_srv echo.cpp)
target_link_libraries(echo_srv muradin_net)

add_executable(echo_srv_mt echo_mt.cpp)
target_link_libraries(echo_srv_mt muradin_net)

