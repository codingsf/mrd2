#ifndef	MURADIN_NET_EVTCHANNLE_H__
#define	MURADIN_NET_EVTCHANNLE_H__


#include <muradin/net/socket_fd.h>

#include <string>

#include <boost/function.hpp>
#include <boost/weak_ptr.hpp>

namespace muradin {
namespace net{

	class io_service;

	class evt_channel{
	public:
		typedef	boost::function<void()>		evt_notify_functor;

	public:
		evt_channel(SOCKET_FD fd,io_service& ios,const std::string& name="unknow");
		~evt_channel();
		std::string	get_name()const{return m_name;};
		void	join_to_service();
		void	remove_from_service();
		void	enable_read(bool enable);
		void	enable_write(bool enable);


		void	set_error_callback(const evt_notify_functor& func){m_error_callback = func;};
		void	set_close_callback(const evt_notify_functor& func){m_close_callback = func;};
		void	set_read_callback(const evt_notify_functor& func){m_read_callback = func;};
		void	set_write_callback(const evt_notify_functor& func){m_write_callback = func;};
		
		/// Tie this channel to the owner object managed by shared_ptr,
		/// prevent the owner object being destroyed in handleEvent.
		void 	tie( const boost::shared_ptr<void>& observer);

		/// for evt-poller
		void	save_evt_status(int status){m_last_evt_status=status;};
		
		int		evt_status()const {return m_last_evt_status;};

		SOCKET_FD fd(){return m_fd;};
		
		/// handle event.for io_service
		void	process_work();
	private:
		void	do_process_work();
	private:
		std::string				m_name;
		bool					m_tied;
		SOCKET_FD 				m_fd;
		int						m_last_evt_status;
		io_service&				m_service;
		evt_notify_functor		m_error_callback;
		evt_notify_functor		m_close_callback;
		evt_notify_functor		m_read_callback;
		evt_notify_functor		m_write_callback;
  		boost::weak_ptr<void> 	m_owner_ob;
  		bool					m_progress_in_write;

	};
}
}
#endif//MURADIN_NET_EVTCHANNLE_H__