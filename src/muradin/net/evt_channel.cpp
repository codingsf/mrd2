#include <muradin/net/evt_channel.h>
#include <muradin/base/log_warper.h>
#include <muradin/net/io_service.h>

#include <sys/epoll.h>
#include <assert.h>

void trace_evt_flag(int flag)
{
	std::string log_str;
	if(flag& EPOLLHUP) 		log_str+="EPOLLHUP ";
	if(flag& EPOLLIN)  		log_str+="EPOLLIN ";
	if(flag& EPOLLOUT) 		log_str+="EPOLLOUT ";
	if(flag& EPOLLRDHUP) 	log_str+="EPOLLRDHUP ";
	if(flag& EPOLLPRI) 		log_str+="EPOLLPRI ";
	if(flag& EPOLLERR) 		log_str+="EPOLLERR ";
	if(flag& EPOLLET)  		log_str+="EPOLLET ";
	if(flag& EPOLLONESHOT) 	log_str+="EPOLLONESHOT ";

	if(log_str.length())
		LOG_DEBG.stream()<<log_str;
}

namespace muradin{
namespace net{

	evt_channel::evt_channel(SOCKET_FD fd,io_service& ios,const std::string& name)
	:m_name(name),
	m_tied(false),
	m_fd(fd),
	m_last_evt_status(0),
	m_service(ios),
	m_error_callback(NULL),
	m_close_callback(NULL),
	m_read_callback(NULL),
	m_write_callback(NULL),
	m_progress_in_write(false)
	{
		//
	}
	evt_channel::~evt_channel()
	{
		//
	}

	void 	evt_channel::tie(const boost::shared_ptr<void>& observer)
	{
		assert(m_tied == false);
		m_owner_ob = observer;
		m_tied = true;
	}
	void	evt_channel::join_to_service()
	{
		m_service.add_channel(this);
	}
	void	evt_channel::remove_from_service()
	{
		m_service.del_channel(this);
	}


	void	evt_channel::enable_read(bool enable)
	{

		if(enable){
			m_last_evt_status |= (EPOLLIN|EPOLLHUP|EPOLLPRI);
		}else{
			m_last_evt_status &= (EPOLLIN|EPOLLHUP|EPOLLPRI);
		}
		m_service.update_channel(this);
	}

	void	evt_channel::enable_write(bool enable)
	{

		if(enable && m_progress_in_write) return;

		if(enable){
			m_last_evt_status |=EPOLLOUT;
		}else{
			m_last_evt_status &=EPOLLOUT;
		}
		m_service.update_channel(this);
		m_progress_in_write = enable;
	}
	
	void	evt_channel::do_process_work()
	{
		//trace_evt_flag(m_last_evt_status);
		
		if((m_last_evt_status & EPOLLHUP) && !(m_last_evt_status & EPOLLIN)){
			/// 连接被挂断,并且不可读
			/// 本端引发的错误

			//LOG_WARN.stream()<<"EPOLLHUP";
			if(m_close_callback) m_close_callback();
		}
		if( m_last_evt_status & EPOLLERR ){
			/// 本端引发的错误

			//LOG_WARN.stream()<<"EPOLLERR,errno:"<< errno;
			if(m_error_callback) m_error_callback();
		}

		if( m_last_evt_status & (EPOLLIN | EPOLLPRI | EPOLLRDHUP) ){
			/// EPOLLRDHUP 会引起 read 读到EOF(即对端半关闭)
			/// 留给上层处理半关闭事件

			//LOG_INFO.stream()<<"EPOLLIN | EPOLLPRI | EPOLLRDHUP";
			if(m_read_callback) m_read_callback();
		}

		if( m_last_evt_status & EPOLLOUT ){
			m_progress_in_write=false;
			//LOG_INFO.stream()<<"EPOLLOUT";
			if(m_write_callback) m_write_callback();
		}
	}

	void	evt_channel::process_work()
	{
		boost::shared_ptr<void> chenck_not_null;
		if (m_tied){
			chenck_not_null = m_owner_ob.lock();
			if (chenck_not_null){
				do_process_work();
			}else{
				LOG_INFO.stream()<<"connection destroyed,skip process_work";
			}
		}else{
			do_process_work();
		}
	}

}
}
