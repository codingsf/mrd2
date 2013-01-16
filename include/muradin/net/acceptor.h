#ifndef MURADIN_NET_ACCEPTOR_H__
#define MURADIN_NET_ACCEPTOR_H__

#include <muradin/net/io_service.h>
#include <muradin/net/net_address.h>
#include <muradin/net/evt_channel.h>
#include <muradin/net/socket.h>
#include <muradin/net/socket_fd.h>

#include <boost/function.hpp>

namespace muradin{
namespace net{
	
class acceptor
{
	typedef boost::function<void()> evt_callback;
public:
	acceptor(const endpoint_v4& local_addr,io_service& ios);
	~acceptor();
	void	start();
	void	stop();
	void	set_accept_cb(const evt_callback& func){m_accept_cb = func;};
	SOCKET_FD fd(){return m_socket.fd();};
protected:
	void	on_read();
private:
	io_service& m_service;
	socket 		m_socket;
	evt_callback m_accept_cb;
	endpoint_v4 m_addr;
	evt_channel m_channel;
};

}
}
#endif//MURADIN_NET_ACCEPTOR_H__