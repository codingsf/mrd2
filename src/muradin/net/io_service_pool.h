#ifndef MURADIN_NET_IOSERVICE_POOL_H__
#define MURADIN_NET_IOSERVICE_POOL_H__

#include <vector>

#include <boost/utility.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace muradin{
namespace net{

	class service_thread;
	class io_service;
	class io_service_pool :boost::noncopyable
	{
		typedef	boost::ptr_vector<service_thread>	thread_vec;
		typedef	std::vector<io_service*>			service_vec;
	public:
		/// size must >= 1
		io_service_pool(size_t size);
		~io_service_pool();
		io_service*		next_service();
		size_t			size();
		void			run();
		void			quit();
	private:
		service_vec		m_services;
		thread_vec		m_threads;
		size_t 			m_size;
		size_t			m_service_index;
	} ;
}
}
#endif//MURADIN_NET_IOSERVICE_POOL_H__