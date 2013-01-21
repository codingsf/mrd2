#ifndef	MURADIN_BASE_BLOCKING_COUNTER_H__
#define MURADIN_BASE_BLOCKING_COUNTER_H__

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>

namespace muradin{
namespace base{
	class blocking_counter
	{
		typedef boost::mutex 					mutex_type;
		typedef boost::unique_lock<mutex_type>	lock_type;

	public:
		blocking_counter(int init_count=0);
		~blocking_counter();
		int	get_val();

		/// increment ,return old value
		int increment();

		/// increment ,return old value
		int decrement();
	
		void	wait_for(int val);
	private:
		mutex_type							m_mutex;
		boost::condition_variable 			m_cond;
		int 								m_val;
	};
}
}
#endif//MURADIN_BASE_BLOCKING_COUNTER_H__