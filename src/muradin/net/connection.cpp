#include <muradin/net/connection.h>
#include <muradin/net/io_service.h>
#include <muradin/base/log_warper.h>
#include <muradin/net/socket_ctl.h>

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
	m_read_cb(NULL),
	m_write_cb(NULL),
	m_close_cb(NULL),
	m_err_cb(NULL),
	m_conn_status(kDisconnect)
	{
		//LOG_INFO.stream()<<"connection >>>>>>>>>>>>>>>>>> c-tor ";
		socket_ctl::set_tcp_nodelay(m_socket.fd(),true);
		socket_ctl::set_keepalive(m_socket.fd());
		m_channle.set_read_callback(boost::bind(&connection::handle_read,this));
		m_channle.set_write_callback(boost::bind(&connection::handle_write,this));
		m_channle.set_close_callback(boost::bind(&connection::handle_close,this));
		m_channle.set_error_callback(boost::bind(&connection::handle_error,this));

	}
	connection::~connection()
	{
		//LOG_INFO.stream()<<"connection >>>>>>>>>>>>>>>>>> d-tor ";
	}

	void	connection::start()
	{
		m_channle.tie(shared_from_this());
		m_channle.join_to_service();
		m_channle.enable_read(true);
		m_conn_status=kConnected;
		m_service.run_task(  boost::bind(m_conn_cb,shared_from_this() ) );
	}
	void	connection::final_destory()
	{
		assert(m_conn_status == kDisconnect);
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
	void		connection::shutdown_in_loop()
	{
		socket::shutdown(m_socket.fd(),socket::k_tcp_close_writ);
	}
	void	connection::write_in_loop(const void* data,size_t len)
	{
		//
		assert(m_service.check_this_loop());
		int ret=m_socket.send(data,len);
		if (ret < 0){
			if(errno == EAGAIN){
				m_write_cache.append(data,len);
				m_channle.enable_write(true);
			}else {
				/*
				if(errno == EPIPE){
					LOG_EROR.stream()<<"write fail ---- EPIPE ----  ";
				}else{
					LOG_EROR.stream()<<"write fail errno = " << errno ;
					//handle_error();
					muradin::base::sys_error err(errno);
					m_service.run_task( boost::bind( m_write_cb,shared_from_this(),err,0 ));
				}
				*/
				LOG_EROR.stream()<<"write fail errno = " << errno ;
				//handle_error();
				muradin::base::sys_error err(errno);
				m_service.run_task( boost::bind( m_write_cb,shared_from_this(),err,0 ));
			}
		}else{
			if(ret < len){
				m_write_cache.append(data,len-ret);
				m_channle.enable_write(true);
			}
			if(ret>0){
				m_service.run_task( 
					boost::bind( m_write_cb,shared_from_this(),muradin::base::sys_error::no_error(),ret )
					);
			}
		}
	}
	void	connection::shutdown()
	{
		m_conn_status=kInDisconnect;
	
		if(m_service.check_this_loop()){
			shutdown_in_loop();
		}else{
			m_service.run_task(  boost::bind(&connection::shutdown_in_loop ,this) );
		}
	}

	void		connection::handle_read()
	{
		//LOG_INFO.stream()<<"handle_read "<< m_peer_address.get_ip() << " : " << m_peer_address.get_port();
		static size_t max_readbytes_once=1024*32;
		m_read_cache.ensure_write_space( max_readbytes_once);
		int ret=m_socket.recv(m_read_cache.wt_ptr(),max_readbytes_once);
		if(ret == 0){
			// peer shutdown
			m_service.run_task( boost::bind( m_conn_cb,shared_from_this()));
			m_service.run_task( boost::bind( &connection::handle_close,this));

		}else if(ret > 0 ){
			m_read_cache.has_written(static_cast<size_t>(ret));
			m_service.run_task( boost::bind( m_read_cb,shared_from_this(),muradin::base::sys_error::no_error()) );

		}else{
			//handle_error();
			muradin::base::sys_error err(errno);
			LOG_EROR.stream()<<"read fail errno = " << errno ;
			m_service.run_task( boost::bind( m_read_cb,shared_from_this(),err ));
		}
		
	}
	/// fd writeable
	void		connection::handle_write()
	{
		LOG_INFO.stream()<<"handle_write "<< m_peer_address.get_ip() << " : " << m_peer_address.get_port();

		int ret=m_socket.send(m_write_cache.rd_ptr(),m_write_cache.readable_bytes());
		
		if (ret < 0){
			if(errno == EAGAIN){
				m_channle.enable_write(true);
			}else{
				LOG_EROR.stream()<<"write fail errno = " << errno ;
				muradin::base::sys_error err(errno);
				m_service.run_task( boost::bind( m_write_cb,shared_from_this(),err,0 ));
			}
		}else{
			m_write_cache.discard(static_cast<size_t>(ret));
			if(m_write_cache.readable_bytes() > 0){
				m_channle.enable_write(true);
			}
			if(ret>0){
				m_write_cache.discard(ret);
				m_service.run_task( 
					boost::bind( m_write_cb,shared_from_this(),muradin::base::sys_error::no_error(),ret )
					);
			}
		}
	}
	/// handle sys-error
	void		connection::handle_error()
	{
		muradin::base::sys_error err( socket::retrieve_err(m_socket.fd()) );
		LOG_INFO.stream()<<"handle_error "<< m_peer_address.get_ip() << " : " << m_peer_address.get_port() 
			<< " err msg = " << err.what(); 
		if(m_err_cb)
			m_service.run_task( boost::bind( m_err_cb,shared_from_this(),err ) );

	}
	/// network closed by peer
	void		connection::handle_close()
	{
		// close passivity
		LOG_INFO.stream()<<"handle_close "<< m_peer_address.get_ip() << " : " << m_peer_address.get_port();
		m_conn_status=kDisconnect;
		m_channle.enable_read(false);
		m_channle.enable_write(false);
		
		//connection_ptr conn_ptr(shared_from_this());
		//m_service.run_task(  boost::bind(m_conn_cb, conn_ptr) );
		//m_close_cb(conn_ptr);

		m_service.run_task(  boost::bind(m_conn_cb,shared_from_this()) );
		m_service.run_task(  boost::bind(m_close_cb,shared_from_this()) );

		//m_conn_cb(shared_from_this());
		//m_close_cb(shared_from_this()); 
	}
}
}
