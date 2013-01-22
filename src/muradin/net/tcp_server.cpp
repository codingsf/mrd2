#include <muradin/net/tcp_server.h>
#include <muradin/net/socket.h>
#include <muradin/base/log_warper.h>

#include <errno.h>

#include <boost/bind.hpp>
#include <boost/function.hpp>

namespace muradin{
	namespace net{

		tcp_server::tcp_server(const std::string& local_ip, boost::uint16_t losten_port,io_service& ios,size_t thread_number)
		:m_io_service(ios),
		m_acceptor(endpoint_v4(local_ip,losten_port),ios),
		m_conn_cb(NULL),
		m_read_cb(NULL),
		m_write_cb(NULL),
		m_err_cb(NULL),
		m_service_pool(thread_number)
		{
			m_acceptor.set_accept_cb( boost::bind(&tcp_server::on_new_conn,this) );
		}
		
		tcp_server::~tcp_server()
		{
			//
		}
		void	tcp_server::ready()
		{
			m_service_pool.run();
			m_acceptor.start();
		}
		void	tcp_server::stop()
		{
			m_acceptor.stop();
		}

		void	tcp_server::on_new_conn()
		{
			/// get fd form TCP
			///  do accept,get peer address
			endpoint_v4 addr;
			SOCKET_FD fd = socket::accept(m_acceptor.fd(),addr); 
			if (fd != -1 ){
				io_service* ios_ptr = m_service_pool.next_service();
				connection_ptr new_conn(new connection(*ios_ptr,fd,addr));

				new_conn->set_close_callback(boost::bind(&tcp_server::on_remove_conn,this,_1));
				new_conn->set_read_callback(m_read_cb);
				new_conn->set_write_callback(m_write_cb);
				new_conn->set_conn_callback(m_conn_cb);
				new_conn->set_error_callback(m_err_cb);
				// 
				m_conn_map[fd]=new_conn;
				LOG_INFO.stream()<<"new conn form :"<< addr.get_ip() << " : " << addr.get_port();
				/// notify connection
				new_conn->start();
			}else{
				// got error
				LOG_EROR.stream()<<"epoll_ctl fail,errno:"<< errno;
			}
		}
		void	tcp_server::on_remove_conn(connection_ptr conn)
		{
			m_conn_map.erase(conn->fd());
			conn->final_destory();
			/// socket FD closed in connection's d-tor
		}
	}
}
