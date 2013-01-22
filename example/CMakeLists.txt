add_executable(echo_srv echo.cpp)
target_link_libraries(echo_srv muradin_net)

add_executable(echo_srv_mt echo_mt.cpp)
target_link_libraries(echo_srv_mt muradin_net)

add_executable(asio_ppong_c asio/pingpong/client.cpp)
target_link_libraries(asio_ppong_c boost_thread boost_regex boost_serialization boost_system )

add_executable(asio_ppong_s asio/pingpong/server.cpp)
target_link_libraries(asio_ppong_s boost_thread boost_regex boost_serialization boost_system )

