#include <muradin/net/connection.h>

namespace muradin{
namespace net{
	connection::connection(SOCKET_FD fd,const net_address& peer_addr,io_service& ios)
	:m_socket(fd),
	m_service(ios),
	m_channle(fd,ios),
	m_peer_address(peer_addr)
	{
		m_channle->join_to_service();
	}

	void	connection::tcp_enstablished()
	{
		m_channle->subscrib_read_evt();
	}
	net::buffer connection::cached_msg()
	{
		return net::buffer(m_read_cache.rd_ptr(),m_read_cache.readablebytes());
	}
	void	connection::write(const net::buffer& data)
	{
		//
	}
	void	connection::shutdown()
	{
		//shutdown();
	}

	/// set callback (error)
	void	connection::reg_error_cb()
	{
		//
	}
	/// set callback (connection closed)
	void	connection::reg_close_cb()
	{
		//
	}
	/// set callback (msg has read)
	void	connection::reg_msg_cb()
	{
		//
	}

	void		connection::handle_read()
	{
		//m_socket->read();
	}
	/// fd writeable
	void		connection::handle_write()
	{
		// m_socket->write();
	}
	/// handle sys-error
	void		connection::handle_error()
	{
		//
	}
	/// network closed by peer
	void		connection::handle_close()
	{
		//
	}
}
}