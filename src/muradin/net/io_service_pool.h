#ifndef MURADIN_NET_IOSERVICE_POOL_H__
#define MURADIN_NET_IOSERVICE_POOL_H__


#include <boost/utility.hpp>

namespace muradin{
namespace net{

	class io_service;
	class io_service_pool :boost::noncopyable
	{
	public:
		/// size must >= 1
		io_service_pool(size_t size);
		~io_service_pool();
		io_service&		next_service();
		size_t		size();
		void		run();
		void		quit();
	private:
		//
	} ;
}
}
#endif//MURADIN_NET_IOSERVICE_POOL_H__