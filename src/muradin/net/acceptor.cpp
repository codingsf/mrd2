#include <muradin/net/acceptor.h>

namespace muradin{
namespace net{
	acceptor::acceptor(const endpoint_v4& local_addr,io_service& ios)
	:m_service(ios),
	m_socket(socket::create()),
	m_accept_cb(NULL),
	m_addr(local_addr),
	m_channel(m_socket.fd(),m_service)
	{
		assert(m_socket.fd() > 0);

		socket::bind(m_socket.fd(),m_addr); /// FIXME::check
		m_channel.set_read_cb(boost::bind(&acceptor::on_read,this));
		m_service.add_channel(&m_channel);
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
		m_service.del_channel(&m_channel);
	}
	

	void	acceptor::on_read()
	{
		m_accept_cb();
	}
		
}
}