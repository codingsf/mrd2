#include <muradin/net/io_service_pool.h>
#include <muradin/net/service_thread.h>
#include <muradin/net/io_service.h>

namespace muradin{
namespace net{

	io_service_pool::io_service_pool(size_t size)
	:m_size(size),m_service_index(0)
	{
		//
	}
	io_service_pool::~io_service_pool()
	{
		//
	}
	io_service*		io_service_pool::next_service()
	{
		if(m_service_index >= m_size) m_service_index=0;

		return m_services[m_service_index++];
	}
	size_t			io_service_pool::size()
	{
		return m_size;
	}
	void			io_service_pool::run()
	{
		for (size_t i = 0; i < m_size; ++i){
			m_threads.push_back( new service_thread());
		}
		for (size_t i = 0; i < m_size; ++i){
			m_services.push_back( m_threads[i].run());
		}
		
	}
	void			io_service_pool::quit()
	{
		for (size_t i = 0; i < m_size; ++i){
			m_services[i]->quit();
		}
		for (size_t i = 0; i < m_size; ++i){
			m_threads[i].join();
		}
	}
}
}