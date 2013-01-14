#ifndef	MURADIN_NET_IOSERVICE_H__
#define MURADIN_NET_IOSERVICE_H__

#include <muradin/net/evt_channle.h>
#include <muradin/net/socket_fd.h>
#include <muradin/net/ep_poller.h>
#inc

#include <vector>

#include <boost/utility.hpp>
#include <boost/function.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>

namespace muradin{
namespace net{

	class io_services:boost::noncopyable
	{
		typedef	boost::function<void()>		task;
		typedef std::vector<task>			task_list;
		typedef std::vector<evt_channle*>	channel_list;
		typedef boost::mutex 				mutex_type;
		typedef boost::unique_lock<mutex_type>	lock_type;
	public:
		io_services();
		~io_services();

		/// main loop
		void	loop();

		/// exit main loop
		void	exit();

		/// add new channel 
		void	add_channle(evt_channle* channel);

		/// remove a channel
		void	del_channle(evt_channle* channel);

		/// update a channel for subcrib event
		void	update_channle(evt_channle* channel);

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
	protected:
		//
	private:
		void	run_pending_task();

		/// task enqueue
		void	queue_task(const task& func);

		void	on_read();
		void	week_me_up();
		/// wait for event 
		void	wait_channle(channel_list& saver,boost::uint32_t timeout_ms);
	private:
		const boost::thread::id 		m_loop_owner_id;
		boost::scoped_ptr<poller_epoll>	m_poller;
		SOCKET_FD 						m_weekup_fd;
		evt_channle						m_self_channel;
		bool							m_running_pending_tasks;
		bool							m_exit;

		task_list						m_pending_tasks;
		mutex_type						m_mutex;
	};
}
}
#endif