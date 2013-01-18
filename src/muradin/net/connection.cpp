#include <muradin/net/connection.h>
#include <muradin/net/io_service.h>
#include <muradin/base/log_warper.h>

#include <errno.h>
#include <assert.h>

#include <boost/bind.hpp>

namespace muradin{
namespace net{



	connection::connection(io_service& ios,SOCKET_FD fd,const endpoint_v4& peer_addr)
	:m_socket(fd),
	m_service(ios),
	m_channle(fd,ios,"connection-chnl"),
	m_peer_address(peer_addr),
	m_conn_cb(NULL),
	m_msg_cb(NULL),
	m_msg_complete_cb(NULL),
	m_close_cb(NULL),
	m_conn_status(kDisconnect)
	{
		LOG_INFO.stream()<<"connection >>>>>>>>>>>>>>>>>> c-tor ";
		m_channle.set_read_cb(boost::bind(&connection::handle_read,this));
		m_channle.set_write_cb(boost::bind(&connection::handle_write,this));
		m_channle.set_close_cb(boost::bind(&connection::handle_close,this));
		m_channle.set_error_cb(boost::bind(&connection::handle_error,this));

	}
	connection::~connection()
	{
		LOG_INFO.stream()<<"connection >>>>>>>>>>>>>>>>>> d-tor ";
	}

	void	connection::start()
	{
		// murst call from owner thread
		assert(m_service.check_this_loop());

		m_channle.join_to_service();
		m_channle.enable_read(true);
		m_conn_status=kConnected;
		m_conn_cb(shared_from_this());
	}
	void	connection::destory()
	{
		assert(m_conn_status == kDisconnect);
		m_channle.enable_read(false);
		m_channle.enable_write(false);
		m_channle.remove_from_service();
	}

	bytebuffer& connection::read_buffer()
	{
		return m_read_cache;
	}
	void	connection::write(const net::buffer& data)
	{
		if(m_service.check_this_loop()){
			write_in_loop(data.data(),data.size());
		}else{
			m_service.run_task(  boost::bind(&connection::write_in_loop,this,data.data(),data.size()) );
		}
	}
	void	connection::write_in_loop(const void* data,size_t len)
	{
		//
		assert(m_service.check_this_loop());
		//bool error=false;
		int ret=::write(m_socket.fd(),data,len);
		if (ret < 0){
			if(errno != EAGAIN){
				LOG_EROR.stream()<<"write fail errno = " << errno ;
			}else{
				m_write_cache.append(data,len);
				m_channle.enable_write(true);
			}
		}else{
			if(ret < len){
				m_write_cache.append(data,len-ret);
				m_channle.enable_write(true);
			}
			m_msg_complete_cb(shared_from_this(),ret);
		}
	}
	void	connection::shutdown()
	{
		m_conn_status=kInDisconnect;
		//shutdown();
	}

	void		connection::handle_read()
	{
		//m_socket->read();
		LOG_INFO.stream()<<"handle_read "<< m_peer_address.get_ip() << " : " << m_peer_address.get_port();
		static int max_readbytes_once=1024*8;
		m_read_cache.ensure_write_space(static_cast<size_t>(max_readbytes_once));
		int ret=::read(m_socket.fd(),m_read_cache.wt_ptr(),max_readbytes_once);
		if(ret == 0){
			// peer shutdown
			m_conn_cb(shared_from_this());
			handle_close();
		}else if(ret > 0 ){
			m_read_cache.has_written(static_cast<size_t>(ret));
			m_msg_cb(shared_from_this());
		}else{
			LOG_EROR.stream()<<"read fail errno = " << errno ;
		}
		// call m_msg_cb;
	}
	/// fd writeable
	void		connection::handle_write()
	{
		// m_socket->write();
		LOG_INFO.stream()<<"handle_write "<< m_peer_address.get_ip() << " : " << m_peer_address.get_port();
		/// write success call m_msg_complete_cb
		int ret=::write(m_socket.fd(),m_write_cache.rd_ptr(),(int)m_write_cache.readable_bytes());
		if(ret > 0 ){
			m_write_cache.discard(ret);
			if(m_write_cache.readable_bytes() <= 0)
				m_channle.enable_write(false);

			m_msg_complete_cb(shared_from_this(),ret);
		}else{
			LOG_EROR.stream()<<"write fail errno = " << errno ;
		}
	}
	/// handle sys-error
	void		connection::handle_error()
	{
		//
		LOG_INFO.stream()<<"handle_error "<< m_peer_address.get_ip() << " : " << m_peer_address.get_port() << " errno = " << socket::retrieve_err(m_socket.fd());
		///  call errcb
	}
	/// network closed by peer
	void		connection::handle_close()
	{
		// close passivity
		LOG_INFO.stream()<<"handle_close "<< m_peer_address.get_ip() << " : " << m_peer_address.get_port();
		m_conn_status=kDisconnect;


		m_conn_cb(shared_from_this());
		m_close_cb(shared_from_this()); 
	}
}
}
