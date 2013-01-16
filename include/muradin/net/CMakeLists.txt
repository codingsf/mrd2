set(net_SRCS
acceptor.cpp
connection.cpp
endian.cpp
ep_poller.cpp
evt_channel.cpp
io_service.cpp
net_address.cpp
socket.cpp
socket_ctl.cpp
tcp_server.cpp
 )

add_library(muradin_net ${net_SRCS})
target_link_libraries(muradin_net muradin_base)
