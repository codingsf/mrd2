#ifndef	MURADIN_NET_TCPSERVER_H__
#define	MURADIN_NET_TCPSERVER_H__

#include <muradin/net/io_service_pool.h>
#include <muradin/net/net_address.h>
#include <muradin/net/io_service.h>
#include <muradin/net/connection.h>
#include <muradin/net/socket_fd.h>
#include <muradin/net/acceptor.h>

#include <muradin/net/user_callback.h>

#include <string>
#include <map>

namespace muradin{
	namespace net{

		class service_thread;

		class tcp_server
		{
		public:
			tcp_server(const std::string& local_ip, boost::uint16_t losten_port,io_service& ios,size_t thread_number);
			~tcp_server();
			void	ready();
			void	stop();

			/// 连接建立回调
			void	set_conn_callback(const connect_callback& func){m_conn_cb = func;};
			/// 消息发送完成回调
			void	set_read_callback(const read_callback& func){m_read_cb = func;};
			/// 消息接收回调
			void	set_write_callback(const write_callback& func){m_write_cb = func;};

			/// 网络错误回调
			void	set_error_callback(const error_callback& func){m_err_cb = func;};
		private:
			void	on_new_conn(SOCKET_FD fd, const endpoint_v4& addr);
			void	on_remove_conn(connection_ptr conn);
		private:
			io_service&	m_io_service;
			acceptor	m_acceptor;
			std::map<SOCKET_FD,connection_ptr>	m_conn_map;
			connect_callback	m_conn_cb;
			read_callback 		m_read_cb;
			write_callback 		m_write_cb;
			error_callback		m_err_cb;
			io_service_pool m_service_pool;
		};
	}
}
#endif//MURADIN_NET_TCPSERVER_H__