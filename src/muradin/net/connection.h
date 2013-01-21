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
#include <muradin/net/net_address.h>
#include <muradin/net/user_callback.h>


namespace muradin{
namespace net{
	
	class connection :public boost::enable_shared_from_this<connection>
	{
	public:

		enum conn_stat{
			kDisconnect=0,kConnected=1,kInConnec=2,kInDisconnect=3
		};

		connection(io_service& ios,SOCKET_FD fd,const endpoint_v4& peer_addr);
		~connection();
		bool	connected()const{return m_conn_status == kConnected;};

		void	start();
		void	destory();

		bytebuffer& read_buffer();

		SOCKET_FD fd(){return m_socket.fd();};

		/// 
		/// thread-safe
		void	write(const net::buffer& data);
		/// 
		/// thread-safe
		void	shutdown();

		/// set callback (error)
		void	set_error_cb(const on_error& func){m_err_cb = func;};
		/// set callback (connection open,closed)
		void	set_close_cb(const close_callback& func){m_close_cb=func;};
		/// set callback (msg has read)
		void	set_msg_cb(const on_msg& func){m_msg_cb = func;};
		/// set callback (msg has wrote)
		void	set_msg_complete_cb(const on_msg_complete& func){m_msg_complete_cb = func;};
		/// set callback (open,closed)
		void	set_conn_cb(const on_connect& func){m_conn_cb = func;};
	private:
		/// fd readable
		void		handle_read();
		/// fd writeable
		void		handle_write();
		/// handle sys-error
		void		handle_error();
		/// network closed by peer
		void		handle_close();
		/// wite 
		void		write_in_loop(const void* data,size_t len);
	private:
		socket 		m_socket;
		io_service&	m_service;
		evt_channel	m_channle;
		bytebuffer	m_write_cache;
		bytebuffer	m_read_cache;
		endpoint_v4	m_peer_address;
		/// callback 
		on_connect	m_conn_cb;
		on_msg 		m_msg_cb;
		on_msg_complete m_msg_complete_cb;
		close_callback  m_close_cb;
		on_error	m_err_cb;
		conn_stat		m_conn_status;
	};

	typedef		boost::shared_ptr<connection>	connection_ptr;
}
}

#endif//MURADIN_NET_CONNECTION_H__
