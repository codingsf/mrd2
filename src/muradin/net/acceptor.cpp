#include <muradin/net/acceptor.h>
#include <muradin/net/socket_ctl.h>

namespace muradin{
namespace net{
	acceptor::acceptor(const endpoint_v4& local_addr,io_service& ios)
	:m_service(ios),
	m_socket(socket::create()),
	m_accept_cb(NULL),
	m_addr(local_addr),
	m_channel(m_socket.fd(),m_service,"acceptor-chnl" )
	{
		assert(m_socket.fd() > 0);

		socket::bind(m_socket.fd(),m_addr); 				/// FIXME::check
		socket_ctl::set_tcp_nodelay(m_socket.fd(),true);	/// FIXME::check
		socket_ctl::set_reuse_addr(m_socket.fd(),true);		/// FIXME::check
		socket_ctl::set_recv_buff_size(m_socket.fd(),16*1024);		/// FIXME::check


		m_channel.set_read_cb(boost::bind(&acceptor::on_read,this));
		m_channel.join_to_service();
		//m_service.add_channel(&m_channel);
	}

	acceptor::~acceptor()
	{
	}
	
	void	acceptor::start()
	{
		socket::listen(m_socket.fd(),256);  /// FIXME::check
		m_channel.enable_read(true);
	}
	
	void	acceptor::stop()
	{
		m_channel.enable_read(false);
		//m_service.del_channel(&m_channel);
		m_channel.remove_from_service();
	}
	

	void	acceptor::on_read()
	{
		m_accept_cb();
	}
		
}
}
