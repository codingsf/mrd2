#include <muradin/net/service_thread.h>
#include <muradin/net/io_service.h>

#include <boost/function.hpp>
#include <boost/bind.hpp>

namespace muradin{
namespace net{

	service_thread::service_thread()
	:m_service(NULL),
	m_thread(new boost::thread(boost::bind(&service_thread::thread_func,this)) )
	{
		//
	}
	service_thread::~service_thread()
	{
		//
	}
	io_service*	service_thread::run()
	{
		lock_type lock(m_mutex);
		while(m_service == NULL){
			m_cond.wait(lock);
		}
		return m_service;
	}
	void		service_thread::join()
	{
		m_thread->join();
	}
	void		service_thread::thread_func()
	{
		io_service ios;

		{
			lock_type lock(m_mutex);
			m_service=&ios;
		}
		m_cond.notify_one();
		ios.run();
	}
}
}