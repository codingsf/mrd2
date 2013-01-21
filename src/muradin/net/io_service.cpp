#include <muradin/net/io_service.h>
#include <muradin/net/socket.h>
#include <muradin/base/log_warper.h>

#include <assert.h>
#include <unistd.h>
#include <sys/eventfd.h>
#include <signal.h>

#include <boost/bind.hpp>

namespace{
	class igrone_sig_pipe
	{
	public:
		igrone_sig_pipe()
		{
			::signal(SIGPIPE, SIG_IGN);
			//LOG_INFO.stream() << "Ignore SIGPIPE";
		}
	};
	igrone_sig_pipe ig_pipe;

}


int create_evt_fd()
{
	int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
	if (evtfd < 0){
		LOG_FATL.stream()<<"eventfd fail"; 
		abort();
	}
	return evtfd;
}

namespace muradin{
namespace net{
	io_service::io_service() 
	:m_owner_tid(boost::this_thread::get_id()),
	m_poller(new poller_epoll()),
	m_wakeup_fd(create_evt_fd()),	
	m_self_channel(m_wakeup_fd,*this,"io_service-chnl"),		
	m_running_pending_tasks(false),
	m_quit(false)
	{
		assert(m_wakeup_fd > 0);
		
		//LOG_INFO.stream()<<"io_service >>>>>>>>>>>>>>>>>> c-tor ";
		m_self_channel.set_read_cb(boost::bind(&io_service::on_read,this));
		//add_channel(&m_self_channel);
		m_self_channel.join_to_service();
		m_self_channel.enable_read(true);
	}
	io_service::~io_service()
	{
		//del_channel(&m_self_channel);
		m_self_channel.remove_from_service();
	}

	void	io_service::run()
	{
		channel_list	active_channels;
		static const boost::uint32_t poll_timout_ms=1000;
		while(!m_quit){
			active_channels.clear();

			wait_channel(active_channels,poll_timout_ms);
			
			//LOG_INFO.stream()<<"process_work : "<<active_channels.size();

			// dispath io-event first
			for (int i = 0; i < active_channels.size(); ++i){
				active_channels[i]->process_work();
			}
			// run tasks
			run_pending_task();
		}
	}

	void	io_service::quit()
	{
		m_quit=true;
		wake_me_up();
	}
	void	io_service::add_channel(evt_channel* channel)
	{
		if(check_this_loop() ){
			add_channel_in_loop(channel);		
		}else{
			LOG_INFO.stream()<<"";
			queue_task( boost::bind(&io_service::add_channel_in_loop,this,channel));
		}
	}
	void	io_service::del_channel(evt_channel* channel)
	{
		if(check_this_loop() ){
			del_channel_in_loop(channel);		
		}else{
			LOG_INFO.stream()<<"";
			queue_task( boost::bind(&io_service::del_channel_in_loop,this,channel));
		}
	}
	void	io_service::update_channel(evt_channel* channel)
	{
		if(check_this_loop() ){
			update_channel_in_loop(channel);		
		}else{
			LOG_INFO.stream()<<"";
			queue_task( boost::bind(&io_service::update_channel_in_loop,this,channel));
		}
	}

	void	io_service::run_task(const task& func)
	{
		if(check_this_loop()){
			LOG_INFO.stream()<<"run task directly";
			func();
		}else{
			LOG_INFO.stream()<<"enqueue task" ;
			queue_task(func);
		}
	}
	void	io_service::queue_task(const task& func)
	{
		{
			lock_type	lock(m_mutex);
			m_pending_tasks.push_back(func);
			//LOG_INFO.stream()<<"task has enqueued" ;
		}

		if (!check_this_loop() || m_running_pending_tasks )
		{
			// queue_task was called from other thread
			// queue_task was called form loop thread,but pending tasks are running

			wake_me_up();
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
		return m_owner_tid == boost::this_thread::get_id();
	}
	void	io_service::run_pending_task()
	{
		m_running_pending_tasks=true;
		task_list to_run;

		{
			lock_type	lock(m_mutex);
			m_pending_tasks.swap(to_run);
		}

		//LOG_INFO.stream()<<"run_pending_task,size = "<<to_run.size() ;
		for(size_t i=0;i<to_run.size();++i){
			to_run[i]();
		}
		m_running_pending_tasks=false;
	}
	void	io_service::wake_me_up()
	{
		LOG_INFO.stream()<<"wakeup loop" ;
		static const size_t write_bytes=sizeof(void*);
		static const char buff[write_bytes]={0};
		ssize_t ret =::write(m_wakeup_fd,buff,write_bytes);
		assert(ret == write_bytes);
	}

	void	io_service::on_read()
	{
		static const size_t read_bytes=sizeof(void*);
		static char buff[read_bytes]={0};
		ssize_t ret =::read(m_wakeup_fd,buff,read_bytes);
		assert(ret == read_bytes);
	}
	void	io_service::wait_channel(channel_list& saver,boost::uint32_t timeout_ms)
	{
		/// blocking untill:
		///   - get IO-Event form OS
		///   - timeout
		///   - wake_me_up has been called.

		m_poller->wait_for_evt(saver,timeout_ms);
	}

	void	io_service::add_channel_in_loop(evt_channel* channel)
	{
		LOG_INFO.stream()<<"";
		if( !check_this_loop()){ 
			LOG_EROR.stream()<<"must run in-loop";
		}
		m_poller->add_channel(channel);
	}

	void	io_service::del_channel_in_loop(evt_channel* channel)
	{
		LOG_INFO.stream()<<"";
		if( !check_this_loop()){ 
			LOG_EROR.stream()<<"must run in-loop";
		}
		m_poller->del_channel(channel);	
	}

	void	io_service::update_channel_in_loop(evt_channel* channel)
	{
		LOG_INFO.stream()<<"";
		if( !check_this_loop()){ 
			LOG_EROR.stream()<<"must run in-loop";
		}
		m_poller->update_evt_code(channel);
	}

}
}