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
	m_fd(fd),
	m_last_evt_status(0),
	m_service(ios),
	m_error_cb(NULL),
	m_close_cb(NULL),
	m_read_cb(NULL),
	m_write_cb(NULL)
	{
		//
	}
	evt_channel::~evt_channel()
	{
		//
	}
	void	evt_channel::join_to_service()
	{
		m_service.add_channel(this);
		/*
		if (m_service.check_this_loop()){
			m_service.add_channel(this);
		}else{
			m_service.run_task( boost::bind(&evt_channel::join_to_service,this) );
		}
		*/
		
	}
	void	evt_channel::remove_from_service()
	{
		m_service.del_channel(this);
		/*
		if (m_service.check_this_loop()){
			m_service.del_channel(this);
		}else{
			m_service.run_task( boost::bind(&evt_channel::remove_from_service,this) );
		}
		*/
	}


	void	evt_channel::enable_read(bool enable)
	{

		if(enable){
			m_last_evt_status |= (EPOLLIN|EPOLLHUP|EPOLLPRI);
		}else{
			m_last_evt_status &= (EPOLLIN|EPOLLHUP|EPOLLPRI);
		}
		m_service.update_channel(this);

		/*
		if (m_service.check_this_loop()){
			if(enable){
				m_last_evt_status |= (EPOLLIN|EPOLLHUP|EPOLLPRI);
			}else{
				m_last_evt_status &= (EPOLLIN|EPOLLHUP|EPOLLPRI);
			}
			m_service.update_channel(this);
		}else{
			m_service.run_task( boost::bind(&evt_channel::enable_read,this,enable) );
		}
		*/
	}

	void	evt_channel::enable_write(bool enable)
	{

		if(enable){
			m_last_evt_status |=EPOLLOUT;
		}else{
			m_last_evt_status &=EPOLLOUT;
		}
		m_service.update_channel(this);

		/*
		if (m_service.check_this_loop()){
			if(enable){
				m_last_evt_status |=EPOLLOUT;
			}else{
				m_last_evt_status &=EPOLLOUT;
			}
			m_service.update_channel(this);
		}else{
			m_service.run_task( boost::bind(&evt_channel::enable_write,this,enable) );
		}
		*/

	}
	

	void	evt_channel::process_work()
	{
		trace_evt_flag(m_last_evt_status);
		
		if((m_last_evt_status & EPOLLHUP) && !(m_last_evt_status & EPOLLIN)){
			/// 连接被挂断,并且不可读
			/// 本端引发的错误
			LOG_WARN.stream()<<"EPOLLHUP";
			if(m_close_cb) m_close_cb();
		}
		if( m_last_evt_status & EPOLLERR ){
			/// 本端引发的错误
			LOG_EROR.stream()<<"EPOLLERR,errno:"<< errno;
			if(m_error_cb) m_error_cb();
		}

		if( m_last_evt_status & (EPOLLIN | EPOLLPRI | EPOLLRDHUP) ){
			/// EPOLLRDHUP 会引起 read 读到EOF(即对端半关闭)
			/// 留给上层处理半关闭事件

			//LOG_INFO.stream()<<"EPOLLIN | EPOLLPRI | EPOLLRDHUP";
			if(m_read_cb) m_read_cb();
		}

		if( m_last_evt_status & EPOLLOUT ){
			//LOG_INFO.stream()<<"EPOLLOUT";
			if(m_write_cb) m_write_cb();
		}
	}

}
}
