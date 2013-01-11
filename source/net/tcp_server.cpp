#include <muradin/net/tcp_server.h>

#include <boost/bind.hpp>
#include <boost/function.hpp>

namespace muradin{
	namespace net{
	
		tcp_server::tcp_server(const std::string& local_ip, boost::uint16_t losten_port,io_service& ios)
		:m_io_service(ios),m_acceptor(ios)
		{
			m_acceptor->reg_conn_cb(&tcp_server::on_new_conn,this);
		}
		
		tcp_server::~tcp_server()
		{
			//
		}
		void	tcp_server::run()
		{
			//
		}
		void	tcp_server::on_new_conn()
		{
			/// get fd form TCP
			///  do accept,get peer address
			SOCKET_FD fd; 
			net_address addr;

			connection_ptr new_conn(new connection(fd,addr));
			new_conn->reg_close_cb(boost::bind(&tcp_server::on_remove_conn,this));

			/// notify connection
			new_conn->tcp_enstablished();
		}
		void	tcp_server::on_remove_conn(SOCKET_FD fd)
		{
			m_conn_map.earse(fd);
			/// socket FD closed in connection's d-tor
		}
	}
}
