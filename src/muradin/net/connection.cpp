#include <muradin/net/connection.h>
#include <muradin/net/io_service.h>
#include <muradin/base/log_warper.h>

#include <errno.h>

#include <boost/bind.hpp>

namespace muradin{
namespace net{



	connection::connection(io_service& ios,SOCKET_FD fd,const endpoint_v4& peer_addr)
	:m_socket(fd),
	m_service(ios),
	m_channle(fd,ios),
	m_peer_address(peer_addr),
	m_conn_cb(NULL),
	m_msg_cb(NULL),
	m_msg_complete_cb(NULL),
	m_close_cb(NULL),
	m_conn_status(kDisconnect)
	{
		m_channle.set_read_cb(boost::bind(&connection::handle_read,this));
		m_channle.set_write_cb(boost::bind(&connection::handle_write,this));
		m_channle.set_close_cb(boost::bind(&connection::handle_close,this));
		m_channle.set_error_cb(boost::bind(&connection::handle_error,this));

	}
	connection::~connection()
	{
		LOG_INFO.stream()<<"connection >>>>>> d-tor "<<ENDLN;
	}

	void	connection::tcp_enstablished()
	{
		m_service.add_channel(&m_channle);
		m_channle.enable_read(true);
		m_conn_status=kConnected;
		m_conn_cb(shared_from_this());
	}
	net::buffer connection::cached_msg()
	{
		return net::buffer(m_read_cache.rd_ptr(),m_read_cache.readable_bytes());
	}
	void	connection::write(const net::buffer& data)
	{
		//
		// if get EAGAIN call m_channle.enable_write(true);
	}
	void	connection::shutdown()
	{
		m_conn_status=kInDisconnect;
		//shutdown();
	}

	void		connection::handle_read()
	{
		//m_socket->read();
		LOG_INFO.stream()<<"handle_read "<< m_peer_address.get_ip() << " : " << m_peer_address.get_port()<<ENDLN;
		// call m_msg_cb;
	}
	/// fd writeable
	void		connection::handle_write()
	{
		// m_socket->write();
		LOG_INFO.stream()<<"handle_write "<< m_peer_address.get_ip() << " : " << m_peer_address.get_port()<<ENDLN;
		/// write success call m_msg_complete_cb
	}
	/// handle sys-error
	void		connection::handle_error()
	{
		//
		LOG_INFO.stream()<<"handle_error "<< m_peer_address.get_ip() << " : " << m_peer_address.get_port()<<ENDLN;
		/// write success call errcb
	}
	/// network closed by peer
	void		connection::handle_close()
	{
		// close passivity
		LOG_INFO.stream()<<"handle_close "<< m_peer_address.get_ip() << " : " << m_peer_address.get_port()<<ENDLN;
		m_conn_status=kDisconnect;
		m_conn_cb(shared_from_this());
		m_close_cb(shared_from_this()); 
	}
}
}
