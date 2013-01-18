#ifndef	MURADIN_NET_TCPSERVER_H__
#define	MURADIN_NET_TCPSERVER_H__

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
		class tcp_server
		{
		public:
			tcp_server(const std::string& local_ip, boost::uint16_t losten_port,io_service& ios);
			~tcp_server();
			void	ready();
			void	stop();

			/// 连接建立回调
			void	set_conn_cb(const on_connect& func){m_conn_cb = func;};
			/// 消息发送完成回调
			void	set_msg_cb(const on_msg& func){m_msg_cb = func;};
			/// 消息接收回调
			void	set_msg_complete_cb(const on_msg_complete& func){m_msg_complete_cb = func;};
		private:
			void	on_new_conn();
			void	on_remove_conn(connection_ptr conn);
		private:
			io_service&	m_io_service;
			acceptor	m_acceptor;
			std::map<SOCKET_FD,connection_ptr>	m_conn_map;
			on_connect	m_conn_cb;
			on_msg 		m_msg_cb;
			on_msg_complete m_msg_complete_cb;
		};
	}
}
#endif//MURADIN_NET_TCPSERVER_H__