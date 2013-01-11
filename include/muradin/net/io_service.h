#ifndef	MURADIN_NET_IOSERVICE_H__
#define MURADIN_NET_IOSERVICE_H__

#include <muradin/net/evt_channle.h>

#include <vector>

#include <boost/utility.hpp>
#include <boost/function.hpp>
#include <boost/smart_ptr.hpp>

namespace muradin{
namespace net{

	class io_services:boost::noncopyable
	{
		typedef	boost::function<void()>		task;
		typedef std::vector<task>			task_list;
		typedef std::vector<evt_channle*>	channle_list;
	public:
		io_services();
		~io_services();

		/// main loop
		void	loop();

		/// run task
		/// if call from same thread as main loop,run immediately.otherwise
		/// the func run latter.
		void	run_task(const task& func);

		/// run task at timepoint
		void	run_task_at(const task& func);
		/// run task  
		void	run_task_after(const task& func,boost::uint32_t milliseconds);
		/// run task 
		void	run_task_every(const task& func,boost::uint32_t milliseconds);

		/// to check caller-therad is the loop thread
		bool	check_this_loop();
		//
		/*
		void	add_channle(evt_channle* channle);
		void	remove_channle(evt_channle* evt_channle);
		*/
	private:
		void	run_pending_task();
		void	week_me_up();
		/// wait for event 
		void	wait_channle(channle_list& saver,boost::uint32_t timeout_ms);
	protected:
		//
	private:
		boost::scoped_ptr<evt_channle>	m_self_channle;
	};
}
}
#endif