#ifndef MURADIN_NET_CONNECTION_H__
#define MURADIN_NET_CONNECTION_H__

#include <boost/function.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <muradin/net/buffer.h>
#include <muradin/net/bytebuffer.h>
#include <muradin/net/socket_fd.h>
#include <muradin/net/socket.h>
#include <muradin/net/evt_channel.h>
#include <muradin/net/user_callback.h>

namespace muradin{
namespace net{
	
	class connection : boost::enable_shared_from_this
	{
		typedef boost::function<void()>    ;
	public:
		connection(SOCKET_FD fd,const net_address& peer_addr,io_service& ios);

		void	tcp_enstablished();
		net::buffer cached_msg();
		void	write(const net::buffer& data);
		void	shutdown();

		/// set callback (error)
		void	reg_error_cb();
		/// set callback (connection closed)
		void	reg_close_cb();
		/// set callback (msg has read)
		void	reg_msg_cb();
	private:
		/// fd readable
		void		handle_read();
		/// fd writeable
		void		handle_write();
		/// handle sys-error
		void		handle_error();
		/// network closed by peer
		void		handle_close();
	private:
		socket 		m_socket;
		io_service&	m_service;
		evt_channel	m_channle;
		bytebuffer	m_write_cache;
		bytebuffer	m_read_cache;
		net_address	m_peer_address;
	};

	typedef		boost::shared_ptr<connection>	connection_ptr;
}
}

#endif//MURADIN_NET_CONNECTION_H__