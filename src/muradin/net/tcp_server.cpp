#include <muradin/net/tcp_server.h>
#include <muradin/net/socket.h>
#include <muradin/base/log_warper.h>

#include <errno.h>

#include <boost/bind.hpp>
#include <boost/function.hpp>

namespace muradin{
	namespace net{

		tcp_server::tcp_server(const std::string& local_ip, boost::uint16_t losten_port,io_service& ios)
		:m_io_service(ios),
		m_acceptor(endpoint_v4(local_ip,losten_port),ios),
		m_conn_cb(NULL),
		m_msg_cb(NULL),
		m_msg_complete_cb(NULL)
		{
			m_acceptor.set_accept_cb( boost::bind(&tcp_server::on_new_conn,this) );
		}
		
		tcp_server::~tcp_server()
		{
			//
		}
		void	tcp_server::ready()
		{
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
				connection_ptr new_conn(new connection(m_io_service,fd,addr));

				new_conn->set_close_cb(boost::bind(&tcp_server::on_remove_conn,this,_1));
				new_conn->set_msg_cb(m_msg_cb);
				new_conn->set_msg_complete_cb(m_msg_complete_cb);
				new_conn->set_conn_cb(m_conn_cb);
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
			conn->destory();
			/// socket FD closed in connection's d-tor
		}
	}
}
