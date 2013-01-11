#ifndef	MURADIN_NET_TCPSERVER_H__
#define	MURADIN_NET_TCPSERVER_H__

#include <muradin/net/net_address.h>
#include <muradin/net/io_service.h>
#include <muradin/net/cooection.h>
#include <muradin/net/socket_fd.h>

#include <string>
#include <map>


namespace muradin{
	namespace net{
		class tcp_server
		{
		public:
			tcp_server(const std::string& local_ip, boost::uint16_t losten_port,io_service& ios);
			~tcp_server();
			void	run();
		private:
			void	on_new_conn();
			void	on_remove_conn(SOCKET_FD fd);
		private:
			io_service&	m_io_service;
			acceptor	m_acceptor;
			std::map<connection_ptr>	m_conn_map;
		};
	}
}
#endif//MURADIN_NET_TCPSERVER_H__