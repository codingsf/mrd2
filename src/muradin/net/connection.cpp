#include <muradin/net/connection.h>
#include <muradin/base/log_waper.h>

#include <errno.h>

namespace muradin{
namespace net{



	connection::connection(io_service& ios,SOCKET_FD fd,const net_address& peer_addr)
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
		LOG_INFO.stream()<<"connection >>>>>> d-tor "<<EOL();
	}

	void	connection::tcp_enstablished()
	{
		m_service.add_channel(&m_channle);
		m_channle.enable_read(true);
		m_conn_status=kConnected;
		m_conn_cb();
	}
	net::buffer connection::cached_msg()
	{
		return net::buffer(m_read_cache.rd_ptr(),m_read_cache.readablebytes());
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
		LOG_INFO.stream()<<"handle_read "<< m_peer_address.get_ip() << " : " << m_peer_address.get_port()<<EOL();
		// call m_msg_cb;
	}
	/// fd writeable
	void		connection::handle_write()
	{
		// m_socket->write();
		LOG_INFO.stream()<<"handle_write "<< m_peer_address.get_ip() << " : " << m_peer_address.get_port()<<EOL();
		/// write success call m_msg_complete_cb
	}
	/// handle sys-error
	void		connection::handle_error()
	{
		//
		LOG_INFO.stream()<<"handle_error "<< m_peer_address.get_ip() << " : " << m_peer_address.get_port()<<EOL();
		/// write success call errcb
	}
	/// network closed by peer
	void		connection::handle_close()
	{
		// close passivity
		LOG_INFO.stream()<<"handle_close "<< m_peer_address.get_ip() << " : " << m_peer_address.get_port()<<EOL();
		m_conn_status=kDisconnect;
		m_conn_cb();
		m_close_cb(); 
	}
}
}