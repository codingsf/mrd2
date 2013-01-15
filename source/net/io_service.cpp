#include <muradin/net/io_service.h>
#include <muradin/net/socket.h>

#include <assert.h>
#include <unistd.h>

#include <boost/bind.hpp>

namespace muradin{
namespace net{
	io_service::io_service()
	:m_loop_owner_id(boost::this_thread::get_id()),
	m_poller(new poller_epoll()),
	m_weekup_fd(socket::create()),	
	m_self_channel(m_weekup_fd,*this),		
	m_running_pending_tasks(false),
	m_exit(false)
	{
		assert(m_weekup_fd > 0);
		
		m_self_channel.set_read_cb(boost::bind(&io_service::on_read,this));
		add_channel(&m_self_channel);
		m_self_channel.enable_read(true);
	}
	io_service::~io_service()
	{
		del_channel(&m_self_channel);
	}

	void	io_service::loop()
	{
		channel_list	active_channels;
		static const boost::uint32_t poll_timout_ms=1000;
		while(m_exit){
			active_channels.clear();

			wait_channel(active_channels,poll_timout_ms);
			
			// dispath io-event first
			for (int i = 0; i < active_channels.size(); ++i){
				active_channels[i]->process_work();
			}
			// run tasks
			run_pending_task();
		}
	}

	void	io_service::add_channel(evt_channel* channel)
	{
		assert(check_this_loop() == true);
		m_poller->add_channel(channel);
	}
	void	io_service::del_channel(evt_channel* channel)
	{
		assert(check_this_loop() == true);
		m_poller->del_channel(channel);	
	}
	void	io_service::update_channel(evt_channel* channel)
	{
		assert(check_this_loop() == true);
		m_poller->update_evt_code(channel);
	}

	void	io_service::run_task(const task& func)
	{
		if(check_this_loop()){
			func();
		}else{
			queue_task(func);
		}
	}
	void	io_service::queue_task(const task& func)
	{
		{
			lock_type	lock(m_mutex);
			m_pending_tasks.push_back(func);
		}

		if (!check_this_loop() || m_running_pending_tasks )
		{
			// queue_task was called from other thread
			// queue_task was called form loop thread,but pending tasks are running
			week_me_up();
		}
	}
	void	io_service::run_task_at(const task& func)
	{
		//
	}
	void	io_service::run_task_after(const task& func,boost::uint32_t milliseconds)
	{
		//
	}
	void	io_service::run_task_every(const task& func,boost::uint32_t milliseconds)
	{
		//
	}

	bool	io_service::check_this_loop()
	{
		return m_loop_owner_id == boost::this_thread::get_id();
	}
	void	io_service::run_pending_task()
	{
		m_running_pending_tasks=true;
		task_list to_run;

		{
			lock_type	lock(m_mutex);
			m_pending_tasks.swap(to_run);
		}

		for(size_t i=0;i<to_run.size();++i){
			to_run[i]();
		}
		m_running_pending_tasks=false;
	}
	void	io_service::week_me_up()
	{
		static const size_t write_bytes=sizeof(void*);
		static const char buff[write_bytes]={0};
		::write(m_weekup_fd,buff,write_bytes);
	}

	void	io_service::on_read()
	{
		static const size_t read_bytes=sizeof(void*);
		static char buff[read_bytes]={0};
		::read(m_weekup_fd,buff,read_bytes);
	}
	void	io_service::wait_channel(channel_list& saver,boost::uint32_t timeout_ms)
	{
		/// blocking untill:
		///   - get IO-Event form OS
		///   - timeout
		///   - week_me_up has been called.

		m_poller.wait_for_evt(saver,timeout_ms);
	}
	void	io_service::exit()
	{
		assert(check_this_loop() == true);
		m_exit=true;
		week_me_up();
	}
}
}