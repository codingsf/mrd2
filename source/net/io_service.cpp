#include <muradin/net/io_service.h>
#include <muradin/net/socket.h>

#include <assert.h>
#include <unistd.h>

#include <boost/bind.hpp>

namespace muradin{
namespace net{
	io_services::io_services()
	:m_loop_owner_id(boost::this_thread::get_id()),
	m_poller(new poller_epoll())
	m_weekup_fd(socket::create()),	
	m_self_channel(new evt_channel(m_weekup_fd,*this)),		
	m_running_pending_tasks(false),
	m_exit(false)
	{
		assert(m_weekup_fd > 0);
		
		m_self_channel.set_read_cb(boost::bind(&io_services::on_read,this));
		add_channle(&m_self_channel);
		m_self_channel.subscrib_read_evt();
	}
	io_services::~io_services()
	{
		del_channle(&m_self_channel);
	}

	void	io_services::loop()
	{
		channel_list	active_channels;
		static const boost::uint32_t poll_timout_ms=1000;
		while(m_exit){
			active_channles.clear();

			wait_channle(active_channels,poll_timout_ms);
			
			// dispath io-event first
			for (int i = 0; i < active_channles.size(); ++i){
				active_channles[i]->process_work();
			}
			// run tasks
			run_pending_task();
		}
	}

	void	io_services::add_channle(evt_channle* channel)
	{
		assert(check_this_loop() == true);
		m_poller->add_channle(channel);
	}
	void	io_services::del_channle(evt_channle* channel)
	{
		assert(check_this_loop() == true);
		m_poller->del_channle(channel);	
	}
	void	io_services::update_channle(evt_channle* channel)
	{
		assert(check_this_loop() == true);
		m_poller->update_channle(channel);
	}

	void	io_services::run_task(const task& func)
	{
		if(check_this_loop()){
			func();
		}else{
			queue_task(func);
		}
	}
	void	io_services::queue_task(const task& func)
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
	void	io_services::run_task_at(const task& func)
	{
		//
	}
	void	io_services::run_task_after(const task& func,boost::uint32_t milliseconds)
	{
		//
	}
	void	io_services::run_task_every(const task& func,boost::uint32_t milliseconds)
	{
		//
	}

	bool	io_services::check_this_loop()
	{
		return m_loop_owner_id == boost::this_thread::get_id();
	}
	void	io_services::run_pending_task()
	{
		m_running_pending_tasks=true;
		{
			task_list to_run;

			lock_type	lock(m_mutex);
			m_pending_tasks.swap(to_run);
		}

		for(size_t i=0;i<to_run.szie();++i){
			to_run[i]();
		}
		m_running_pending_tasks=false;
	}
	void	io_services::week_me_up()
	{
		static const size_t write_bytes=sizeof(void*);
		static const char buff[write_bytes]={0};
		::write(m_weekup_fd,buff,write_bytes);
	}

	void	io_services::on_read()
	{
		static const size_t read_bytes=sizeof(void*);
		static char buff[read_bytes]={0};
		::read(m_weekup_fd,buff,read_bytes);
	}
	void	io_services::wait_channle(channle_list& saver,boost::uint32_t timeout_ms)
	{
		/// blocking untill:
		///   - get IO-Event form OS
		///   - timeout
		///   - week_me_up has been called.

		m_poller.wait_for_evt(saver,timeout_ms);
	}
	void	io_services::exit()
	{
		assert(check_this_loop() == true);
		m_exit=true;
		week_me_up();
	}
}
}