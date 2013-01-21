#include <muradin/base/blocking_counter.h>

namespace muradin{
namespace base{

	blocking_counter::blocking_counter(int init_count)
	:m_val(init_count)
	{
		//
	}
	blocking_counter::~blocking_counter()
	{
		//
	}
	int	blocking_counter::get_val()
	{
		lock_type lock(m_mutex);
		return m_val;
	}

	int blocking_counter::increment()
	{
		{
			lock_type lock(m_mutex);
			++m_val;
		}
		m_cond.notify_one();
	}

	int blocking_counter::decrement()
	{
		{
			lock_type lock(m_mutex);
			--m_val;
		}
		m_cond.notify_one();
	}

	void	blocking_counter::wait_for(int val )
	{
		lock_type lock(m_mutex);
		while(val != m_val){
			m_cond.wait(lock);
		}
	}
}
}