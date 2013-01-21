#ifndef	MURADIN_NET_SERVICE_THREAD_H__
#define MURADIN_NET_SERVICE_THREAD_H__

#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>

namespace muradin{
namespace net{

	class io_service;
	class service_thread:boost::noncopyable
	{
	public:
		service_thread();
		~service_thread();
		io_service*	run();
		void		join();
	private:
		void		thread_func();
	private:

		typedef boost::mutex 					mutex_type;
		typedef boost::unique_lock<mutex_type>	lock_type;

		io_service*							m_service;
		mutex_type							m_mutex;
		boost::condition_variable 			m_cond;
		boost::scoped_ptr<boost::thread>	m_thread;
	};
}
}
#endif//MURADIN_NET_SERVICE_THREAD_H__